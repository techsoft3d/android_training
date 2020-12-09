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

HPS::WalkOperator::WalkOperator(MouseButtons in_mouse_trigger, ModifierKeys in_modifier_trigger) 
	: FlyOperator(in_mouse_trigger, in_modifier_trigger), primary_up_axis(Axis::Y)
{

}

void HPS::WalkOperator::OnViewAttached(HPS::View const & in_attached_view)
{
	FlyOperator::OnViewAttached(in_attached_view);
	height_off_ground = GetKeyboardSensitivity() * 10;	//should be 10% of extents
	CalculateGroundPlane();
	SnapToPlane();
}

void HPS::WalkOperator::OnModelAttached()
{
	FlyOperator::OnModelAttached();
	height_off_ground = GetKeyboardSensitivity() * 10;	//should be 10% of extents
	CalculateGroundPlane();
	SnapToPlane();
}

void HPS::WalkOperator::CalculateGroundPlane()
{
	Model model = GetAttachedView().GetAttachedModel();
	if (model.Type() == HPS::Type::None)
		ground = HPS::Plane(0, 0, 1, 0);
	else
	{
		HPS::SimpleSphere bounding_sphere;
		HPS::SimpleCuboid bounding_cuboid;
		model.GetSegmentKey().GetBoundingControl().ShowVolume(bounding_sphere, bounding_cuboid);

		//create the six planes representing the bounding with extra distance applied to it
		HPS::Plane world_bbx_planes[6];
		world_bbx_planes[0] =  HPS::Plane(-1, 0, 0, bounding_cuboid.min.x);
		world_bbx_planes[1] =  HPS::Plane(0, -1, 0, bounding_cuboid.min.y);
		world_bbx_planes[2] =  HPS::Plane(0, 0, -1, bounding_cuboid.min.z);
		world_bbx_planes[3] =  HPS::Plane(1, 0, 0, -bounding_cuboid.max.x);
		world_bbx_planes[4] =  HPS::Plane(0, 1, 0, -bounding_cuboid.max.y);
		world_bbx_planes[5] =  HPS::Plane(0, 0, 1, -bounding_cuboid.max.z);

		HPS::KeyPath keypath;
		keypath += model.GetSegmentKey();
		keypath += GetAttachedView().GetAttachedModelIncludeLink();
		keypath += GetAttachedView().GetSegmentKey();

		HPS::MatrixKit world_to_view;
		keypath.ComputeTransform(Coordinate::Space::World, Coordinate::Space::Camera, world_to_view);
		HPS::PlaneArray view_bbx_planes;
		view_bbx_planes = world_to_view.Transform(6, world_bbx_planes);

		int plane_index = 0;
		for (int i = 0; i < 6; i++)
		{
			if(view_bbx_planes[i].b <= view_bbx_planes[plane_index].b && view_bbx_planes[i].c > -0.1)
				plane_index = i;
		}

		ground = world_bbx_planes[plane_index];
	}
}

void HPS::WalkOperator::SnapToPlane()
{
	HPS::Vector up;
	HPS::Vector ground_normal(ground.a, ground.b, ground.c);
	ground_normal.Normalize();
	GetAttachedView().GetSegmentKey().GetCameraControl().ShowUpVector(up);
	if (up != -ground_normal)
	{
		up = -ground_normal;
		GetAttachedView().GetSegmentKey().GetCameraControl().SetUpVector(up);
		Database::GetEventDispatcher().InjectEvent(CameraChangedEvent(GetAttachedView()));
		GetAttachedView().Update();
	}

	UpdatePrimaryUpAxis(up);

	HPS::Point position, target;
	GetAttachedView().GetSegmentKey().GetCameraControl().ShowPosition(position);
	GetAttachedView().GetSegmentKey().GetCameraControl().ShowTarget(target);

	walking_direction = HPS::Vector(target - position).Normalize();
}

HPS::WalkOperator::Axis HPS::WalkOperator::UpdatePrimaryUpAxis(HPS::Vector const & v)
{
	primary_up_axis = GetPrimaryAxis(v);
	return primary_up_axis;
}

