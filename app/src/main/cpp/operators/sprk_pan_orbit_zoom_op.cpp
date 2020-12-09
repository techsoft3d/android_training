// Copyright (c) Tech Soft 3D, Inc.
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "sprk_ops.h"

HPS::PanOrbitZoomOperator::PanOrbitZoomOperator(MouseButtons in_mouse_trigger, ModifierKeys in_modifier_trigger)
	: Operator(in_mouse_trigger, in_modifier_trigger),
	operator_active(false),
	zoom_limit(0),
	focal_length(1),
	last_zoom(HPS::WindowPoint(0, 0, 0)),
	last_relative(HPS::Vector(0, 0, 1)),
	zoom_start_field_width(2),
	zoom_start_field_height(2),
	zoom_start_camera_look(HPS::Vector(0, 0, 1)),
	current_touches(0)
{
}


bool HPS::PanOrbitZoomOperator::OnMouseDown(MouseState const  & in_state)
{
	if(IsMouseTriggered(in_state))
	{
		operator_active = true;
		if(in_state.GetModifierKeys().Control())
			UpdateZoomLimit();

		HPS::KeyPath key_path = in_state.GetEventPath();

		key_path.ConvertCoordinate(HPS::Coordinate::Space::Window, in_state.GetLocation(), HPS::Coordinate::Space::World, start);

		start_point = in_state.GetLocation();
		last_zoom = start_point;
		ZoomStart();

		OperatorUtility::ScreenToSphereMousePoint(start_point, start_sphere_pos);
		return true;
	}
	return false;
}

bool HPS::PanOrbitZoomOperator::OnMouseUp(MouseState const & in_state)
{
	if(operator_active && !IsMouseTriggered(in_state))
		operator_active = false;
	return false;
}

bool HPS::PanOrbitZoomOperator::OnMouseMove(MouseState const  & in_state)
{
	if(operator_active && IsMouseTriggered(in_state))
	{
		//Zoom
		if(in_state.GetModifierKeys().Control())
		{
			HPS::WindowPoint curr = in_state.GetLocation();
			HPS::Vector delta = curr - last_zoom;
			UpdateZoom(delta.y);
		}
		// Pan
		else if(in_state.GetModifierKeys().Shift())
		{
			HPS::WorldPoint curr;
			HPS::KeyPath key_path(in_state.GetEventPath());
			key_path.ConvertCoordinate(HPS::Coordinate::Space::Window, in_state.GetLocation(), HPS::Coordinate::Space::World, curr);
			UpdatePan(curr);
		}
		// Orbit
		else
		{
			float angle1, angle2, angle3;
			float theta;

			new_point = in_state.GetLocation();
			OperatorUtility::ScreenToSphereMousePoint(new_point, new_sphere_pos);

			// get the axis of rotation
			HPS::Vector axis = start_sphere_pos.Cross(new_sphere_pos);

			// this is for screen mouse based movement
			HPS::Vector vtmp = new_point - start_point;
			float dist = static_cast<float>(vtmp.Length()) * 90.0f;

			if ((axis.x != 0.0f || axis.y != 0.0f || axis.z != 0.0f))
			{
				HPS::Drawing::Handedness worldHandedness;
				if (GetAttachedView().GetSegmentKey().GetDrawingAttributeControl().ShowWorldHandedness(worldHandedness)
					&& worldHandedness == HPS::Drawing::Handedness::Right)
				{
					axis.y = -axis.y;
					axis.z = -axis.z;
				}

				axis.Normalize();

				theta = OperatorUtility::CalculateAngleOrbitOnPlane(axis, OperatorUtility::Plane_YZ);
				angle2 = axis.x < 0 ? -theta * dist : theta * dist;

				theta = OperatorUtility::CalculateAngleOrbitOnPlane(axis, OperatorUtility::Plane_XZ);
				angle1 = axis.y < 0 ? theta * dist : -theta * dist;

				theta = OperatorUtility::CalculateAngleOrbitOnPlane(axis, OperatorUtility::Plane_XY);
				angle3 = axis.z < 0 ? theta * dist : -theta * dist;

				View view = GetAttachedView();
				HPS::CameraKit cam;
				view.GetSegmentKey().ShowCamera(cam);
				cam.Orbit(angle1, angle2);
				cam.Roll(angle3);
				view.GetSegmentKey().SetCamera(cam);
			}

			start_sphere_pos = new_sphere_pos;
			start_point = new_point;
		}
		GetAttachedView().Update();
		Database::GetEventDispatcher().InjectEvent(CameraChangedEvent(GetAttachedView()));
		return true;
	}
	return false;
}

