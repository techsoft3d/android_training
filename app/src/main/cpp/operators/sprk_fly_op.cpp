// Copyright (c) Tech Soft 3D, Inc.
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "sprk_ops.h"

using namespace HPS;

HPS::FlyOperator::FlyOperator(MouseButtons in_mouse_trigger, ModifierKeys in_modifier_trigger) 
	: Operator(in_mouse_trigger, in_modifier_trigger)
    , movement_flags(0)
	, left_side_touch(-1), right_side_touch(-1), second_right_touch(-1)
	, joystick_dead_zone(0.07f)
	, keyboard_sensitivity(0), old_keyboard_sensitivity(0), mouse_sensitivity(6.0), scene_extents(0)
	, two_right_fingers_down(false), invert_x_axis(false), invert_y_axis(false), world_handedness(Drawing::Handedness::Right), shift_pressed(false), ctrl_pressed(false)
{
}

void HPS::FlyOperator::OnViewAttached(HPS::View const & in_attached_view)
{
	HPS::View view = in_attached_view;
	HPS::SegmentKey view_segment = view.GetSegmentKey();
	HPS::Camera::Projection camera_projection;
	view_segment.GetCameraControl().ShowProjection(camera_projection);
	if (camera_projection != Camera::Projection::Perspective)
	{
		view_segment.GetCameraControl().SetProjection(Camera::Projection::Perspective);
		view.Update();
	}
	ComputeReasonableStepLength();
	view_segment.GetDrawingAttributeControl().ShowWorldHandedness(world_handedness);
}

void HPS::FlyOperator::OnViewDetached(HPS::View const &)
{
    if (left_joystick_segment.Type() != HPS::Type::None)
        left_joystick_segment.Delete();
    if (right_joystick_segment.Type() != HPS::Type::None)
        right_joystick_segment.Delete();
}

void HPS::FlyOperator::OnModelAttached()
{
	ComputeReasonableStepLength();
}

bool HPS::FlyOperator::OnKeyDown(KeyboardState const & in_state)
{
	UpdateKeyboardState(in_state);
	return true;
}

bool HPS::FlyOperator::OnKeyUp(KeyboardState const & in_state) //-V524
{
	UpdateKeyboardState(in_state);
	return true;
}

bool HPS::FlyOperator::OnMouseDown(MouseState const & in_state)
{
	if (IsMouseTriggered(in_state))
	{
		start_point = in_state.GetLocation();
		return true;
	}

	return false;
}

bool HPS::FlyOperator::OnMouseUp(MouseState const & in_state)
{
	if (!IsMouseTriggered(in_state))
	{
		movement_flags &= ~MovementFlags::rotating;
		return true;
	}

	return false;
}

bool HPS::FlyOperator::OnMouseMove(HPS::MouseState const & in_state)
{
	if (IsMouseTriggered(in_state))
	{
		movement_flags |= MovementFlags::rotating;
		event_path = in_state.GetEventPath();
		rotation_location = in_state.GetLocation();
		return true;
	}

	return false;
}

bool HPS::FlyOperator::OnMouseWheel(MouseState const & in_state)
{
	const MouseEvent event = in_state.GetActiveEvent();
	if (in_state.GetModifierKeys().Shift())
	{
		HPS::CameraKit camera;
		GetAttachedView().GetSegmentKey().ShowCamera(camera);
		float width, height;
		camera.ShowField(width, height);
		
		if (event.WheelDelta > 0)
		{
			width += width * 0.05f;
			height += height * 0.05f;
		}
		else
		{
			width -= width * 0.05f;
			height -= height * 0.05f;
		}

		camera.SetField(width, height);
		GetAttachedView().GetSegmentKey().SetCamera(camera);
		Database::GetEventDispatcher().InjectEvent(CameraChangedEvent(GetAttachedView()));
		GetAttachedView().Update();
	}
	else
	{
		if (event.WheelDelta > 0)
			keyboard_sensitivity *= 2;
		else
			keyboard_sensitivity /= 2;
	}

	return true;
}