HPS::WalkOperator::Axis HPS::WalkOperator::GetPrimaryAxis(HPS::Vector const & v)
{
	Axis primary = Axis::Y;
	float const x = fabs(v.x);
	float const y = fabs(v.y);
	float const z = fabs(v.z);
	if (z > y && z > x)
		primary = (v.z < 0) ? Axis::Negative_Z : Axis::Z;
	else if (x > y)
		primary = (v.x < 0) ? Axis::Negative_X  : Axis::X;
	else
		primary = (v.y < 0) ? Axis::Negative_Y : Axis::Y;

	return primary;
}

bool HPS::WalkOperator::OnTimerTick(HPS::TimerTickEvent const & in_event)
{
	static HPS::Time last_tick_time = -1;
	HPS::Time const current_time = in_event.GetTimeStamp();
	bool restore_length = false;
	float scaling_factor = 1;
	if (last_tick_time >= 0)
	{
		HPS::Time delta = current_time - last_tick_time;

		float const tick_period = 14.0f;
		scaling_factor = (float)(delta / tick_period);

		if (Float::Equals(delta, 0))
			return false;

		restore_length = true;

		SetKeyboardSensitivity(GetKeyboardSensitivity() * scaling_factor);
		SetLeftJoystickSensitivity(GetLeftJoystickSensitivity() * scaling_factor);
		SetRightJoystickSensitivity(GetRightJoystickSensitivity() * scaling_factor);
	}

	HPS::CameraKit camera;
	GetAttachedView().GetSegmentKey().ShowCamera(camera);
	HPS::Point camera_position, camera_target;
	HPS::Vector camera_up;
	camera.ShowPosition(camera_position);
	camera.ShowTarget(camera_target);
	camera.ShowUpVector(camera_up);

	if (movement_flags & MovementFlags::rotating)
	{
		if (RotateScene(camera_position, camera_target))
			AdjustWalkingDirection(HPS::Vector(camera_target - camera_position).Normalize(), camera_up);
		else
			movement_flags &= ~MovementFlags::rotating;
	}
	if (movement_flags & MovementFlags::rotate_with_touch)
	{
		if (RotateWithTouch(camera_position, camera_target))
			AdjustWalkingDirection(HPS::Vector(camera_target - camera_position).Normalize(), camera_up);
		else
			movement_flags &= ~MovementFlags::rotate_with_touch;
	}
	if (movement_flags & MovementFlags::moving_forward)
		MoveForward(camera_position, camera_target, walking_direction.Normalize());
	if (movement_flags & MovementFlags::moving_back)
		MoveBack(camera_position, camera_target, walking_direction.Normalize());
	if (movement_flags & MovementFlags::moving_left)
	{
		if (IsXAxisInverted())
			MoveRight(camera_position, camera_target, camera_up);
		else
			MoveLeft(camera_position, camera_target, camera_up);
	}
	if (movement_flags & MovementFlags::moving_right)
	{
		if (IsXAxisInverted())
			MoveLeft(camera_position, camera_target, camera_up);
		else
			MoveRight(camera_position, camera_target, camera_up);
	}
	if (movement_flags & MovementFlags::moving_up)
	{
		if (IsYAxisInverted())
			MoveDown(camera_position, camera_target, camera_up);
		else
			MoveUp(camera_position, camera_target, camera_up);
	}
	if (movement_flags & MovementFlags::moving_down)
	{
		if (IsYAxisInverted())
			MoveUp(camera_position, camera_target, camera_up);
		else
			MoveDown(camera_position, camera_target, camera_up);
	}
	if (movement_flags & MovementFlags::move_with_touch)
	{
		if (!MoveWithTouch(camera_position, camera_target, camera_up, walking_direction.Normalize()))
			movement_flags &= ~MovementFlags::move_with_touch;
	}
    
	HPS::Vector ground_normal(ground.a, ground.b, ground.c);
	ground_normal.Normalize();

	camera.SetPosition(camera_position).SetTarget(camera_target).SetUpVector(-ground_normal);
	GetAttachedView().GetSegmentKey().SetCamera(camera);

	if (movement_flags)
	{
		Database::GetEventDispatcher().InjectEvent(CameraChangedEvent(GetAttachedView()));
		GetAttachedView().Update();
	}

	if (restore_length)
	{
		SetKeyboardSensitivity(GetKeyboardSensitivity() / scaling_factor);
		SetLeftJoystickSensitivity(GetLeftJoystickSensitivity() / scaling_factor);
		SetRightJoystickSensitivity(GetRightJoystickSensitivity() / scaling_factor);
	}

	last_tick_time = current_time;

	return false;
}

