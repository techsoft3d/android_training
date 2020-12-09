#include <jni.h>
#include <android/log.h>
#include <android/input.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>

#include <android/native_window_jni.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "MobileSurface.h"
#include "JNIHelpers.h"

#include "jpaths.h"

#define  LOG_TAG    "AndroidMobileSurfaceJNI"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static void * platform_data[] = {(void*)2, 0, 0};
extern JavaVM *g_javaVM;
extern HPS::PlatformData g_android_platform_data;

JNIHelpers::ShowKeyboardHandler show_keyboard_handler;

jobject classObject;
jclass classz;

static jlong create(JNIEnv * env, jclass cobj, jobject classObj, int guiSurfaceId)
{
	classObject = env->NewGlobalRef(classObj);
	classz = (jclass)env->NewGlobalRef(env->GetObjectClass(classObject));
	return (jlong)createMobileSurface(guiSurfaceId);
}

static void onTextInputJS(JNIEnv *env, jclass cobj, jlong ptr, jstring text)
{
	JNIHelpers::String ctext(env, text);
	HPS::TextInputEvent hps_event(HPS::UTF8(ctext.str()));

	auto mobileSurface = ((MobileSurface*)ptr);

	if (mobileSurface->isValid())
    	mobileSurface->GetCanvas().GetWindowKey().GetEventDispatcher().InjectEvent(hps_event);
}

static void onKeyboardHiddenJ(JNIEnv *env, jclass cobj, jlong ptr)
{
	HPS::HideKeyboardEvent hps_event;
	HPS::Database::GetEventDispatcher().InjectEventWithNotifier(hps_event).Wait();
}

static jboolean bind(JNIEnv * env, jclass cobj, jlong ptr, jobject context, jobject surface)
{
	g_android_platform_data = (intptr_t)platform_data;
	platform_data[1] = g_javaVM;
	platform_data[2] = env->NewGlobalRef(context);
	EGLNativeWindowType		nativeWindow = ANativeWindow_fromSurface(env, surface);

	HPS::Database::GetEventDispatcher().Subscribe(show_keyboard_handler, HPS::Object::ClassID<HPS::ShowKeyboardEvent>());

	return ((MobileSurface*)ptr)->bind(nativeWindow);
}

static void release(JNIEnv *env, jclass cobj, jlong ptr, jint flags)
{
	((MobileSurface*)ptr)->release(flags);
}

static void refresh(JNIEnv *env, jclass cobj, jlong ptr)
{
	((MobileSurface*)ptr)->refresh();
}

static void touchDown(JNIEnv * env, jclass cobj, jlong ptr, jint numTouches, jintArray xposArray, jintArray yposArray, jlongArray idArray)
{
	JNIHelpers::IntArray xa(env, xposArray);
	JNIHelpers::IntArray ya(env, yposArray);
	JNIHelpers::LongArray ida(env, idArray);
	((MobileSurface*)ptr)->touchDown(numTouches, xa.arr(), ya.arr(), (HPS::TouchID*)ida.arr(), 1);
}

static void touchMove(JNIEnv * env, jclass cobj, jlong ptr, jint numTouches, jintArray xposArray, jintArray yposArray, jlongArray idArray)
{
	JNIHelpers::IntArray xa(env, xposArray);
	JNIHelpers::IntArray ya(env, yposArray);
	JNIHelpers::LongArray ida(env, idArray);
	((MobileSurface*)ptr)->touchMove(numTouches, xa.arr(), ya.arr(), (HPS::TouchID*)ida.arr());
}

static void touchUp(JNIEnv * env, jclass cobj, jlong ptr, jint numTouches, jintArray xposArray, jintArray yposArray, jlongArray idArray)
{
	JNIHelpers::IntArray xa(env, xposArray);
	JNIHelpers::IntArray ya(env, yposArray);
	JNIHelpers::LongArray ida(env, idArray);
	((MobileSurface*)ptr)->touchUp(numTouches, xa.arr(), ya.arr(), (HPS::TouchID*)ida.arr());
}