bool HPS::FlyOperator::OnTouchDown(const HPS::TouchState &in_state)
{
    HPS::TouchArray touches = in_state.GetActiveEvent().Touches;
    
    if (touches[0].TapCount != 1)
        return false;
    
    
    TouchPosition current_position = TouchPosition::None;
    
    if (touches.size() == 2 &&
        right_side_touch == -1 &&
        two_right_fingers_down == false)
    {
        if (touches[0].Location.x >= 0 && touches[1].Location.x >= 0)
        {
            two_right_fingers_down = true;
            if (right_joystick_segment.Type() != HPS::Type::None)
                right_joystick_segment.Delete();
            right_joystick_segment = GetAttachedView().GetSegmentKey().Subsegment();
            current_position = TouchPosition::Right;
            
            right_side_touch = touches[0].ID;
            right_start_point = touches[0].Location;
            current_right_touch_position = right_start_point;
            
            second_right_touch = touches[1].ID;
            second_right_start_point = touches[1].Location;
            current_second_right_touch_position = second_right_start_point;
            movement_flags &= ~MovementFlags::rotate_with_touch;
            
            start_mid_point = current_right_touch_position;
            start_mid_point.x += (second_right_start_point.x - current_right_touch_position.x) / 2;
            start_mid_point.y += (second_right_start_point.y - current_right_touch_position.y) / 2;
            start_mid_point.z += (second_right_start_point.z - current_right_touch_position.z) / 2;
        }
    }
    else
    {
        if (touches[0].Location.x < 0 && left_side_touch == -1)
        {
            if (left_joystick_segment.Type() != HPS::Type::None)
                left_joystick_segment.Delete();
            left_joystick_segment = GetAttachedView().GetSegmentKey().Subsegment();
            current_position = TouchPosition::Left;
            left_side_touch = touches[0].ID;
            left_start_point = touches[0].Location;
        }
        else if (touches[0].Location.x >= 0 && right_side_touch == -1)
        {
            if (right_joystick_segment.Type() != HPS::Type::None)
                right_joystick_segment.Delete();
            right_joystick_segment = GetAttachedView().GetSegmentKey().Subsegment();
            current_position = TouchPosition::Right;
            right_side_touch = touches[0].ID;
            right_start_point = touches[0].Location;
            current_right_touch_position = right_start_point;
        }
        else if (touches[0].Location.x >= 0
                 && right_side_touch != -1
                 && !two_right_fingers_down)
        {
            if (right_joystick_segment.Type() != HPS::Type::None)
                right_joystick_segment.Delete();
            right_joystick_segment = GetAttachedView().GetSegmentKey().Subsegment();
            two_right_fingers_down = true;
            second_right_touch = touches[0].ID;
            second_right_start_point = touches[0].Location;
            current_position = TouchPosition::Right;
            current_second_right_touch_position = second_right_start_point;
            movement_flags &= ~MovementFlags::rotate_with_touch;
        
            start_mid_point = current_right_touch_position;
            start_mid_point.x += (second_right_start_point.x - current_right_touch_position.x) / 2;
            start_mid_point.y += (second_right_start_point.y - current_right_touch_position.y) / 2;
            start_mid_point.z += (second_right_start_point.z - current_right_touch_position.z) / 2;
        }
        else if (touches[0].Location.x >= 0
                 && right_side_touch != -1
                 && two_right_fingers_down)
        {
            two_right_fingers_down = false;
            second_right_touch = -1;
        }
    }


    if (right_joystick_sensitivity != old_right_joystick_sensitivity)
        right_joystick_sensitivity = old_right_joystick_sensitivity;
    if (left_joystick_sensitivity != old_left_joystick_sensitivity)
        left_joystick_sensitivity = old_left_joystick_sensitivity;
    CreateJoystick(in_state, current_position);
        
    return true;
}