void HPS::WalkOperator::AdjustWalkingDirection(HPS::Vector const & camera_direction, HPS::Vector const & camera_up)
{
	HPS::Vector ground_normal(-ground.a, -ground.b, -ground.c);

	HPS::Vector vector_on_plane = camera_direction.Cross(camera_up);
	walking_direction = -vector_on_plane.Cross(ground_normal).Normalize();
}




HPS::SimpleWalkOperator::SimpleWalkOperator(MouseButtons in_mouse_trigger, ModifierKeys in_modifier_trigger) 
	: WalkOperator(in_mouse_trigger, in_modifier_trigger), 
	mouse_operator_started(false),
	mouse_moving(false),
	touch_operator_started(false),
	touch_moving(false),
	keybrd_control(false), 
	keybrd_shift(false),
	enable_middle_button_dolly(!in_mouse_trigger.Middle()), 
	middle_mouse_button_active(false),
	double_touch_active(false),
	pending_camera_valid(false)
{
	SetJoystickDeadZone(0.005f);
	SetMouseSensitivity(0.1f);
}

void HPS::SimpleWalkOperator::Init()
{
	GetAttachedView().GetSegmentKey().ShowCamera(last_camera);
	SetMouseSensitivity(GetSceneExtents());
}

void HPS::SimpleWalkOperator::OnViewAttached(HPS::View const & in_attached_view)
{
	WalkOperator::OnViewAttached(in_attached_view);
	Init();
}

void HPS::SimpleWalkOperator::OnModelAttached()
{
	WalkOperator::OnModelAttached(); 
	Init();
}

void HPS::SimpleWalkOperator::SetEnableMiddleMouseButtonDolly(bool enable)
{
	enable_middle_button_dolly = enable;
}
bool HPS::SimpleWalkOperator::GetEnableMiddleMouseButtonDolly() const
{
	return enable_middle_button_dolly;
}

bool HPS::SimpleWalkOperator::IsMouseTriggered(MouseState const  & in_state)
{ 
	return WalkOperator::IsMouseTriggered(in_state) || (enable_middle_button_dolly && in_state.GetButtons().Middle());
}

bool HPS::SimpleWalkOperator::OnTimerTick(HPS::TimerTickEvent const & in_event)
{
	if (pending_camera_valid)
	{
		if (last_notifier.Status() != HPS::Window::UpdateStatus::InProgress)
		{
			View view = GetAttachedView();
			view.GetSegmentKey().SetCamera(pending_camera);
			Database::GetEventDispatcher().InjectEvent(CameraChangedEvent(view));
			last_notifier = view.UpdateWithNotifier();
			pending_camera_valid = false;
		}
		return false;
	}

	static HPS::Time last_tick_time = -1;
	HPS::Time const current_time = in_event.GetTimeStamp();
	if (mouse_moving || touch_moving)
	{
		float time_scale = 1;
		if (last_tick_time >= 0)
		{
			HPS::Time const delta = current_time - last_tick_time;

			if (Float::Equals(delta, 0))
				return false;

			time_scale = static_cast<float>(delta/1000.0f);
		}

		int direction = 0;
		HPS::Point move_start_point, move_current_point;

		ScaleFactor scale(time_scale);
		scale.SetHorizontal(GetMouseSensitivity());
		scale.SetVertical(GetMouseSensitivity());
		scale.SetForward(GetMouseSensitivity());
		scale.SetYaw(50.0f);

		if (touch_moving)
		{
			if (double_touch_active)
				direction = DirectionHorizontal() | DirectionVertical();
			else
				direction = DirectionForward() | DirectionYaw();

			move_start_point = touch_start_point;
			move_current_point = touch_current_point;
		}
		else if (mouse_moving)
		{
			if (middle_mouse_button_active)
				direction = DirectionHorizontal() | DirectionVertical();
			else
				direction = DirectionForward() | DirectionYaw();

			move_start_point = mouse_start_point;
			move_current_point = mouse_current_point;
		}
		else
			return false;
		
		if (keybrd_control)
		{
			scale.SetForward(scale.GetForward() / 10.0f);
			scale.SetYaw(scale.GetYaw() * 0.55f);
		}
		else if (keybrd_shift)
		{
			scale.SetForward(scale.GetForward() * 10.0f);
		}

		View view = GetAttachedView();
		SegmentKey view_key = view.GetSegmentKey();
		view_key.ShowCamera(pending_camera);
		if (Move(move_start_point, move_current_point, direction, pending_camera, scale))
		{
			if (last_notifier.Status() == HPS::Window::UpdateStatus::InProgress)
			{
				pending_camera_valid = true;
			}
			else
			{
				view_key.SetCamera(pending_camera);
				Database::GetEventDispatcher().InjectEvent(CameraChangedEvent(view));
				last_notifier = view.UpdateWithNotifier();
			}
			last_camera = pending_camera;
		}
	}

	last_tick_time = current_time;

	return false;
}