bool HPS::PanOrbitZoomOperator::OnTouchDown(TouchState const  & in_state)
{
	TouchArray touches = in_state.GetTouches();
	if (!touches.empty() && touches[0].TapCount != 1)
		return false;

	current_touches = in_state.GetTouchCount();

	if (in_state.GetTouchCount() == 1)
	{
		if(in_state.GetModifierKeys().Control())
			UpdateZoomLimit();

		HPS::KeyPath key_path = in_state.GetEventPath();
		key_path.ConvertCoordinate(HPS::Coordinate::Space::Window, touches[0].Location, HPS::Coordinate::Space::World, start);

		start_point = touches[0].Location;
		last_zoom = start_point;
		ZoomStart();

		OperatorUtility::ScreenToSphereMousePoint(start_point, start_sphere_pos);
		return true;
	} 
	else if (in_state.GetTouchCount() == 2)
	{
		UpdateZoomLimit();
		HPS::Point center = HPS::Midpoint(touches[1].Location, touches[0].Location);
		HPS::KeyPath key_path = in_state.GetEventPath();
		key_path.ConvertCoordinate(HPS::Coordinate::Space::Window, center, HPS::Coordinate::Space::World, start);

		start_point = center;

		Vector relative = touches[1].Location - touches[0].Location;
		last_zoom.x = static_cast<float>(relative.Length());
		ZoomStart();

		last_relative = relative;
		last_relative.Normalize();
		return true;
	}
	else
		return false;
}

bool HPS::PanOrbitZoomOperator::OnTouchUp(TouchState const  & in_state)
{
	// If we went from 2 locators down to 1, need to reset as though we just put one down because we're going to start orbiting on the next move.
	if(in_state.GetTouchCount() == 1)
	{
		current_touches = 1;
		HPS::KeyPath key_path = in_state.GetEventPath();
		start_point = in_state.GetTouches()[0].Location;
		key_path.ConvertCoordinate(HPS::Coordinate::Space::Window, start_point, HPS::Coordinate::Space::World, start);
		OperatorUtility::ScreenToSphereMousePoint(start_point, start_sphere_pos);
		return true;
	}
	else
	{
		current_touches = 0;
		return false;
	}
}

bool HPS::PanOrbitZoomOperator::OnTouchMove(TouchState const  & in_state)
{
	if (current_touches == 0)
		return false;

    TouchArray touches = in_state.GetTouches();

	if (in_state.GetTouchCount() == 1)
	{
		if(in_state.GetModifierKeys().Control())
		{
			HPS::WindowPoint curr = touches[0].Location;
			HPS::Vector delta = curr - last_zoom;
			UpdateZoom(delta.y);
		}
		// Pan
		else if(in_state.GetModifierKeys().Shift())
		{   
			HPS::WorldPoint curr;
			HPS::KeyPath key_path(in_state.GetEventPath());
			key_path.ConvertCoordinate(HPS::Coordinate::Space::Window, touches[0].Location, HPS::Coordinate::Space::World, curr);
			UpdatePan(curr);
		}
		// Orbit
		else
		{
			float angle1, angle2, angle3;
			float theta;

			new_point = touches[0].Location;
			OperatorUtility::ScreenToSphereMousePoint(new_point, new_sphere_pos);

			// get the axis of rotation
			HPS::Vector axis = start_sphere_pos.Cross(new_sphere_pos);

			// this is for screen mouse based movement
			HPS::Vector vtmp = new_point - start_point;
			float dist = static_cast<float>(vtmp.Length()) * 90.0f;

			if ((axis.x != 0.0f || axis.y != 0.0f || axis.z != 0.0f))
			{
				HPS::Drawing::Handedness worldHandedness;
				if (GetAttachedView().GetSegmentKey().GetDrawingAttributeControl().ShowWorldHandedness(worldHandedness)
					&& worldHandedness == HPS::Drawing::Handedness::Right)
				{
					axis.y = -axis.y;
					axis.z = -axis.z;
				}

				axis.Normalize();

				theta = OperatorUtility::CalculateAngleOrbitOnPlane(axis, OperatorUtility::Plane_YZ);
				angle2 = axis.x < 0 ? -theta * dist : theta * dist;

				theta = OperatorUtility::CalculateAngleOrbitOnPlane(axis, OperatorUtility::Plane_XZ);
				angle1 = axis.y < 0 ? theta * dist : -theta * dist;

				theta = OperatorUtility::CalculateAngleOrbitOnPlane(axis, OperatorUtility::Plane_XY);
				angle3 = axis.z < 0 ? theta * dist : -theta * dist;

				View view = GetAttachedView();
				HPS::CameraKit cam;
				view.GetSegmentKey().ShowCamera(cam);
				cam.Orbit(angle1, angle2);
				cam.Roll(angle3);
				view.GetSegmentKey().SetCamera(cam);
			}

			start_sphere_pos = new_sphere_pos;
			start_point = new_point;
		}
	}
	else if (in_state.GetTouchCount() == 2)
	{
		HPS::Point center = HPS::Midpoint(touches[0].Location, touches[1].Location);
		HPS::WorldPoint curr;
		HPS::KeyPath key_path(in_state.GetEventPath());
		key_path.ConvertCoordinate(HPS::Coordinate::Space::Window, center, HPS::Coordinate::Space::World, curr);
		UpdatePan(curr);

		Vector relative = touches[1].Location - touches[0].Location;
		float dist = static_cast<float>(relative.Length());
		float zoom_factor = dist - last_zoom.x;
		UpdateZoom(zoom_factor);
	}
	GetAttachedView().Update();
	Database::GetEventDispatcher().InjectEvent(CameraChangedEvent(GetAttachedView()));
	return true;
}

