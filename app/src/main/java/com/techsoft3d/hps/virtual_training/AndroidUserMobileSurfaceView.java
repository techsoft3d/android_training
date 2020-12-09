package com.techsoft3d.hps.virtual_training;

// Auto-generated file

import android.content.Context;

public class AndroidUserMobileSurfaceView extends AndroidMobileSurfaceView {
	private static native boolean loadFileS(long ptr, String fileName);
	private static native void setOperatorOrbitV(long ptr);
	private static native void onModeSimpleShadowZ(long ptr, boolean enable);
	private static native void onModeSmoothV(long ptr);
	private static native void onModeHiddenLineV(long ptr);
	private static native void onUserCode1V(long ptr);
	private static native void onUserCode2V(long ptr);
	private static native void onUserCode3V(long ptr);
	private static native void onUserCode4V(long ptr);

	public AndroidUserMobileSurfaceView(Context context) {
		super(context);
	}

	public AndroidUserMobileSurfaceView(Context context, AndroidMobileSurfaceView.Callback svcb, int guiSurfaceId, long savedSurfacePointer) {
		super(context, svcb, guiSurfaceId, savedSurfacePointer);
	}

	public  boolean loadFile(String fileName) {
		return  loadFileS(mSurfacePointer, fileName);
	}


	public  void setOperatorOrbit() {
		 setOperatorOrbitV(mSurfacePointer);
	}


	public  void onModeSimpleShadow(boolean enable) {
		 onModeSimpleShadowZ(mSurfacePointer, enable);
	}


	public  void onModeSmooth() {
		 onModeSmoothV(mSurfacePointer);
	}


	public  void onModeHiddenLine() {
		 onModeHiddenLineV(mSurfacePointer);
	}


	public  void onUserCode1() {
		 onUserCode1V(mSurfacePointer);
	}


	public  void onUserCode2() {
		 onUserCode2V(mSurfacePointer);
	}


	public  void onUserCode3() {
		 onUserCode3V(mSurfacePointer);
	}


	public  void onUserCode4() {
		 onUserCode4V(mSurfacePointer);
	}


}

