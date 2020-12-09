// Copyright (c) Tech Soft 3D, Inc.
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "sprk_ops.h"

HPS::OrbitOperator::OrbitOperator(MouseButtons in_mouse_trigger, ModifierKeys in_modifier_trigger)
	: Operator(in_mouse_trigger, in_modifier_trigger), tracked_touch_ID(-1), operator_active(false)
{

}

bool HPS::OrbitOperator::OnMouseDown(MouseState const & in_state)
{
	if(IsMouseTriggered(in_state))
	{
		operator_active = true;
		start_point = in_state.GetLocation();
		OperatorUtility::ScreenToSphereMousePoint(start_point, start_sphere_pos);
		tracked_touch_ID = -1;
		return true;
	}
	return false;
}

bool HPS::OrbitOperator::OnMouseUp(MouseState const & in_state)
{
	if(operator_active && !IsMouseTriggered(in_state))
		operator_active = false;
	return false;
}

bool HPS::OrbitOperator::OnMouseMove(MouseState const & in_state)
{
	if(operator_active && IsMouseTriggered(in_state) && OrbitCommon(in_state.GetLocation()))
	{
		GetAttachedView().Update();
		return true;
	}

	return false;
}

bool HPS::OrbitOperator::OnTouchDown(TouchState const & in_state)
{
	TouchArray touches = in_state.GetActiveEvent().Touches;
	if (touches.empty())
		return false;

	if(tracked_touch_ID == -1)
	{
		tracked_touch_ID = touches[0].ID;
		start_point = touches[0].Location;
		OperatorUtility::ScreenToSphereMousePoint(start_point, start_sphere_pos);
        if (touches[0].TapCount == 1)
            return true;
        else
            return false;
	}

	return false;
}

bool HPS::OrbitOperator::OnTouchUp(TouchState const & in_state)
{
	TouchArray up_touches = in_state.GetActiveEvent().Touches;

	for(size_t i = 0, e = up_touches.size(); i < e && tracked_touch_ID != -1; ++i)
	{
		if(up_touches[i].ID == tracked_touch_ID)
			tracked_touch_ID = -1;
	}
	return true;
}

bool HPS::OrbitOperator::OnTouchMove(TouchState const & in_state)
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
    
	bool ret = OrbitCommon(the_one.Location);
	if(ret) 
		GetAttachedView().Update();
	return ret;
}

bool HPS::OrbitOperator::OrbitCommon(HPS::WindowPoint const & in_loc)
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

		HPS::CameraKit cam;
		key.ShowCamera(cam);
		cam.Orbit(angle1, angle2);
		cam.Roll(angle3);
		key.SetCamera(cam);
		Database::GetEventDispatcher().InjectEvent(CameraChangedEvent(GetAttachedView()));
	}

	start_sphere_pos = new_sphere_pos;
	start_point = new_point;

	return true;
}