bool HPS::FlyOperator::OnTouchMove(const HPS::TouchState &in_state)
{
    TouchArray touches = in_state.GetTouches();
    
    HPS::Touch left_touch, right_touch;
    bool strafe = false;
    bool rotate = false;
    
    float left_x_delta = 0, left_y_delta = 0, left_delta = 0;
    float right_x_delta = 0, right_y_delta = 0, right_delta = 0;
    
    for (size_t i = 0; i < in_state.GetTouchCount(); ++i)
    {
        if (touches[i].ID == left_side_touch)
        {
            left_touch = touches[i];
            strafe = true;
            left_x_delta = touches[i].Location.x - left_start_point.x;
            left_y_delta = touches[i].Location.y - left_start_point.y;
            left_delta = fabs(fabs(left_x_delta) > fabs(left_y_delta) ? left_x_delta : left_y_delta);
            float scale;
            scale = pow(left_delta / (3 * GetJoystickDeadZone()), 2);
            left_joystick_sensitivity = old_left_joystick_sensitivity * scale;
        }
        else if(touches[i].ID == right_side_touch)
        {
            right_touch = touches[i];
            rotate = true;
            right_x_delta = touches[i].Location.x - right_start_point.x;
            right_y_delta = touches[i].Location.y - right_start_point.y;
            right_delta = fabs(fabs(right_x_delta) > fabs(right_y_delta) ? right_x_delta : right_y_delta);
            float scale;
            scale = pow(right_delta / (3 * GetJoystickDeadZone()), 2);
            right_joystick_sensitivity = old_right_joystick_sensitivity * scale;
            current_right_touch_position = touches[i].Location;
        }
        else if(touches[i].ID == second_right_touch)
            current_second_right_touch_position = touches[i].Location;
    }
    
    if (strafe)
	{
        if (fabs(left_x_delta) > GetJoystickDeadZone() || fabs(left_y_delta) > GetJoystickDeadZone())
        {
			movement_flags |= MovementFlags::move_with_touch;
            event_path = in_state.GetEventPath();
            moving_position = left_touch.Location;
        }
        else
			movement_flags &= ~MovementFlags::move_with_touch;
    }
    if (rotate && ! two_right_fingers_down)
    {
        if (fabs(right_x_delta) > GetJoystickDeadZone() || fabs(right_y_delta) > GetJoystickDeadZone())
        {
			movement_flags |= MovementFlags::rotate_with_touch;
            rotation_location = right_touch.Location;
            event_path = in_state.GetEventPath();
        }
    }
   else if (two_right_fingers_down)
    {
        HPS::Point mid_point = current_right_touch_position;
        mid_point.x += (current_second_right_touch_position.x - current_right_touch_position.x) / 2;
        mid_point.y += (current_second_right_touch_position.y - current_right_touch_position.y) / 2;
        mid_point.z += (current_second_right_touch_position.z - current_right_touch_position.z) / 2;
        
        if (mid_point.y - start_mid_point.y > GetJoystickDeadZone())
        {
			movement_flags |= MovementFlags::moving_up;
			movement_flags &= ~MovementFlags::moving_down;
        }
        else if (mid_point.y - start_mid_point.y < -GetJoystickDeadZone())
        {
			movement_flags |= MovementFlags::moving_down;
			movement_flags &= ~MovementFlags::moving_up;
        }
        else
			movement_flags &= ~(MovementFlags::moving_up | MovementFlags::moving_down);

		if (mid_point.x - start_mid_point.x > GetJoystickDeadZone())
        {
			movement_flags |= MovementFlags::roll_right;
			movement_flags &= ~MovementFlags::roll_left;
        }
        else if (mid_point.x - start_mid_point.x < -GetJoystickDeadZone())
        {
			movement_flags |= MovementFlags::roll_left;
			movement_flags &= ~MovementFlags::roll_right;
        }
        else
			movement_flags &= ~(MovementFlags::roll_left | MovementFlags::roll_right);
    }
    else
        return false;
    
	return true;
}

