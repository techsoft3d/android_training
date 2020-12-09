// Copyright (c) Tech Soft 3D, Inc.
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "sprk_ops.h"

HPS::PanOperator::PanOperator(MouseButtons in_mouse_trigger, ModifierKeys in_modifier_trigger)
	: Operator(in_mouse_trigger, in_modifier_trigger), tracked_touch_ID(-1)
{

}

bool HPS::PanOperator::OnMouseDown(MouseState const & in_state)
{
	if(IsMouseTriggered(in_state))
	{
		operator_active = true;
		HPS::KeyPath key_path = in_state.GetEventPath();
		key_path.ConvertCoordinate(HPS::Coordinate::Space::Window, in_state.GetLocation(), HPS::Coordinate::Space::World, start);
		tracked_touch_ID = -1;
		return true;
	}
	return false;
}

bool HPS::PanOperator::OnMouseUp(MouseState const & in_state)
{
	if(operator_active && !IsMouseTriggered(in_state))
		operator_active = false;
	return false;
}

bool HPS::PanOperator::OnMouseMove(MouseState const & in_state)
{
	if(operator_active && IsMouseTriggered(in_state))
		return PanCommon(in_state.GetLocation(), in_state.GetEventPath());
	return false;
}

bool HPS::PanOperator::OnTouchDown(TouchState const & in_state)
{
	TouchArray touches = in_state.GetActiveEvent().Touches;
	if (touches.empty())
		return false;

	if(tracked_touch_ID == -1)
	{
		tracked_touch_ID = touches[0].ID;
		if (touches[0].TapCount != 1)
			return false;
		HPS::KeyPath key_path = in_state.GetEventPath();
		key_path.ConvertCoordinate(HPS::Coordinate::Space::Window, touches[0].Location, HPS::Coordinate::Space::World, start);
		return true;
	}

	return false;
}

bool HPS::PanOperator::OnTouchUp(TouchState const & in_state)
{
	TouchArray up_touches = in_state.GetActiveEvent().Touches;

	for(size_t i = 0, e = up_touches.size(); i < e && tracked_touch_ID != -1; ++i)
	{
		if(up_touches[i].ID == tracked_touch_ID)
		{
			tracked_touch_ID = -1;
			return true;
		}
	}
	return false;
}

bool HPS::PanOperator::OnTouchMove(TouchState const & in_state)
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
    
	return PanCommon(the_one.Location, in_state.GetEventPath());
}

bool HPS::PanOperator::PanCommon(HPS::WindowPoint const & in_loc, HPS::KeyPath const & in_key_path)
{
	HPS::WorldPoint curr;
	in_key_path.ConvertCoordinate(HPS::Coordinate::Space::Window, in_loc, HPS::Coordinate::Space::World, curr);

	HPS::Vector delta = curr - start;

	HPS::Point pt;
	HPS::CameraKit cam;
	GetAttachedView().GetSegmentKey().ShowCamera(cam);
	cam.ShowPosition(pt);
	cam.SetPosition(pt - delta);

	cam.ShowTarget(pt);
	cam.SetTarget(pt - delta);
	GetAttachedView().GetSegmentKey().SetCamera(cam);
	Database::GetEventDispatcher().InjectEvent(CameraChangedEvent(GetAttachedView()));
	GetAttachedView().Update();
	return true;
}