void HPS::SimpleWalkOperator::SetKeyboardShiftKey(bool in_state)
{
	keybrd_shift = in_state;
}
void HPS::SimpleWalkOperator::SetKeyboardControlKey(bool in_state)
{
	keybrd_control = in_state;
}

bool HPS::SimpleWalkOperator::OnMouseDown(MouseState const & in_state)
{
	touch_moving = false;
	touch_operator_started = false;
	double_touch_active = false;

	if (IsMouseTriggered(in_state))
	{
		mouse_start_point = in_state.GetLocation();
		mouse_operator_started = true;
		return true;
	}

	return false;
}

bool HPS::SimpleWalkOperator::OnMouseMove(MouseState const & in_state)
{
	if (IsMouseTriggered(in_state) && mouse_operator_started)
	{
		mouse_current_point = in_state.GetLocation();
		mouse_moving = true;

		SetKeyboardControlKey(in_state.GetModifierKeys().Control());
		SetKeyboardShiftKey(in_state.GetModifierKeys().Shift());

		middle_mouse_button_active = enable_middle_button_dolly && in_state.GetButtons().Middle();

		return true;
	}

	return false;
}

bool HPS::SimpleWalkOperator::OnMouseUp(MouseState const & in_state)
{
	if (!IsMouseTriggered(in_state) && mouse_operator_started)
	{
		mouse_moving = false;
		middle_mouse_button_active = false;
		mouse_operator_started = false;
		return true;
	}

	return false;
}

bool HPS::SimpleWalkOperator::OnMouseEnter(MouseState const  & in_state)
{
	HPS_UNREFERENCED(in_state); return false; 
}

bool HPS::SimpleWalkOperator::OnMouseLeave(MouseState const  & in_state)
{ 
	if (!IsMouseTriggered(in_state))
	{
		mouse_moving = false;
		return true;
	}

	return false;
}

bool HPS::SimpleWalkOperator::OnMouseWheel(MouseState const & in_state) 
{
	const MouseEvent event = in_state.GetActiveEvent();
	if (in_state.GetModifierKeys().Shift())
	{
		const float speed_adjustment_factor = 1.2f;
		if (event.WheelDelta > 0)
			SetMouseSensitivity(GetMouseSensitivity()*speed_adjustment_factor);
		else
			SetMouseSensitivity(GetMouseSensitivity()/speed_adjustment_factor);
	}
	else
	{
		const float wheel_pan_sensitivity = 0.01f;
		const float phi = event.WheelDelta * wheel_pan_sensitivity;
		Pan(0, phi);
	}

	return true;
}

bool HPS::SimpleWalkOperator::OnTouchDown(TouchState const & in_state)
{
	HPS::TouchArray const & touches = in_state.GetActiveEvent().Touches;

	if (touches.size() != 2
		|| touches[0].ID == -1 
		|| touches[1].ID == -1)
	{
		touch_moving = false;
		touch_operator_started = false;
		double_touch_active = false;
		touch1.ID = touch2.ID = -1;
		return false;
	}

	touch1 = touches[0];
	touch2 = touches[1];
	touch_start_point = HPS::Midpoint(touch1.Location, touch2.Location);
	double_touch_active = true;
	touch_operator_started = true;
	return true;
}