bool HPS::FlyOperator::OnTouchUp(const HPS::TouchState &in_state)
{
    TouchArray up_touches = in_state.GetActiveEvent().Touches;
    
    for (size_t i = 0, e= up_touches.size(); i < e; ++i)
    {
        if (left_side_touch != -1 && left_side_touch == up_touches[i].ID)
        {
            left_side_touch = -1;
            left_joystick_segment.Delete();
			movement_flags &= ~MovementFlags::move_with_touch;
        }
        if (right_side_touch != -1 && right_side_touch == up_touches[i].ID)
        {
            right_side_touch = -1;
            right_joystick_segment.Delete();
            two_right_fingers_down = false;
			movement_flags &= ~(MovementFlags::moving_up | MovementFlags::moving_down
								|MovementFlags::roll_right | MovementFlags::roll_left
								| MovementFlags::rotate_with_touch);
            
            //if we have another right touch, reposition the joystick under it
            //and assume its values
            if (second_right_touch != -1)
            {
                right_joystick_segment = GetAttachedView().GetSegmentKey().Subsegment();
                right_side_touch = second_right_touch;
                right_start_point = current_second_right_touch_position;
                current_right_touch_position = current_second_right_touch_position;
                second_right_touch = -1;
                CreateJoystick(in_state, TouchPosition::Right);
            }
        }
        if (second_right_touch != -1 && second_right_touch == up_touches[i].ID)
        {
            two_right_fingers_down = false;
            second_right_touch = -1;
            right_joystick_segment.Delete();
			movement_flags &= ~(MovementFlags::moving_up | MovementFlags::moving_down
				|MovementFlags::roll_right | MovementFlags::roll_left
				| MovementFlags::rotate_with_touch);

            //if we have a right touch, reposition the joystick under it
            if (right_side_touch != -1)
            {
                right_joystick_segment = GetAttachedView().GetSegmentKey().Subsegment();
                right_start_point = current_right_touch_position;
                CreateJoystick(in_state, TouchPosition::Right);
            }
        }
    }
    
    GetAttachedView().Update();
    
    right_joystick_sensitivity = old_right_joystick_sensitivity;
    left_joystick_sensitivity = old_left_joystick_sensitivity;
    
	return true;
}

