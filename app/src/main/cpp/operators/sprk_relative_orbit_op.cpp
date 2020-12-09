// Copyright (c) Tech Soft 3D, Inc.
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "sprk_ops.h"

HPS::RelativeOrbitOperator::RelativeOrbitOperator(MouseButtons in_mouse_trigger, ModifierKeys in_modifier_trigger)
	: Operator(in_mouse_trigger, in_modifier_trigger)
	, center_of_rotation(Point(0, 0, 0))
{
	selection_options
		.SetLevel(HPS::Selection::Level::Entity)
		.SetAlgorithm(HPS::Selection::Algorithm::Visual)
		.SetRelatedLimit(0)
		.SetProximity(0.1f);
}

bool HPS::RelativeOrbitOperator::OnMouseDown(MouseState const & in_state)
{
	if(IsMouseTriggered(in_state))
	{
		operator_active = true;
		start_point = in_state.GetLocation();
		CalculateTarget(in_state.GetEventPath());

		OperatorUtility::ScreenToSphereMousePoint(start_point, start_sphere_pos);
		tracked_touch_ID = -1;
		return true;
	}
	return false;
}


bool HPS::RelativeOrbitOperator::OnMouseUp(MouseState const & in_state)
{
	if(operator_active && !IsMouseTriggered(in_state))
		operator_active = false;
	return false;
}

bool HPS::RelativeOrbitOperator::OnMouseMove(MouseState const & in_state)
{
	bool ret = false;
	if(operator_active && IsMouseTriggered(in_state))
	{
		HPS::KeyPath key_path = in_state.GetEventPath();
		ret = RelativeOrbitCommon(in_state.GetLocation(), key_path);
	}
	
	return ret;
}

bool HPS::RelativeOrbitOperator::OnTouchDown(TouchState const & in_state)
{
	TouchArray touches = in_state.GetActiveEvent().Touches;
	if (touches.empty())
		return false;

	if(tracked_touch_ID == -1)
	{
		tracked_touch_ID = touches[0].ID;
		start_point = touches[0].Location;
		if (touches[0].TapCount != 1)
			return false;

		CalculateTarget(in_state.GetEventPath());
		OperatorUtility::ScreenToSphereMousePoint(start_point, start_sphere_pos);
		return true;
	}

	return false;
}

bool HPS::RelativeOrbitOperator::OnTouchUp(TouchState const & in_state)
{
	TouchArray up_touches = in_state.GetActiveEvent().Touches;

	for(size_t i = 0, e = up_touches.size(); i < e && tracked_touch_ID != -1; ++i)
	{
		if(up_touches[i].ID == tracked_touch_ID)
			tracked_touch_ID = -1;
	}
	return true;
}

bool HPS::RelativeOrbitOperator::OnTouchMove(TouchState const & in_state)
{
	TouchArray touches = in_state.GetActiveEvent().Touches;
	if (tracked_touch_ID == -1 || touches.empty())
		return false;

    bool found = false;
	Touch the_one = touches[0];

	for(size_t i = 0, e = touches.size(); i < e; ++i)
	{
		if(touches[i].ID == tracked_touch_ID)
		{
			the_one = touches[i];
            found = true;
			break;
		}
	}
    
    if (!found)
        return false;

	HPS::KeyPath key_path = in_state.GetEventPath();
	return RelativeOrbitCommon(the_one.Location, key_path);
}

bool HPS::RelativeOrbitOperator::OrbitCommon(HPS::WindowPoint const & in_loc, HPS::CameraKit & out_camera)
{
	new_point = in_loc;
	OperatorUtility::ScreenToSphereMousePoint(new_point, new_sphere_pos);

	// get the axis of rotation
	HPS::Vector axis = start_sphere_pos.Cross(new_sphere_pos);

	// this is for screen mouse based movement
	HPS::Vector vtmp = new_point - start_point;
	float dist = static_cast<float>(vtmp.Length()) * 90.0f;

	if ((axis.x != 0.0f || axis.y != 0.0f || axis.z != 0.0f))
	{
		View view = GetAttachedView();
		SegmentKey key = view.GetSegmentKey();
		HPS::Drawing::Handedness worldHandedness;

		if (key.GetDrawingAttributeControl().ShowWorldHandedness(worldHandedness)
			&& worldHandedness == HPS::Drawing::Handedness::Right)
		{
			axis.y *= -1;
			axis.z *= -1;
		}

		axis.Normalize();

		float	theta = OperatorUtility::CalculateAngleOrbitOnPlane(axis, OperatorUtility::Plane_YZ);
		float	angle2 = axis.x < 0 ? -theta * dist : theta * dist;

		theta = OperatorUtility::CalculateAngleOrbitOnPlane(axis, OperatorUtility::Plane_XZ);
		float	angle1 = axis.y < 0 ? theta * dist : -theta * dist;

		theta = OperatorUtility::CalculateAngleOrbitOnPlane(axis, OperatorUtility::Plane_XY);
		float	angle3 = axis.z < 0 ? theta * dist : -theta * dist;

		out_camera.Orbit(angle1, angle2);
		out_camera.Roll(angle3);
	}
	start_sphere_pos = new_sphere_pos;
	start_point = new_point;

	return true;
}

void HPS::RelativeOrbitOperator::CalculateTarget(KeyArray const & in_event_path)
{
	SelectionResults results;
	size_t ret = WindowKey(in_event_path.back()).GetSelectionControl().SelectByPoint(start_point, selection_options, results);
	if (ret)
	{
		HPS::WorldPoint selection;
		HPS::SelectionResultsIterator iter = results.GetIterator();
		HPS::SelectionItem item = iter.GetItem();

		item.ShowSelectionPosition(selection);
		center_of_rotation = selection;
	}
	else
		SegmentKey(in_event_path.front()).GetCameraControl().ShowTarget(center_of_rotation);
}

bool HPS::RelativeOrbitOperator::RelativeOrbitCommon(HPS::WindowPoint const & in_loc, HPS::KeyPath & in_event_path)
{
	View view = GetAttachedView();
	SegmentKey view_segment = view.GetSegmentKey();
	CameraKit camera;
	view_segment.ShowCamera(camera);

	Point target;
	camera.ShowTarget(target);

	Point xform_center_of_rotation;
	Point xform_camera_target;

	in_event_path.ConvertCoordinate(HPS::Coordinate::Space::World, center_of_rotation, HPS::Coordinate::Space::Camera, xform_center_of_rotation);
	in_event_path.ConvertCoordinate(HPS::Coordinate::Space::World, target, HPS::Coordinate::Space::Camera, xform_camera_target);

	Vector dolly_delta = xform_center_of_rotation - xform_camera_target;

	camera.Dolly(dolly_delta.x, dolly_delta.y, dolly_delta.z);
	bool ret = OrbitCommon(in_loc, camera);
	camera.Dolly(-dolly_delta.x, -dolly_delta.y, -dolly_delta.z);

	if (ret)
	{
		view_segment.SetCamera(camera);
		Database::GetEventDispatcher().InjectEvent(CameraChangedEvent(GetAttachedView()));
		view.Update();
	}

	return ret;
}

bool HPS::RelativeOrbitOperator::ShowCenterOfRotation(HPS::Point & out_center_of_rotation) const
{
	if (operator_active)
	{
		out_center_of_rotation = center_of_rotation;
		return true;
	}
	else
		return false;
}
