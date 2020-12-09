// Copyright (c) Tech Soft 3D, Inc.
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "sprk_ops.h"

HPS::ZoomOperator::ZoomOperator(MouseButtons in_mouse_trigger, ModifierKeys in_modifier_trigger)
	: Operator(in_mouse_trigger, in_modifier_trigger), tracked_touch_ID(-1)
{

}

bool HPS::ZoomOperator::OnMouseDown(MouseState const & in_state)
{
	if(IsMouseTriggered(in_state))
	{
		operator_active = true;
		UpdateZoomLimit();
		HPS::KeyPath key_path = in_state.GetEventPath();
		last_zoom = in_state.GetLocation();
		ZoomStart();
		tracked_touch_ID = -1;
		Database::GetEventDispatcher().InjectEvent(CameraChangedEvent(GetAttachedView()));
		GetAttachedView().Update();
		return true;
	}
	return false;
}

bool HPS::ZoomOperator::OnMouseUp(MouseState const & in_state)
{
	if(operator_active && !IsMouseTriggered(in_state))
		operator_active = false;
	return false;
}

bool HPS::ZoomOperator::OnMouseMove(MouseState const & in_state)
{
	if(operator_active && IsMouseTriggered(in_state))
	{
		HPS::WindowPoint curr = in_state.GetLocation();
		HPS::Vector delta = curr - last_zoom;
		
		UpdateZoom(delta.y);
		Database::GetEventDispatcher().InjectEvent(CameraChangedEvent(GetAttachedView()));
		GetAttachedView().Update();
		return true;
	}
	return false;
}

bool HPS::ZoomOperator::OnTouchDown(TouchState const & in_state)
{
	TouchArray touches = in_state.GetTouches();

	if (in_state.GetTouchCount() == 2)
	{
		UpdateZoomLimit();
		HPS::Point center = HPS::Midpoint(touches[1].Location, touches[0].Location);
		HPS::KeyPath key_path = in_state.GetEventPath();
		key_path.ConvertCoordinate(HPS::Coordinate::Space::Window, center, HPS::Coordinate::Space::World, start_point);

		start_point = center;
		last_zoom = start_point;

		Vector relative = touches[1].Location - touches[0].Location;
		last_zoom.x = static_cast<float>(relative.Length());
		ZoomStart();

		last_relative = relative;
		last_relative.Normalize();
		Database::GetEventDispatcher().InjectEvent(CameraChangedEvent(GetAttachedView()));
		GetAttachedView().Update();
        if (touches[0].TapCount == 1)
            return true;
        else
            return false;
	}
	return false;
}

bool HPS::ZoomOperator::OnTouchMove(TouchState const & in_state)
{
	TouchArray touches = in_state.GetTouches();
	
	if (in_state.GetTouchCount() == 2)
	{
		HPS::Point center = HPS::Midpoint(touches[0].Location, touches[1].Location);
		HPS::WorldPoint curr;
		HPS::KeyPath key_path(in_state.GetEventPath());
		key_path.ConvertCoordinate(HPS::Coordinate::Space::Window, center, HPS::Coordinate::Space::World, curr);

		Vector relative = touches[1].Location - touches[0].Location;
		float dist = static_cast<float>(relative.Length());
		float zoom_factor = dist - last_zoom.x;
		UpdateZoom(zoom_factor);

		relative /= dist;
		Database::GetEventDispatcher().InjectEvent(CameraChangedEvent(GetAttachedView()));
		GetAttachedView().Update();
		return true;
	}
	return false;
}


void HPS::ZoomOperator::ZoomStart()
{
	HPS::Point pos;

	HPS::CameraKit cam;
	GetAttachedView().GetSegmentKey().ShowCamera(cam);
	cam.ShowPosition(pos);
	cam.ShowTarget(zoom_start_camera_target);
	cam.ShowHeight(zoom_start_field_height);
	cam.ShowWidth(zoom_start_field_width);

	zoom_start_camera_look = zoom_start_camera_target - pos;
	focal_length = static_cast<float>(zoom_start_camera_look.Length());
	zoom_start_camera_look.Normalize();
}

void HPS::ZoomOperator::UpdateZoom(float zoom_factor)
{
	HPS::Point target;
	float width, height;  
	HPS::Camera::Projection projection;

	HPS::CameraKit cam;
	GetAttachedView().GetSegmentKey().ShowCamera(cam);
	cam.ShowTarget(target);
	cam.ShowField(width, height);
	cam.ShowProjection(projection);

	if (width > zoom_limit || height > zoom_limit || zoom_factor < 0)
	{
		if (zoom_factor < 0.99f)
		{
			HPS::Point newPos = zoom_start_camera_target - zoom_start_camera_look * (focal_length * (1.0f - zoom_factor));

			if (!newPos.Equals(target))
			{
				// Note that we call SetPosition directly on the control. 3dgs recalculates the field of view after setting camera position (which this code is relying on), 
				// but HPS will stomp on the fov and replace it with the old value from the kit if we set the whole kit.
				GetAttachedView().GetSegmentKey().GetCameraControl().SetPosition(newPos);
			}

			if (projection == HPS::Camera::Projection::Orthographic)
			{
				width = zoom_start_field_width * (1.0f - zoom_factor);
				height = zoom_start_field_height * (1.0f - zoom_factor);
				GetAttachedView().GetSegmentKey().GetCameraControl().SetField(width, height);
			}
		}
	}
}

void HPS::ZoomOperator::UpdateZoomLimit()
{
	HPS::SimpleSphere sphere;
	HPS::SimpleCuboid bounds;
	HPS::BoundingKit bkit;
	GetAttachedView().GetAttachedModel().GetSegmentKey().ShowBounding(bkit);
	bkit.ShowVolume(sphere,bounds);

	HPS::Vector delta = bounds.max - bounds.min;
	zoom_limit = static_cast<float>(delta.Length()) * 0.002f;
}


