package com.techsoft3d.hps.virtual_training;

import android.content.Context;
import android.view.Display;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.WindowManager;

/**
 * AndroidMobileSurfaceView is the SurfaceView which HPS will render on to.
 * 
 * This class serves as a base class for AndroidUserMobileSurfaceView, and
 * users should instead create an instance of AndroidUserMobileSurfaceView.
 * 
 * AndroidMobileSurfaceView handles surface creation/destruction and touch input
 * by communicating with the C++ MobileSurface class.
 *
 */
public class AndroidMobileSurfaceView extends SurfaceView implements SurfaceHolder.Callback {
	
	public static native void onTextInputJS(long ptr, String text);
	public static native long create(AndroidMobileSurfaceView view, int surfaceId);
	public static native void onKeyboardHiddenJ(long ptr);
	public static native boolean bind(long ptr, Object context, Object surface);
	public static native void release(long ptr, int flags);
	public static native void refresh(long ptr);
	public static native void touchDown(long ptr, int numTouches, int[] xArray, int[] yArray, long[] posArray);
	public static native void touchMove(long ptr, int numTouches, int[] xArray, int[] yArray, long[] posArray);
	public static native void touchUp(long ptr, int numTouches, int[] xArray, int[] yArray, long[] posArray);
	public static native void touchesCancel(long ptr);
	public static native void singleTap(long ptr, int x, int y);
	public static native void doubleTap(long ptr, int x, int y, long id);
	
	private static int SCREEN_ROTATING = 0x00000001;	
	private int lastRotation;

	// Surface id integer seen by C++ code
	private int mGuiSurfaceId;

	private boolean surfaceIsValid = false;
	
	// Pointer to UserMobileSurface instance associated with this SurfaceView
	protected long mSurfacePointer;

	private GestureDetector mGestureDetector;
	private AndroidMobileSurfaceView.Callback mSurfaceViewCallback;

	public void onTextInput(String text)
	{
		onTextInputJS(mSurfacePointer, text);
	}
	
	public void onKeyboardHidden()
	{
		onKeyboardHiddenJ(mSurfacePointer);
	}
	
	public interface Callback {
		// Called with return value of MobileSurface::bind() 
		public void onSurfaceBind(boolean bindRet); 
	}

	// Constructor should only be called by derived class
	protected AndroidMobileSurfaceView(Context context) {
		this(context, null, 0, 0);
	}
	
	// Constructor should only be called by derived class
	protected AndroidMobileSurfaceView(Context context, AndroidMobileSurfaceView.Callback svcb, int guiSurfaceId, long savedSurfacePointer) {
		super(context);

		mGuiSurfaceId = guiSurfaceId;
		
		if (savedSurfacePointer != 0)
			mSurfacePointer = savedSurfacePointer;
		else
			mSurfacePointer = create(this,mGuiSurfaceId);
		
		mSurfaceViewCallback = svcb;
		getHolder().addCallback(this);
		
		mGestureDetector = new GestureDetector(context, new CustomGestureDetector());
	}

	public int getGuiSurfaceId() {
		return mGuiSurfaceId;
	}
	
	public long getSurfacePointer() {
		return mSurfacePointer;
	}
	
	public void clearTouches() {
		touchesCancel(mSurfacePointer);
	}
	
	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		
		Display display = ((WindowManager) getContext().getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
		lastRotation = display.getRotation ();
		
		boolean ret = bind(mSurfacePointer, getContext(), getHolder().getSurface());
		if (mSurfaceViewCallback != null)
			mSurfaceViewCallback.onSurfaceBind(ret);

		surfaceIsValid = ret;
	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {

		if (surfaceIsValid) {
			Display display = ((WindowManager) getContext().getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
			int rotation = display.getRotation();

			int flags = 0;
			if (rotation != lastRotation)
				flags |= SCREEN_ROTATING;

			release(mSurfacePointer, flags);

			lastRotation = rotation;

			surfaceIsValid = false;
		}
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent e) {

		if (mGestureDetector.onTouchEvent(e))
			return true;
		
		final int action = e.getActionMasked();
		
		// Only track multiple move changes.  Each separate touch up/down gets its own action.
		int pointerCount = 1;
		if (action == MotionEvent.ACTION_MOVE) {
			pointerCount = e.getPointerCount();
		}

		int[] xposArray = new int[pointerCount];
		int[] yposArray = new int[pointerCount];
		long[] idArray = new long[pointerCount];
		
		switch (action) {
		case MotionEvent.ACTION_DOWN: {
			// Single touch down
			xposArray[0] = (int) e.getX();
			yposArray[0] = (int) e.getY();
			idArray[0] = e.getPointerId(0);
			touchDown(mSurfacePointer, pointerCount, xposArray, yposArray, idArray);
			break;
		}
		case MotionEvent.ACTION_UP: {
			// Last touch went up
			xposArray[0] = (int) e.getX();
			yposArray[0] = (int) e.getY();
			idArray[0] = e.getPointerId(0);
			touchUp(mSurfacePointer, pointerCount, xposArray, yposArray, idArray);
			break;
		}
		case MotionEvent.ACTION_POINTER_DOWN: {
			// Additional touch down
			final int index = e.getActionIndex();
			xposArray[0] = (int) e.getX(index);
			yposArray[0] = (int) e.getY(index);
			idArray[0] = e.getPointerId(index);
			touchDown(mSurfacePointer, pointerCount, xposArray, yposArray, idArray);
			break;
		}
		case MotionEvent.ACTION_POINTER_UP: {
			// Touch went up, but not last
			final int index = e.getActionIndex();
			xposArray[0] = (int) e.getX(index);
			yposArray[0] = (int) e.getY(index);
			idArray[0] = e.getPointerId(index);
			touchUp(mSurfacePointer, pointerCount, xposArray, yposArray, idArray);
			break;
		}
		case MotionEvent.ACTION_MOVE: {
			// Multiple touches move
			for (int i=0; i<pointerCount; i++) {
				xposArray[i] = (int)e.getX(i);
				yposArray[i] = (int)e.getY(i);
				idArray[i] 	 = e.getPointerId(i);	
			}
			touchMove(mSurfacePointer, pointerCount, xposArray, yposArray, idArray);
			break;
		}
		case MotionEvent.ACTION_CANCEL: {
			touchesCancel(mSurfacePointer);
			break;
		}
		default:
			return super.onTouchEvent(e);
		}

		return true;
	}

	private class CustomGestureDetector extends GestureDetector.SimpleOnGestureListener {
		@Override
		public boolean onDoubleTap(MotionEvent e) {
			AndroidMobileSurfaceView sv = AndroidMobileSurfaceView.this;
			AndroidMobileSurfaceView.doubleTap(sv.mSurfacePointer, (int)e.getX(), (int)e.getY(), (int)e.getPointerId(0));
			return true;
		}

		@Override
		public boolean onSingleTapConfirmed(MotionEvent e) {
			AndroidMobileSurfaceView sv = AndroidMobileSurfaceView.this;
			AndroidMobileSurfaceView.singleTap(sv.mSurfacePointer, (int)e.getX(), (int)e.getY());
			return true;
		}
	}
}
