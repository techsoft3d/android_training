// Copyright (c) Tech Soft 3D, Inc.
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "sprk_ops.h"

HPS::TurntableOperator::TurntableOperator(MouseButtons in_mouse_trigger, ModifierKeys in_modifier_trigger)
	: Operator(in_mouse_trigger, in_modifier_trigger), tracked_touch_ID(-1)
{
	center_point_set = false;
	selection_options = HPS::SelectionOptionsKit::GetDefault();
}

bool HPS::TurntableOperator::OnMouseDown(MouseState const & in_state)
{
	if(IsMouseTriggered(in_state))
	{
		if (in_state.GetModifierKeys().Control())
			CalculateCenterPoint(in_state.GetEventSource(), in_state.GetLocation());
		else
		{
			operator_active = true;
			start_point = in_state.GetLocation();
			tracked_touch_ID = -1;
		}
		return true;
	}
	return false;
}

bool HPS::TurntableOperator::OnMouseUp(MouseState const & in_state)
{
	if(operator_active && !IsMouseTriggered(in_state))
		operator_active = false;
	return false;
}

bool HPS::TurntableOperator::OnMouseMove(MouseState const & in_state)
{
	if(operator_active && IsMouseTriggered(in_state))
	{
		HPS::WindowPoint delta;
		delta.x = in_state.GetLocation().x - start_point.x;
		delta.y = in_state.GetLocation().y - start_point.y;
		delta.z = in_state.GetLocation().z - start_point.z;

		TurntableCommon(delta, HPS::Vector(0, 0, 1));

		start_point = in_state.GetLocation();

		Database::GetEventDispatcher().InjectEvent(CameraChangedEvent(GetAttachedView()));
		GetAttachedView().Update();
		
		return true;
	}

	return false;
}

bool HPS::TurntableOperator::OnMouseWheel(HPS::MouseState const & in_state)
{
	const MouseEvent event = in_state.GetActiveEvent();
	const float mouse_wheel_sensitivity = 0.03f;					

	HPS::Point point1(1, 0, 0), point2(0, 0, 0);
	HPS::WindowKey window = in_state.GetEventSource();
	window.ConvertCoordinate(HPS::Coordinate::Space::InnerWindow, point1, HPS::Coordinate::Space::World, point1);
	window.ConvertCoordinate(HPS::Coordinate::Space::InnerWindow, point2, HPS::Coordinate::Space::World, point2);
	HPS::Vector rotation_axis = point2 - point1;
	rotation_axis.Normalize();

	TurntableCommon(mouse_wheel_sensitivity * event.WheelDelta, rotation_axis);
	Database::GetEventDispatcher().InjectEvent(CameraChangedEvent(GetAttachedView()));
	GetAttachedView().Update();
	return true;
}

bool HPS::TurntableOperator::OnTouchDown(TouchState const & in_state)
{
	TouchArray touches = in_state.GetActiveEvent().Touches;
	if (touches.empty())
		return false;
    
    if (tracked_touch_ID == -1)
    {
        if (touches[0].TapCount == 2)
            CalculateCenterPoint(in_state.GetEventSource(), touches[0].Location);
        else if (touches[0].TapCount == 1)
        {
			tracked_touch_ID = touches[0].ID;
			start_point = touches[0].Location;
        }
		else
			return false;

        return true;
    }
    
	return false;
}

bool HPS::TurntableOperator::OnTouchUp(TouchState const & in_state)
{
	TouchArray up_touches = in_state.GetActiveEvent().Touches;
    
    for (size_t i = 0, e= up_touches.size(); i < e && tracked_touch_ID != -1; ++i)
    {
        if (up_touches[i].ID == tracked_touch_ID)
            tracked_touch_ID = -1;
    }
	return true;
}

bool HPS::TurntableOperator::OnTouchMove(TouchState const & in_state)
{
	TouchArray touches = in_state.GetActiveEvent().Touches;
	if (tracked_touch_ID == -1 ||  touches.empty())
		return false;

	size_t touch_count = touches.size();
	if (touch_count > 2)
		return false;
    
    HPS::WindowPoint delta;
    delta.x = touches[0].Location.x - start_point.x;
    delta.y = touches[0].Location.y - start_point.y;
    delta.z = touches[0].Location.z - start_point.z;
    
    if (touch_count == 1)
		TurntableCommon(delta, HPS::Vector(0, 0, 1));
    else if (touch_count == 2)
    {
        HPS::Point point1(1, 0, 0), point2(0, 0, 0);
        HPS::WindowKey window = in_state.GetEventSource();
        window.ConvertCoordinate(HPS::Coordinate::Space::InnerWindow, point1, HPS::Coordinate::Space::World, point1);
        window.ConvertCoordinate(HPS::Coordinate::Space::InnerWindow, point2, HPS::Coordinate::Space::World, point2);
        HPS::Vector rotation_axis = point2 - point1;
        rotation_axis.Normalize();
        
        TurntableCommon(delta, rotation_axis);
    }
    
    start_point = touches[0].Location;
    
	Database::GetEventDispatcher().InjectEvent(CameraChangedEvent(GetAttachedView()));
    GetAttachedView().Update();
	return true;
}

void HPS::TurntableOperator::TurntableCommon(HPS::WindowPoint const & delta, HPS::Vector const & rotation_axis)
{
	HPS::SegmentKey view_segment = GetAttachedView().GetSegmentKey();
	HPS::CameraKit camera;
	view_segment.ShowCamera(camera);

	HPS::Point target, position;
	HPS::Vector up;
	camera.ShowTarget(target);
	camera.ShowPosition(position);
	camera.ShowUpVector(up);

	HPS::Point rotation_point = target;
	if (center_point_set)
		rotation_point = center_point;

	HPS::MatrixKit off_axis_matrix, translation_matrix;
	HPS::MatrixKit identity;

	off_axis_matrix.RotateOffAxis(rotation_axis, delta.x * -250);
	translation_matrix.Translate(-rotation_point.x, -rotation_point.y, -rotation_point.z);
	HPS::MatrixKit matrix1 = translation_matrix.Multiply(off_axis_matrix);
	translation_matrix = identity;
	translation_matrix.Translate(rotation_point.x, rotation_point.y, rotation_point.z);
	HPS::MatrixKit matrix2 = matrix1.Multiply(translation_matrix);

	position = matrix2.Transform(position);
	up = off_axis_matrix.Transform(up);
	camera.SetPosition(position).SetUpVector(up);

	if (center_point_set)
	{
		target = matrix2.Transform(target);
		camera.SetTarget(target);
	}

	view_segment.SetCamera(camera);
}

void HPS::TurntableOperator::CalculateCenterPoint(HPS::WindowKey const & window, HPS::Point const & location)
{
    try
    {
        HPS::SelectionResults selection_results;
        size_t number_selected = window.GetSelectionControl().SelectByPoint(location, selection_options, selection_results);
        if (number_selected > 0)
        {
            HPS::SelectionResultsIterator it = selection_results.GetIterator();
            if (it.IsValid())
            {
                HPS::SelectionItem selection_item = it.GetItem();
                selection_item.ShowSelectionPosition(center_point);
                center_point_set = true;
            }
        }
    }
    catch(HPS::InvalidObjectException const &)
    {
        //do nothing
    }
}