
#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <map>
#include "UserMobileSurface.h"

#define  LOG_TAG    "AndroidSandbox"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

bool registerMobileSurfaceViewNatives(JNIEnv *env);
bool registerAndroidUserMobileSurfaceViewNatives(JNIEnv *env);
bool registerMobileAppNatives(JNIEnv *env);

JavaVM *g_javaVM;

static std::map<int, UserMobileSurface *> g_surfaces;

// Users must implement createMobileSurface() to return a pointer to their derived MobileSurface
// Only one surface is created is created in the sandbox apps.
MobileSurface *createMobileSurface(int guiSurfaceId)
{
	if (g_surfaces.count(guiSurfaceId) == 0)
	{
		auto surface = new UserMobileSurface();
		g_surfaces.insert(std::make_pair(guiSurfaceId, surface));
		return surface;
	}

	return g_surfaces[guiSurfaceId];
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
	JNIEnv *env = NULL;
	if ((vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) || (env == NULL)) {
		LOGE("Error calling GetEnv");
		return -1;
	}

	g_javaVM = vm;

	if (!registerMobileSurfaceViewNatives(env))
		return -1;

	if (!registerAndroidUserMobileSurfaceViewNatives(env))
		return -1;

	if (!registerMobileAppNatives(env))
		return -1;

	return JNI_VERSION_1_6;
}