bool HPS::SimpleWalkOperator::OnTouchMove(TouchState const & in_state)
{
	HPS::TouchArray const & touches = in_state.GetActiveEvent().Touches;

	if (touches.size() != 2)
	{
		touch_moving = false;
		touch_operator_started = false;
		double_touch_active = false;
		touch1.ID = touch2.ID = -1;
		return false;
	}

	if (touch_operator_started)
	{
		bool t1(false), t2(false);

		for (auto const touch : touches)
		{
			if (touch.ID == -1)
				continue;

			if (touch.ID == touch1.ID)
				t1 = true;

			if (touch.ID == touch2.ID)
				t2 = true;
		}

		if (t1 && t2)
		{
			double_touch_active = true;
			touch_current_point = HPS::Midpoint(touches[0].Location, touches[1].Location);
			SetKeyboardControlKey(in_state.GetModifierKeys().Control());
			SetKeyboardShiftKey(in_state.GetModifierKeys().Shift());
			touch_moving = true;
			return true;
		}
		else
		{
			touch_moving = false;
			touch_operator_started = false;
			double_touch_active = false;
			touch1.ID = touch2.ID = -1;
			return false;
		}
	}

	return false;
}

bool HPS::SimpleWalkOperator::OnTouchUp(TouchState const & in_state)
{
	bool untouch = false;
	HPS::TouchArray const & touches = in_state.GetActiveEvent().Touches; 
	if (touches.size() > 2)
		untouch = true;
	else
	{
		for (auto const touch : touches)
		{
			if (touch.ID == -1)
				continue;

			if (touch.ID == touch1.ID)
				untouch = true;

			if (touch.ID == touch2.ID)
				touch2.ID = -1;
		}
	}

	if (untouch)
	{
		touch_moving = false;
		touch_operator_started = false;
		double_touch_active = false;
		touch1.ID = touch2.ID = -1;
	}

	return false;
}


void HPS::SimpleWalkOperator::Pan(float in_theta, float in_phi)
{
	View view = GetAttachedView();
	SegmentKey view_key = view.GetSegmentKey();
	HPS::CameraKit camera;
	view_key.ShowCamera(camera);
	camera.Pan(in_theta, in_phi);
	last_camera = camera;
	view_key.SetCamera(camera);

	Database::GetEventDispatcher().InjectEvent(CameraChangedEvent(view));
	view.Update();
}

void HPS::SimpleWalkOperator::Dolly(float in_right, float in_up, float in_forward)
{
	View view = GetAttachedView();
	SegmentKey view_key = view.GetSegmentKey();
	HPS::CameraKit camera;
	view_key.ShowCamera(camera);
	camera.Dolly(in_right, in_up, in_forward);
	last_camera = camera;
	view_key.SetCamera(camera);

	Database::GetEventDispatcher().InjectEvent(CameraChangedEvent(view));
	view.Update();
}

float HPS::SimpleWalkOperator::CalculatePitch(HPS::CameraKit & camera) const
{
	HPS::Point position, target;
	camera.ShowPosition(position);
	camera.ShowTarget(target);
	HPS::Vector delta(target - position);
	double const length1 = delta.Length();
	float pos_axis = 0, targ_axis = 0;
	bool negative_up_axis = false;

	switch (GetPrimaryUpAxis())
	{
	case Axis::Negative_X:
		negative_up_axis = true;
	case Axis::X:
		delta.x = 0;
		pos_axis = position.x;
		targ_axis = target.x;
		break;
	case Axis::Negative_Y:
		negative_up_axis = true;
	case Axis::Y:
		delta.y = 0;
		pos_axis = position.y;
		targ_axis = target.y;
		break;
	case Axis::Negative_Z:
		negative_up_axis = true;
	case Axis::Z:
		delta.z = 0;
		pos_axis = position.z;
		targ_axis = target.z;
		break;
	}
	double const length2 = delta.Length();

	double angle = acos(length2/length1) * (180.0f / 3.1415927f);

	if (pos_axis > targ_axis)
		angle *= -1;
	if (negative_up_axis)
		angle *= -1;

	return static_cast<float>(angle);
}

void HPS::SimpleWalkOperator::ResetCamera(HPS::CameraKit & camera) const
{
	HPS::Point position, target; 
	camera.ShowTarget(target);
	camera.ShowPosition(position);
	HPS::Vector v(target - position);
	HPS::Vector adjustment(v);
	float const length = static_cast<float>(v.Length());
	bool negative_up_axis = false;
	switch (GetPrimaryUpAxis())
	{
		case Axis::Negative_X:
			negative_up_axis = true;
		case Axis::X:
			adjustment.x = 0;
			v = Vector::XAxis();
			break;
		case Axis::Negative_Y:
			negative_up_axis = true;
		case Axis::Y:
			adjustment.y = 0;
			v = Vector::YAxis();
			break;
		case Axis::Negative_Z:
			negative_up_axis = true;
		case Axis::Z:
			adjustment.z = 0;
			v = Vector::ZAxis();
			break;
	}
	adjustment.Normalize();
	if (negative_up_axis)
		v *= -1;

	HPS::Point target2 = position + (adjustment * length);
	camera.SetTarget(target2);
	camera.SetUpVector(v);
	camera.SetProjection(HPS::Camera::Projection::Perspective);
}

