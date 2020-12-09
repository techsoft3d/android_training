#pragma once

#include "hps.h"
#include "sprk.h"
#include "sprk_ops.h"
#ifdef USING_EXCHANGE
#include "sprk_exchange.h"
#endif

// MobileSurface is a plaform-independent base class which gui code will communicate with
//  to handle surface creation/updates/destruction, as well as input events.
// Users *should not* modify this class; instead, users should modify UserMobileSurface.
// There may be multiple instances of MobileSurface depending on if the gui has multiple
// surface views or not.

static const int SCREEN_ROTATING = 0x00000001;

class MobileSurface
{
public:

	MobileSurface();
	virtual ~MobileSurface() {}

    // Indicates if this surface is valid or not.
    // Note that rotating the device in Android will cause the surface to become invalid until bind is called again (by gui code)
    // isValid() should be called before performing any Update()
	bool			isValid() { return _valid; }

    // Called to bind a window id to for HPS to render on to
	virtual bool	bind(void *window);

    // Called when the surface is about to become invalid
    virtual void    release(int flags);

    // Called to explicitly update the HPS surface
    virtual void    refresh();

    // Touch Down/Move/Up Input Events
	virtual void	touchDown(int numTouches, int xPosArray[], int yPosArray[], HPS::TouchID idArray[], size_t tapCount);
	virtual void	touchMove(int numTouches, int xPosArray[], int yPosArray[], HPS::TouchID idArray[]);
	virtual void	touchUp(int numTouches, int xPosArray[], int yPosArray[], HPS::TouchID idArray[]);

    // Called when tracked touches should be cancelled
    virtual void    touchesCancel();

    // Single/double-tap gestures
	virtual void	singleTap(int x, int y);
	virtual void	doubleTap(int x, int y, HPS::TouchID id);

    // Return HPS::Canvas instance associated with this surface
	HPS::Canvas		GetCanvas() const { return _canvas; }

protected:
	void InjectTouchEvent(HPS::TouchEvent::Action action, int numTouches, int xposArray[], int yposArray[], HPS::TouchID idArray[], size_t tapCount = 1);

private:
	bool			_valid;
	HPS::Canvas		_canvas;
};

// Users must implement createMobileSurface() to return a pointer to their derived MobileSurface
MobileSurface *createMobileSurface(int guiSurfaceId);