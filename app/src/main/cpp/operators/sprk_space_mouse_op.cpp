#include "sprk_space_mouse_op.h"
#include <algorithm>
//#include <cwtype.h>

#pragma comment(lib, "dinput8")
#pragma comment(lib, "dxguid")

SpaceMouseOperator::SpaceMouseOperator(HPS::WindowHandle in_window_handle, Canvas const & in_canvas)
	: Operator()
	, di_initialized(false)
	, di_interface(nullptr)
	, window_handle(in_window_handle)
	, canvas(in_canvas)
	, bbx_radius(1.0f)
	, bbx_center(Point(0, 0, 0))
{ 
	button_states.resize(32);
	for (size_t i = 0; i < button_states.size(); ++i)
		button_states[i] = false;
}

SpaceMouseOperator::~SpaceMouseOperator()
{ 
	di_interface->Release();
}

void SpaceMouseOperator::OnViewAttached(HPS::View const & in_attached_view)
{ 
	if (InitializeDirectInput())
		di_initialized = true;

	attached_view = in_attached_view;
	view_segment = attached_view.GetSegmentKey();

	Model attached_model = attached_view.GetAttachedModel();
	if (attached_model.Type() != HPS::Type::None)
		GetModelBounding(attached_model);
}

void SpaceMouseOperator::OnModelAttached()
{
	GetModelBounding(attached_view.GetAttachedModel());
}

void SpaceMouseOperator::GetModelBounding(Model const & model)
{
	SprocketPath spath(canvas, canvas.GetAttachedLayout(), attached_view, model);
	path_to_model = spath.GetKeyPath();

	BoundingKit bounding;
	if (!attached_view.GetAttachedModel().GetSegmentKey().ShowBounding(bounding))
		Database::GetEventDispatcher().InjectEvent(WarningEvent("Space Mouse Operator - Failed to get model bounding, movement may be erratic."));
	else
	{
		SimpleSphere sphere;
		SimpleCuboid cuboid;
		bounding.ShowVolume(sphere, cuboid);
		bbx_radius = sphere.radius;
		bbx_center = sphere.center;
	}
}

bool SpaceMouseOperator::OnTimerTick(HPS::TimerTickEvent const &)
{ 
	if (!di_initialized)
		return false;

	if (UpdateState())
	{
		//holding the pressure at 100% for a frame will translate by 5% of the default camera field
		static long max_displacement = 1400;
		float camera_field = 2;
		attached_view.GetSegmentKey().GetCameraControl().ShowField(camera_field, camera_field);

		static float max_movement_per_frame = 0.05f;
		float translation_factor = camera_field * max_movement_per_frame;
		float deadzone = 0.05f * (float)max_displacement;
		auto & state = di_device_info.di_joystick;

		HandleButtons(state.rgbButtons);

		if (std::abs(state.lRx) < deadzone)
			state.lRx = 0;
		if (std::abs(state.lRy) < deadzone)
			state.lRy = 0;
		if (std::abs(state.lRz) < deadzone)
			state.lRz = 0;
		if (std::abs(state.lX) < deadzone)
			state.lX = 0;
		if (std::abs(state.lY) < deadzone)
			state.lY = 0;
		if (std::abs(state.lZ) < deadzone)
			state.lZ = 0;
		
		float percentage_pressure_x = (float)state.lX / (float)(max_displacement);
		float translation_x = percentage_pressure_x * translation_factor;
		float percentage_pressure_y = (float)state.lY / (float)(max_displacement);
		float translation_y = percentage_pressure_y * translation_factor;
		float percentage_pressure_z = (float)state.lZ / (float)(max_displacement);
		float translation_z = percentage_pressure_z * translation_factor;

		CameraKit current_camera;
		CameraKit previous_camera = current_camera;
		view_segment.ShowCamera(current_camera);
		Point camera_target;
		current_camera.ShowTarget(camera_target);

		Point xform_center_of_rotation;
		Point xform_camera_target;
		path_to_model.ConvertCoordinate(Coordinate::Space::World, bbx_center, Coordinate::Space::Camera, xform_center_of_rotation);
		path_to_model.ConvertCoordinate(Coordinate::Space::World, camera_target, Coordinate::Space::Camera, xform_camera_target);
		Vector delta = xform_center_of_rotation - xform_camera_target;

		//The Y and Z are swapped to match how the default space mouse demo works
		current_camera.Dolly(translation_x, translation_z, translation_y);

		//holding the pressure at 100% for a frame will rotate by 2 degrees
		static float max_rotation_angle_per_frame = 10.0f;
		float percentage_pressure_rx = (float)state.lRx / (float)(max_displacement);
		float rotation_x = percentage_pressure_rx * max_rotation_angle_per_frame;
		float percentage_pressure_ry = (float)state.lRy / (float)(max_displacement);
		float rotation_y = percentage_pressure_ry * max_rotation_angle_per_frame;
		float percentage_pressure_rz = (float)state.lRz / (float)(max_displacement);
		float rotation_z = percentage_pressure_rz * max_rotation_angle_per_frame;

		current_camera.Dolly(delta.x, delta.y, delta.z);

		current_camera.Orbit(-rotation_z, rotation_x);
		current_camera.Roll(rotation_y);

		current_camera.Dolly(-delta.x, -delta.y, -delta.z);

		view_segment.SetCamera(current_camera);

		if (previous_camera != current_camera)
			attached_view.Update();
	}

	return true;
}

