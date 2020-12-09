#pragma once

#ifndef dprintf
	#if TARGET_OS_ANDROID
		#include <android/log.h>

		#define LOG_TAG "SandboxApp"
		#define dprintf(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))
		#define eprintf(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))
		#define wprintf(...) ((void)__android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__))
	#else
		#include <stdio.h>
		#define dprintf(...) printf(__VA_ARGS__)
        #define eprintf(...) printf(__VA_ARGS__)
        #define wprintf(...) printf(__VA_ARGS__)
	#endif
#endif