bool HPS::FlyOperator::OnTimerTick(HPS::TimerTickEvent const & in_event)
{
	static HPS::Time last_tick_time = -1;
	HPS::Time current_time = in_event.GetTimeStamp();
	bool restore_length = false;
	float scaling_factor = 1;
	HPS::Time delta = 0;
	if (last_tick_time == -1)
		last_tick_time = current_time;
	else
	{
		delta = current_time - last_tick_time;
		last_tick_time = current_time;

		float tick_period = 14.0;
		scaling_factor = (float)(delta / tick_period);

		if (Float::Equals(delta, 0))
			return false;

		restore_length = true;

		keyboard_sensitivity *= scaling_factor;
		left_joystick_sensitivity *= scaling_factor;
		right_joystick_sensitivity *= scaling_factor;
	}

	View view = GetAttachedView();
	SegmentKey view_key = view.GetSegmentKey();
	HPS::CameraKit camera;
	view_key.ShowCamera(camera);
	HPS::Point camera_position, camera_target;
    HPS::Vector camera_up;
	camera.ShowPosition(camera_position);
    camera.ShowTarget(camera_target);
    camera.ShowUpVector(camera_up);
    
	if (movement_flags & MovementFlags::rotating)
	{
		if (!RotateScene(camera_position, camera_target))
			movement_flags &= ~MovementFlags::rotating;
	}
    if (movement_flags & MovementFlags::rotate_with_touch)
	{
		if (!RotateWithTouch(camera_position, camera_target))
			movement_flags &= ~MovementFlags::rotate_with_touch;
	}
	if (movement_flags & MovementFlags::moving_forward)
		MoveForward(camera_position, camera_target, HPS::Vector(camera_target - camera_position).Normalize());
	if (movement_flags & MovementFlags::moving_back)
		MoveBack(camera_position, camera_target, HPS::Vector(camera_target - camera_position).Normalize());
	if (movement_flags & MovementFlags::moving_left)
	{
		if (invert_x_axis)
			MoveRight(camera_position, camera_target, camera_up);
		else
			MoveLeft(camera_position, camera_target, camera_up);
	}
	if (movement_flags & MovementFlags::moving_right)
	{
		if (invert_x_axis)
			MoveLeft(camera_position, camera_target, camera_up);
		else
			MoveRight(camera_position, camera_target, camera_up);
	}
	if (movement_flags & MovementFlags::moving_up)
	{
		if (invert_y_axis)
			MoveDown(camera_position, camera_target, camera_up);
		else
			MoveUp(camera_position, camera_target, camera_up);
	}
	if (movement_flags & MovementFlags::moving_down)
	{
		if (invert_y_axis)
			MoveUp(camera_position, camera_target, camera_up);
		else
			MoveDown(camera_position, camera_target, camera_up);
	}
    if (movement_flags & MovementFlags::move_with_touch)
	{
        if (!MoveWithTouch(camera_position, camera_target, camera_up, HPS::Vector(camera_target - camera_position).Normalize()))
			movement_flags &= ~MovementFlags::move_with_touch;
	}
    
    camera.SetPosition(camera_position).SetTarget(camera_target).SetUpVector(camera_up);
	view_key.SetCamera(camera);

	if (movement_flags & MovementFlags::roll_left)
		view_key.GetCameraControl().Roll(-0.5f);
	if (movement_flags & MovementFlags::roll_right)
		view_key.GetCameraControl().Roll(0.5f);

	if (movement_flags)
	{
		Database::GetEventDispatcher().InjectEvent(CameraChangedEvent(view));
		view.Update();
	}

	if (restore_length)
	{
		keyboard_sensitivity /= scaling_factor;
		left_joystick_sensitivity /= scaling_factor;
		right_joystick_sensitivity /= scaling_factor;
	}
	return false;
}

void HPS::FlyOperator::UpdateKeyboardState(KeyboardState const & in_state)
{
	KeyboardCodeArray keys = in_state.GetActiveEvent().KeyboardCodes;
	bool key_down = in_state.GetActiveEvent().CurrentAction == KeyboardEvent::Action::KeyDown;

	if (in_state.GetModifierKeys().Shift() && !shift_pressed)
	{
		shift_pressed = true;
		keyboard_sensitivity *= 2;
	}
	else if (!in_state.GetModifierKeys().Shift() && shift_pressed)
	{
		shift_pressed = false;
		keyboard_sensitivity /= 2;
	}

	if (in_state.GetModifierKeys().Control() && !ctrl_pressed)
	{
		ctrl_pressed = true;
		keyboard_sensitivity /= 2;
	}
	else if (!in_state.GetModifierKeys().Control() && ctrl_pressed)
	{
		ctrl_pressed = false;
		keyboard_sensitivity *= 2;
	}


	for (auto key : keys)
	{
		switch (key)
		{
		case KeyboardCode::W:
        case KeyboardCode::w:
		case KeyboardCode::Up:
			if (key_down)
				movement_flags |= MovementFlags::moving_forward;
			else
				movement_flags &= ~MovementFlags::moving_forward;
			break;
		case KeyboardCode::A:
        case KeyboardCode::a:
		case KeyboardCode::Left:
			if (key_down)
				movement_flags |= MovementFlags::moving_left;
			else
				movement_flags &= ~MovementFlags::moving_left;
			break;
		case KeyboardCode::S:
        case KeyboardCode::s:
		case KeyboardCode::Down:
			if (key_down)
				movement_flags |= MovementFlags::moving_back;
			else
				movement_flags &= ~MovementFlags::moving_back;
			break;
		case KeyboardCode::D:
		case KeyboardCode::d:
		case KeyboardCode::Right:
			if (key_down)
				movement_flags |= MovementFlags::moving_right;
			else
				movement_flags &= ~MovementFlags::moving_right;
			break;
		case KeyboardCode::C:
		case KeyboardCode::c:
			if (key_down)
				movement_flags |= MovementFlags::moving_down;
			else
				movement_flags &= ~MovementFlags::moving_down;
			break;
		case KeyboardCode::Q:
		case KeyboardCode::q:
			if (key_down)
				movement_flags |= MovementFlags::roll_left;
			else
				movement_flags &= ~MovementFlags::roll_left;
			break;
		case KeyboardCode::E:
		case KeyboardCode::e:
			if (key_down)
				movement_flags |= MovementFlags::roll_right;
			else
				movement_flags &= ~MovementFlags::roll_right;
			break;
		case KeyboardCode::Space:
			if (key_down)
				movement_flags |= MovementFlags::moving_up;
			else
				movement_flags &= ~MovementFlags::moving_up;
			break;

        default:
            break;
		}
	}
}