static void touchesCancel(JNIEnv * env, jclass obj, jlong ptr)
{
	((MobileSurface*)ptr)->touchesCancel();
}

static void singleTap(JNIEnv * env, jclass cobj, jlong ptr, jint x, jint y)
{
	((MobileSurface*)ptr)->singleTap(x, y);
}

static void doubleTap(JNIEnv * env, jclass cobj, jlong ptr, jint x, jint y, jlong id)
{
	((MobileSurface*)ptr)->doubleTap(x, y, id);
}

static void onShowKeyboard()
{
	JNIEnv *env = nullptr;
	int status = g_javaVM->GetEnv((void **)&env, JNI_VERSION_1_6);
	if (status == JNI_EDETACHED) {
		int new_status = g_javaVM->AttachCurrentThread(&env, nullptr);
		if( new_status != JNI_OK) {
			return;
		}
	}

	jmethodID callback = env->GetMethodID(classz, "ShowKeyboard", "()V");
	if (callback == nullptr) {
		return;
	}

	env->CallVoidMethod(classObject,callback);
	if (status == JNI_EDETACHED) {
		g_javaVM->DetachCurrentThread();
	}
	return;
}

bool registerMobileSurfaceViewNatives(JNIEnv *env)
{
	jclass k = env->FindClass(JPATH_ANDROID_MOBILE_SURFACE_VIEW);
	if (k == NULL) {
		LOGE("Error loading class %s", JPATH_ANDROID_MOBILE_SURFACE_VIEW);
		return false;
	}

	JNINativeMethod	methods[] = {
		{"create", "(Lcom/techsoft3d/hps/virtual_training/AndroidMobileSurfaceView;I)J", (void*)create},
		{"bind", "(JLjava/lang/Object;Ljava/lang/Object;)Z", (void*)bind},
		{"release", "(JI)V", (void*)release},
		{"refresh", "(J)V", (void*)refresh},
		{"touchDown", "(JI[I[I[J)V", (void*)touchDown},
		{"touchMove", "(JI[I[I[J)V", (void*)touchMove},
		{"touchUp", "(JI[I[I[J)V", (void*)touchUp},
		{"touchesCancel", "(J)V", (void*)touchesCancel},
		{"singleTap", "(JII)V", (void*)singleTap},
		{"doubleTap", "(JIIJ)V", (void*)doubleTap},
		{"onTextInputJS", "(JLjava/lang/String;)V", (void*)onTextInputJS},
		{"onKeyboardHiddenJ", "(J)V", (void*)onKeyboardHiddenJ},
	};
	const size_t	count = sizeof(methods) / sizeof(methods[0]);

	if (env->RegisterNatives(k, methods, count) != JNI_OK) {
		LOGE("Error calling RegisterNatives");
		return false;
	}

	return true;
}

JNIHelpers::ShowKeyboardHandler::ShowKeyboardHandler()
{
}

JNIHelpers::ShowKeyboardHandler::~ShowKeyboardHandler()
{
	Shutdown();
}

HPS::EventHandler::HandleResult JNIHelpers::ShowKeyboardHandler::Handle(HPS::Event const * in_event)
{
	onShowKeyboard();
	return HPS::EventHandler::HandleResult::Handled;
}

void ShowPerformanceTestResult(float fps)
{
	JNIEnv *env = nullptr;
	int status = g_javaVM->GetEnv((void **)&env, JNI_VERSION_1_6);
	if (status == JNI_EDETACHED) {
		int new_status = g_javaVM->AttachCurrentThread(&env, nullptr);
		if( new_status != JNI_OK) {
			return;
		}
	}

	jmethodID callback = env->GetMethodID(classz, "ShowPerformanceTestResult", "(F)V");
	if (callback == nullptr) {
		return;
	}

	env->CallVoidMethod(classObject,callback, fps);
	if (status == JNI_EDETACHED) {
		g_javaVM->DetachCurrentThread();
	}
	return;
}

