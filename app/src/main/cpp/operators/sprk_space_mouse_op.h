#include "sprk.h"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

using namespace HPS;

class SpaceMouseOperator : public Operator
{
public:
	SpaceMouseOperator(HPS::WindowHandle in_window_handle, Canvas const & in_canvas);
	~SpaceMouseOperator();

	virtual HPS::UTF8 GetName() const override { return "HPS_SpaceMouseOperator"; }

	virtual void OnViewAttached(HPS::View const & attached_view) override;
	virtual void OnModelAttached() override;
	virtual bool OnTimerTick(HPS::TimerTickEvent const & in_event) override;

	void HandleButtons(unsigned char const buttons[32]);

private:
	class DIDeviceInfo
	{
	public:
		DIDEVICEINSTANCE di_instance;
		DIJOYSTATE di_joystick;
		LPDIRECTINPUTDEVICE8 di_hid_device;
	};

	void GetModelBounding(Model const & model);
	bool InitializeDirectInput();
	bool UpdateState();
	static BOOL CALLBACK SMEnumDevices(DIDEVICEINSTANCE const * instance, void * context);
	static BOOL CALLBACK SMEnumAxes(DIDEVICEOBJECTINSTANCE const * instance, void * context);

	bool di_initialized;
	HPS::WindowHandle window_handle;
	LPDIRECTINPUT8 di_interface;
	DIDeviceInfo di_device_info;
	std::vector<bool> button_states;

	float bbx_radius;
	Point bbx_center;

	View attached_view;
	Canvas canvas;
	SegmentKey view_segment;
	KeyPath path_to_model;
	
};