float HPS::FlyOperator::CalculateSceneExtents()
{
	SegmentKey bounding_segment;
	Model model = GetAttachedView().GetAttachedModel();
	if (model.Type() == HPS::Type::None)
		bounding_segment = GetAttachedView().GetSegmentKey();
	else
		bounding_segment = model.GetSegmentKey();

	SimpleSphere bounding_sphere;
	SimpleCuboid bounding_cuboid;
	if (!bounding_segment.GetBoundingControl().ShowVolume(bounding_sphere, bounding_cuboid))
	{
		bounding_cuboid.min = HPS::Point(-0.1f, -0.1f, -0.1f);
		bounding_cuboid.max = HPS::Point(0.1f, 0.1f, 0.1f);
	}

	float extents = 0;
	HPS::Point extent_point;

	extent_point.x = bounding_cuboid.max.x - bounding_cuboid.min.x;
	if (extent_point.x > extents)
		extents = extent_point.x;

	extent_point.y = bounding_cuboid.max.y - bounding_cuboid.min.y;
	if (extent_point.y > extents)
		extents = extent_point.y;

	extent_point.z = bounding_cuboid.max.z - bounding_cuboid.min.z;
	if (extent_point.z > extents)
		extents = extent_point.z;

	return extents;
}

void HPS::FlyOperator::ComputeReasonableStepLength()
{
	scene_extents = CalculateSceneExtents();
	SetKeyboardSensitivity(scene_extents * 0.01f);
    
    left_joystick_sensitivity = right_joystick_sensitivity = GetKeyboardSensitivity();
    old_left_joystick_sensitivity = old_right_joystick_sensitivity = old_keyboard_sensitivity = GetKeyboardSensitivity();
}

void HPS::FlyOperator::MoveForward(HPS::Point & position, HPS::Point & target, HPS::Vector const & direction)
{
	position = position + direction * GetKeyboardSensitivity();
	target = target + direction * GetKeyboardSensitivity();

	if (world_handedness == Drawing::Handedness::Left)
	{
		position.x = -position.x;
		target.x = -target.x;
	}
}

void HPS::FlyOperator::MoveBack(HPS::Point & position, HPS::Point & target, HPS::Vector const & direction)
{
	position = position - direction * GetKeyboardSensitivity();
	target = target - direction * GetKeyboardSensitivity();

	if (world_handedness == Drawing::Handedness::Left)
	{
		position.x = -position.x;
		target.x = -target.x;
	}
}

void HPS::FlyOperator::MoveLeft(HPS::Point & position, HPS::Point & target, HPS::Vector & up)
{
	HPS::Vector direction = target - position;
	HPS::Vector normal = direction.Cross(up).Normalize();
	position = position - normal * GetKeyboardSensitivity();
	target = target - normal * GetKeyboardSensitivity();

	if (world_handedness == Drawing::Handedness::Left)
	{
		position.x = -position.x;
		target.x = -target.x;
	}
}

