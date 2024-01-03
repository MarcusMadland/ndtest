/*
 * Copyright 2010-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/base/blob/master/LICENSE
 */

#include <base/timer.h>

#if BASE_CRT_NONE
#	include "crt0.h"
#elif BASE_PLATFORM_ANDROID
#	include <time.h> // clock, clock_gettime
#elif BASE_PLATFORM_EMSCRIPTEN
#	include <emscripten.h>
#elif BASE_PLATFORM_WINDOWS || BASE_PLATFORM_XBOXONE || BASE_PLATFORM_WINRT
#	ifndef WIN32_LEAN_AND_MEAN
#		define WIN32_LEAN_AND_MEAN
#	endif // WIN32_LEAN_AND_MEAN
#	include <windows.h>
#else
#	include <sys/time.h> // gettimeofday
#endif // BASE_PLATFORM_

namespace base
{
	int64_t getHPCounter()
	{
#if    BASE_CRT_NONE
		int64_t i64 = crt0::getHPCounter();
#elif  BASE_PLATFORM_WINDOWS \
	|| BASE_PLATFORM_XBOXONE \
	|| BASE_PLATFORM_WINRT
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		int64_t i64 = li.QuadPart;
#elif BASE_PLATFORM_ANDROID
		struct timespec now;
		clock_gettime(CLOCK_MONOTONIC, &now);
		int64_t i64 = now.tv_sec*INT64_C(1000000000) + now.tv_nsec;
#elif BASE_PLATFORM_EMSCRIPTEN
		int64_t i64 = int64_t(1000.0f * emscripten_get_now() );
#elif !BASE_PLATFORM_NONE
		struct timeval now;
		gettimeofday(&now, 0);
		int64_t i64 = now.tv_sec*INT64_C(1000000) + now.tv_usec;
#else
		BASE_ASSERT(false, "Not implemented!");
		int64_t i64 = UINT64_MAX;
#endif // BASE_PLATFORM_
		return i64;
	}

	int64_t getHPFrequency()
	{
#if    BASE_CRT_NONE
		return INT64_C(1000000000);
#elif  BASE_PLATFORM_WINDOWS \
	|| BASE_PLATFORM_XBOXONE \
	|| BASE_PLATFORM_WINRT
		LARGE_INTEGER li;
		QueryPerformanceFrequency(&li);
		return li.QuadPart;
#elif BASE_PLATFORM_ANDROID
		return INT64_C(1000000000);
#elif BASE_PLATFORM_EMSCRIPTEN
		return INT64_C(1000000);
#else
		return INT64_C(1000000);
#endif // BASE_PLATFORM_
	}

} // namespace base
