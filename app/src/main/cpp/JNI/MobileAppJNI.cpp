
// Auto-generated file

#include "MobileApp.h"

#include <jni.h>
#include <android/log.h>
#include <stdio.h>

#include "jpaths.h"

#define  JNI_LOG_TAG    "MobileApp"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,JNI_LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,JNI_LOG_TAG,__VA_ARGS__)

#include "JNIHelpers.h"

static void shutdownV(JNIEnv *env, jclass cobj)
{
	
	MobileApp::inst().shutdown();
	
}


static void setLibraryDirectoryS(JNIEnv *env, jclass cobj, jstring libraryDir)
{
	JNIHelpers::String clibraryDir(env, libraryDir);
	MobileApp::inst().setLibraryDirectory(clibraryDir.str());
	
}


static void setFontDirectoryS(JNIEnv *env, jclass cobj, jstring fontDir)
{
	JNIHelpers::String cfontDir(env, fontDir);
	MobileApp::inst().setFontDirectory(cfontDir.str());
	
}


static void setMaterialsDirectoryS(JNIEnv *env, jclass cobj, jstring materialsDir)
{
	JNIHelpers::String cmaterialsDir(env, materialsDir);
	MobileApp::inst().setMaterialsDirectory(cmaterialsDir.str());
	
}



bool registerMobileAppNatives(JNIEnv *env)
{
	jclass k = env->FindClass(JPATH_MOBILE_APP);
	if (k == NULL) {
		LOGE("Error loading class %s", JPATH_MOBILE_APP);
		return false;
	}

	JNINativeMethod	methods[] = {
		{"shutdownV", "()V", (void*)shutdownV},
		{"setLibraryDirectoryS", "(Ljava/lang/String;)V", (void*)setLibraryDirectoryS},
		{"setFontDirectoryS", "(Ljava/lang/String;)V", (void*)setFontDirectoryS},
		{"setMaterialsDirectoryS", "(Ljava/lang/String;)V", (void*)setMaterialsDirectoryS},
	};
	const size_t	count = sizeof(methods) / sizeof(methods[0]);

	if (env->RegisterNatives(k, methods, count) != JNI_OK) {
		LOGE("Error calling RegisterNatives");
		return false;
	}

	return true;
}