void HPS::FlyOperator::MoveRight(HPS::Point & position, HPS::Point & target, HPS::Vector & up)
{
	HPS::Vector direction = target - position;
	HPS::Vector normal = direction.Cross(up).Normalize();
	position = position + normal * GetKeyboardSensitivity();
	target = target + normal * GetKeyboardSensitivity();

	if (world_handedness == Drawing::Handedness::Left)
	{
		position.x = -position.x;
		target.x = -target.x;
	}
}

bool HPS::FlyOperator::MoveWithTouch(HPS::Point & position, HPS::Point & target, HPS::Vector & up, HPS::Vector const & walking_direction)
{
    HPS::Vector touch_direction(moving_position - left_start_point);
    touch_direction.Normalize();
    
    float x_step = fabs(touch_direction.x);
    float y_step = fabs(touch_direction.y);
    
    HPS::Point new_target, start_point_w;
    if (event_path.ConvertCoordinate(HPS::Coordinate::Space::Window, moving_position, HPS::Coordinate::Space::World, new_target))
	{
		if (event_path.ConvertCoordinate(HPS::Coordinate::Space::Window, left_start_point, HPS::Coordinate::Space::World, start_point_w))
		{
			float y_multiplier = 3;
			if (moving_position.y - left_start_point.y < 0)
				y_multiplier *= -1;
			if (invert_y_axis)
				y_multiplier *= -1;
    
			float x_multiplier = 3;
			if (moving_position.x - left_start_point.x < 0)
				x_multiplier *= -1;
			if (invert_x_axis)
				x_multiplier *= -1;
    
			HPS::Vector direction;
			//move forward/back
			if (fabs(moving_position.y - left_start_point.y) > GetJoystickDeadZone()
				|| fabs(moving_position.x - left_start_point.x) > GetJoystickDeadZone())
			{
				position = position + walking_direction * y_step * left_joystick_sensitivity * y_multiplier;
				target = target + walking_direction * y_step * left_joystick_sensitivity * y_multiplier;
    
				direction = target - position;
				HPS::Vector normal = direction.Cross(up).Normalize();
				position = position + normal * x_step * left_joystick_sensitivity * x_multiplier;
				target = target + normal * x_step * left_joystick_sensitivity * x_multiplier;
			}
    
			if (world_handedness == Drawing::Handedness::Left)
			{
				position.x = -position.x;
				target.x = -target.x;
			}

			return true;
		}
	}
	return false;
}

void HPS::FlyOperator::MoveUp(HPS::Point & position, HPS::Point & target, HPS::Vector & up)
{
    float step = GetKeyboardSensitivity();
    if (two_right_fingers_down)
        step = right_joystick_sensitivity;

	position = position + up * step;
	target = target + up * step;

	if (world_handedness == Drawing::Handedness::Left)
	{
		position.x = -position.x;
		target.x = -target.x;
	}
}

void HPS::FlyOperator::MoveDown(HPS::Point & position, HPS::Point & target, HPS::Vector & up)
{
    float step = GetKeyboardSensitivity();
    if (two_right_fingers_down)
        step = right_joystick_sensitivity;

	position = position - up * step;
	target = target - up * step;

	if (world_handedness == Drawing::Handedness::Left)
	{
		position.x = -position.x;
		target.x = -target.x;
	}
}

bool HPS::FlyOperator::RotateScene(HPS::Point & position, HPS::Point & target)
{
    HPS::Point location_w, start_point_w;
	if (event_path.ConvertCoordinate(HPS::Coordinate::Space::Window, rotation_location, HPS::Coordinate::Space::World, location_w))
	{
		if (event_path.ConvertCoordinate(HPS::Coordinate::Space::Window, start_point, HPS::Coordinate::Space::World, start_point_w))
		{
			HPS::Point delta(location_w.x - start_point_w.x, location_w.y - start_point_w.y, location_w.z - start_point_w.z);
    
			//increase mouse sensitivity
			delta *= GetMouseSensitivity();

			RotateCommon(delta, position, target);
			start_point = rotation_location;

			return true;
		}
	}

	return false;
}

