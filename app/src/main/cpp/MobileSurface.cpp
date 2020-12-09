
#include "MobileApp.h"
#include "MobileSurface.h"
#include "dprintf.h"

// g_android_platform_data is initialized in Android platforms
HPS::PlatformData g_android_platform_data;

MobileSurface::MobileSurface()
	: _valid(false)
{
}

bool MobileSurface::bind(void *window)
{
	// Ensure app is created
	MobileApp::inst();

    // Initialize if this is the first time called
	if (_canvas.Type() == HPS::Type::None)
	{
        // Initialize window options.  Note that OpenGL2 is the only valid mobile driver
		HPS::ApplicationWindowOptionsKit		windowOpts;
		windowOpts.SetDriver(HPS::Window::Driver::OpenGL2);
		windowOpts.SetAntiAliasCapable(false);
		windowOpts.SetFramebufferRetention(false);

		if (g_android_platform_data != 0)
			windowOpts.SetPlatformData(g_android_platform_data);

        // Specify window handle
		_canvas = HPS::Factory::CreateCanvas(reinterpret_cast<HPS::WindowHandle>(window), "", windowOpts);
	}
	else if (_valid == false)
	{
		// If the surface is invalid (was destroyed), notify HPS we now have a new one.
        HPS::ApplicationWindowKey awk(_canvas.GetWindowKey());
        awk.GetWindowOptionsControl().SetWindowHandle(reinterpret_cast<HPS::WindowHandle>(window));
	}
    
	touchesCancel();

	_valid = true;
    _canvas.Update();

	return true;
}


void MobileSurface::release(int flags)
{
	// Perform blocking update to flush any current or pending updates, then signal that the surface is invalid.
	HPS::UpdateNotifier notifier = _canvas.UpdateWithNotifier(HPS::Window::UpdateType::Complete);
	notifier.Wait();

	// Don't destroy canvas if we're only rotating the screen.
	if ((flags & SCREEN_ROTATING) == 0)
	{
	    _canvas.Delete();
	    HPS::Database::Synchronize();
	}

    _valid = false;
}

void MobileSurface::refresh()
{
    if (!isValid())
        return;
    
    if (_canvas.Type() != HPS::Type::None)
        _canvas.Update(HPS::Window::UpdateType::Refresh);
}

void MobileSurface::touchDown(int numTouches, int xposArray[], int yposArray[], HPS::TouchID idArray[], size_t tapCount)
{
	InjectTouchEvent(HPS::TouchEvent::Action::TouchDown, numTouches, xposArray, yposArray, idArray, tapCount);
}

void MobileSurface::touchMove(int numTouches, int xposArray[], int yposArray[], HPS::TouchID idArray[])
{
	InjectTouchEvent(HPS::TouchEvent::Action::Move, numTouches, xposArray, yposArray, idArray);
}

void MobileSurface::touchUp(int numTouches, int xposArray[], int yposArray[], HPS::TouchID idArray[])
{
	InjectTouchEvent(HPS::TouchEvent::Action::TouchUp, numTouches, xposArray, yposArray, idArray);
}

void MobileSurface::touchesCancel()
{
    // TouchUp event with empty touch array clears tracked touches
	InjectTouchEvent(HPS::TouchEvent::Action::TouchUp, 0, 0, 0, 0);
}

void MobileSurface::singleTap(int x, int y)
{
	touchesCancel();
}

void MobileSurface::doubleTap(int x, int y, HPS::TouchID id)
{
    touchesCancel();
}

void MobileSurface::InjectTouchEvent(HPS::TouchEvent::Action action, int numTouches, int xposArray[], int yposArray[], HPS::TouchID idArray[], size_t tapCount)
{
    if (!isValid())
        return;
    
	HPS::WindowKey			windowKey = _canvas.GetWindowKey();
	HPS::TouchArray			touches;
    touches.reserve(numTouches);

	for (int i=0; i < numTouches; i++)
	{
        // Touch info needs to be converted from Pixel to Window space
		HPS::Point	p(xposArray[i], yposArray[i], 0);
		windowKey.ConvertCoordinate(HPS::Coordinate::Space::Pixel, p, HPS::Coordinate::Space::Window, p);
		touches.push_back(HPS::Touch(idArray[i], p, tapCount));
	}

	HPS::TouchEvent			event(action, touches);
	windowKey.GetEventDispatcher().InjectEvent(event);
}