void HPS::PanOrbitZoomOperator::ZoomStart()
{
	HPS::Point pos;
	HPS::Point tar;
    
	HPS::CameraKit cam;
	GetAttachedView().GetSegmentKey().ShowCamera(cam);
    cam.ShowPosition(pos);
    cam.ShowTarget(tar);
    cam.ShowHeight(zoom_start_field_height);
    cam.ShowWidth(zoom_start_field_width);
    
    zoom_start_camera_look = tar - pos;
    focal_length = static_cast<float>(zoom_start_camera_look.Length());
    zoom_start_camera_look.Normalize();
}

void::HPS::PanOrbitZoomOperator::UpdateZoom(float zoom_factor)
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
            HPS::Point newPos = target - zoom_start_camera_look * (focal_length * (1.0f - zoom_factor));
            
            if (!newPos.Equals(target))
            {            
                // Note that we call SetPosition directly on the control. 3dgs recalculates the field of view after setting camera position (which this code is relying on), 
                // but HPS will stomp on the fov and replace it with the old value from the kit if we set the whole kit.
                GetAttachedView().GetSegmentKey().GetCameraControl().SetPosition(newPos);
            }            
            
            if (projection == HPS::Camera::Projection::Orthographic)
            {
                if (zoom_factor < 0.99f)
                {                
                    width = zoom_start_field_width * (1.0f - zoom_factor);             
                    height = zoom_start_field_height * (1.0f - zoom_factor);
                    GetAttachedView().GetSegmentKey().GetCameraControl().SetField(width, height);
                }                
            }
        }        
    }
}

void HPS::PanOrbitZoomOperator::UpdatePan(HPS::WorldPoint const & newLocation)
{
    HPS::Vector delta = newLocation - start;
    
    HPS::Point pt;
	HPS::CameraKit cam;
	GetAttachedView().GetSegmentKey().ShowCamera(cam);
    cam.ShowPosition(pt);
    cam.SetPosition(pt - delta);
    
    cam.ShowTarget(pt);
    cam.SetTarget(pt - delta);
    GetAttachedView().GetSegmentKey().SetCamera(cam);
}

void HPS::PanOrbitZoomOperator::UpdateZoomLimit()
{
	HPS::SimpleSphere sphere;
	HPS::SimpleCuboid bounds;
	HPS::BoundingKit bkit;
    
    auto model = GetAttachedView().GetAttachedModel();
    
    bool bounding_good = false;
    
    if (model.Type() != HPS::Type::None)
        bounding_good = model.GetSegmentKey().ShowBounding(bkit);
    else
        bounding_good = GetAttachedView().GetSegmentKey().ShowBounding(bkit);
    
    if (bounding_good)
    {
        bkit.ShowVolume(sphere,bounds);
        HPS::Vector delta = bounds.max - bounds.min;
        zoom_limit = static_cast<float>(delta.Length()) * 0.002f;
    }
}