bool HPS::FlyOperator::RotateWithTouch(HPS::Point & position, HPS::Point & target)
{
    HPS::Point location_w, start_point_w;
	if (event_path.ConvertCoordinate(HPS::Coordinate::Space::Window, rotation_location, HPS::Coordinate::Space::World, location_w))
	{
		if (event_path.ConvertCoordinate(HPS::Coordinate::Space::Window, right_start_point, HPS::Coordinate::Space::World, start_point_w))
		{
			HPS::Vector touch_direction(location_w - start_point_w);
			touch_direction.Normalize();
			float rotation_multiplier = 5.0;
			HPS::Point rotation_delta(right_joystick_sensitivity * touch_direction.x * rotation_multiplier,
									  right_joystick_sensitivity * touch_direction.y * rotation_multiplier,
									  right_joystick_sensitivity * touch_direction.z * rotation_multiplier);
    
			RotateCommon(rotation_delta, position, target);

			return true;
		}
	}
	return false;
}

void HPS::FlyOperator::RotateCommon(HPS::Point const & delta, HPS::Point & position, HPS::Point & target)
{
	HPS::Point new_target(target.x + delta.x, target.y + delta.y, target.z + delta.z);

	HPS::Vector translated_position(target - position);
	float radius = translated_position.Magnitude();

	HPS::Vector new_direction(new_target - position);
	float new_radius = new_direction.Magnitude();
	new_direction *= radius / new_radius;
	new_target = position + new_direction;

	target = new_target;
}

void HPS::FlyOperator::CreateJoystick(const HPS::TouchState &in_state, TouchPosition position)
{
    HPS::SegmentKey joystick_segment;
    HPS::WindowPoint starting_point;
    if (position == TouchPosition::Left)
    {
        joystick_segment = left_joystick_segment;
        starting_point = left_start_point;
    }
    else if (position == TouchPosition::Right)
    {
        joystick_segment = right_joystick_segment;
        starting_point = current_right_touch_position;
        
        if (two_right_fingers_down)
        {
            starting_point.x += (second_right_start_point.x - current_right_touch_position.x) / 2;
            starting_point.y += (second_right_start_point.y - current_right_touch_position.y) / 2;
            starting_point.z += (second_right_start_point.z - current_right_touch_position.z) / 2;
        }
    }
    else
        return;
    
    joystick_segment.GetTransformMaskControl().SetCameraEverything(true).SetModellingMatrixEverything(true);
    joystick_segment.GetVisibilityControl().SetMarkers(true).SetGeometry(true).SetFaces(false);
    joystick_segment.GetMaterialMappingControl().SetMarkerColor(HPS::RGBColor(1, 0, 0));
    joystick_segment.GetMarkerAttributeControl().SetSize(0.5f);
    joystick_segment.GetDrawingAttributeControl().SetOverlay(HPS::Drawing::Overlay::Default);
    
    HPS::Point center;
    HPS::KeyArray keys = in_state.GetEventPath();
	HPS::KeyArray complete_keys;
	complete_keys.push_back(joystick_segment);
	complete_keys.insert(complete_keys.end(), keys.begin(), keys.end());
    HPS::KeyPath path(complete_keys);
    path.ConvertCoordinate(HPS::Coordinate::Space::Window, starting_point, HPS::Coordinate::Space::World, center);
    joystick_segment.InsertMarker(center);
    
    HPS::WindowPoint window_coordinate_radius(starting_point.x + GetJoystickDeadZone(), starting_point.y, starting_point.z);
    HPS::Point world_coordinate_radius;
    path.ConvertCoordinate(HPS::Coordinate::Space::Window, window_coordinate_radius, HPS::Coordinate::Space::World, world_coordinate_radius);
    float radius = HPS::Vector(world_coordinate_radius - center).Magnitude();
    joystick_segment.InsertCircle(center, radius, HPS::Vector(0, 0, 1));
    
    GetAttachedView().Update();
}