bool HPS::SimpleWalkOperator::NotableCameraDifference(HPS::CameraKit const & camera1, HPS::CameraKit const & camera2)
{
	int const tolerance = 12;
	Point p1, p2, t1, t2; 
	Vector v1, v2;
	float w1, h1, w2, h2;
	HPS::Camera::Projection proj1, proj2;

	if (!camera1.ShowPosition(p1))
		return true;
	if (!camera1.ShowTarget(t1))
		return true;
	if (!camera1.ShowField(w1, h1))
		return true;
	if (!camera1.ShowUpVector(v1))
		return true;
	if (!camera1.ShowProjection(proj1))
		return true;

	if (!camera2.ShowPosition(p2))
		return true;
	if (!camera2.ShowTarget(t2))
		return true;
	if (!camera2.ShowField(w2, h2))
		return true;
	if (!camera2.ShowUpVector(v2))
		return true;
	if (!camera2.ShowProjection(proj2))
		return true;

	Vector eye1(t1 - p1);
	Vector eye2(t2 - p2);
	eye1.Normalize();
	eye2.Normalize();
	if (!eye1.Equals(eye2, tolerance))
	{
		HPS::Vector const cross(eye1.Cross(eye2));
		float const angle = static_cast<float>(atan2(cross.Length(), eye1.Dot(eye2))) * (180.0f/3.1415927f);

		if (angle > 12)
			return true;
	}

	if (GetPrimaryAxis(v1) != GetPrimaryAxis(v2))
		return true;

	if (!Float::Equals(w1, w2, tolerance) ||
		!Float::Equals(h1, h2, tolerance))
		return true;

	if (proj1 != proj2)
		return true;

	return false;

}


bool HPS::SimpleWalkOperator::Move(HPS::Point const & start, HPS::Point const & current, int directions, HPS::CameraKit & camera, ScaleFactor const & scale)
{
	bool moved = false;
	float pitch_angle(0);
	if (NotableCameraDifference(camera, last_camera))
	{
		HPS::Vector up;
		camera.ShowUpVector(up);
		UpdatePrimaryUpAxis(up);
		pitch_angle = CalculatePitch(camera);
		ResetCamera(camera);
	}
	else
		pitch_angle = CalculatePitch(camera);

	if (directions & DirectionForward())
	{
		float const y_delta = current.y - start.y;
		if (fabs(y_delta) > GetJoystickDeadZone())
		{
			ResetCamera(camera);
			float y_multiplier = scale.Forward();
			if (IsYAxisInverted())
				y_multiplier *= -1;
			camera.Dolly(0, 0, y_delta*y_multiplier);
			moved = true;
		}
	}
	else if (directions & DirectionVertical())
	{
		float const y_delta = current.y - start.y;
		if (fabs(y_delta) > GetJoystickDeadZone())
		{
			ResetCamera(camera);
			float y_multiplier = scale.Vertical();
			if (IsYAxisInverted())
				y_multiplier *= -1;
			camera.Dolly(0, y_delta*y_multiplier, 0);
			moved = true;
		}
	}

	if (directions & DirectionYaw())
	{
		float const x_delta = current.x - start.x;
		if (fabs(x_delta) > GetJoystickDeadZone())
		{
			ResetCamera(camera);
			float x_multiplier = scale.Yaw();
			if (!IsXAxisInverted())
				x_multiplier *= -1;
			camera.Pan(x_delta*x_multiplier, 0);
			moved = true;
		}
	}
	else if (directions & DirectionHorizontal())
	{
		float const x_delta = current.x - start.x;
		if (fabs(x_delta) > GetJoystickDeadZone())
		{
			ResetCamera(camera);
			float x_multiplier = scale.Horizontal();
			if (!IsXAxisInverted())
				x_multiplier *= -1;
			camera.Dolly(x_delta*x_multiplier, 0, 0);
			moved = true;
		}
	}

	if (moved)
	{
		ResetCamera(camera);
		camera.Pan(0, pitch_angle);
	}

	return moved;
}