void SpaceMouseOperator::HandleButtons(unsigned char const buttons[32])
{
	const size_t button_count = 32;
	for (size_t i = 0; i < button_count; ++i)
	{
		if (buttons[i] > 0 && button_states[i] == false)
		{
			button_states[i] = true;

			char message[128];
			sprintf_s(message, 128, "Button %zd pressed\n", i);
			OutputDebugStringA(message);
		}
		else if (buttons[i] == 0 && button_states[i] == true)
		{
			button_states[i] = false;

			char message[128];
			sprintf_s(message, 128, "Button %zd released\n", i);
			OutputDebugStringA(message);
		}
	}
}

bool SpaceMouseOperator::InitializeDirectInput()
{
	HRESULT hr = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&di_interface, nullptr);
	if (FAILED(hr))
	{
		Database::GetEventDispatcher().InjectEvent(WarningEvent("Space Mouse Operator - Failed to create a Direct Input device"));
		return false;
	}

	
	hr = di_interface->EnumDevices(DIDEVTYPE_HID, &SMEnumDevices, this, DIEDFL_ATTACHEDONLY);
	if (FAILED(hr))
	{
		Database::GetEventDispatcher().InjectEvent(WarningEvent("Space Mouse Operator - Failed to enumerate devices"));
		return false;
	}
	else if (di_device_info.di_hid_device == nullptr)
	{
		Database::GetEventDispatcher().InjectEvent(WarningEvent("Space Mouse Operator - Failed to create device"));
		return false;
	}

	hr = di_device_info.di_hid_device->SetDataFormat(&c_dfDIJoystick);
	if (FAILED(hr))
	{
		Database::GetEventDispatcher().InjectEvent(WarningEvent("Space Mouse Operator - Failed to set data format"));
		return false;
	}

	//Setup the Space Mouse to only receive events when the window has focus.
	//To receive events all the time, pass DISCL_NONEXCLUSIVE | DISCL_BACKGROUND instead.
	hr = di_device_info.di_hid_device->SetCooperativeLevel((HWND)window_handle, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr))
	{
		Database::GetEventDispatcher().InjectEvent(WarningEvent("Space Mouse Operator - Failed to set cooperative level"));
		return false;
	}

	hr = di_device_info.di_hid_device->EnumObjects(SMEnumAxes, (void *)&di_device_info, DIDFT_AXIS);
	if (FAILED(hr))
	{
		Database::GetEventDispatcher().InjectEvent(WarningEvent("Space Mouse Operator - Failed to set axes properties"));
		return false;
	}

	return true;
}

BOOL CALLBACK SpaceMouseOperator::SMEnumDevices(DIDEVICEINSTANCE const * instance, void * context)
{
	std::wstring product_name(instance->tszProductName);
	std::transform(product_name.begin(), product_name.end(), product_name.begin(), towlower);
	if (product_name.find(L"spacemouse") != std::wstring::npos)
	{
		//found the space mouse
		SpaceMouseOperator * op = reinterpret_cast<SpaceMouseOperator *>(context);
		ZeroMemory(&op->di_device_info.di_joystick, sizeof(DIJOYSTATE));
		op->di_device_info.di_instance = *instance;
		HRESULT hr = op->di_interface->CreateDevice(instance->guidInstance, &op->di_device_info.di_hid_device, nullptr);

		if (FAILED(hr))
			Database::GetEventDispatcher().InjectEvent(WarningEvent("Space Mouse Operator - Failed to create a Direct Input device"));

		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;
}

BOOL CALLBACK SpaceMouseOperator::SMEnumAxes(DIDEVICEOBJECTINSTANCE const *, void * context)
{
	DIDeviceInfo * device_info = reinterpret_cast<DIDeviceInfo *>(context);

	DIPROPDWORD properties;
	properties.diph.dwSize = sizeof(DIPROPDWORD);
	properties.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	properties.diph.dwHow = DIPH_DEVICE;
	properties.diph.dwObj = 0;
	properties.dwData = DIPROPAXISMODE_REL;

	HRESULT hr = device_info->di_hid_device->SetProperty(DIPROP_AXISMODE, &properties.diph);
	if (FAILED(hr))
		return DIENUM_STOP;

	properties.dwData = 10;

	hr = device_info->di_hid_device->SetProperty(DIPROP_BUFFERSIZE, &properties.diph);
	if (FAILED(hr))
		return DIENUM_STOP;

	return DIENUM_CONTINUE;
}

bool SpaceMouseOperator::UpdateState()
{
	if (di_device_info.di_hid_device == nullptr)
		return true;

	HRESULT hr = di_device_info.di_hid_device->Poll();
	if (FAILED(hr))
	{
		hr = di_device_info.di_hid_device->Acquire();
		while (hr == DIERR_INPUTLOST)
			hr = di_device_info.di_hid_device->Acquire();

		return true;
	}

	hr = di_device_info.di_hid_device->GetDeviceState(sizeof(DIJOYSTATE), &di_device_info.di_joystick);
	if (FAILED(hr))
		return false;

	return true;
}