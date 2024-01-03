/*
 * Copyright 2010-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/base/blob/master/LICENSE
 */

#include <base/mutex.h>

#if BASE_CONFIG_SUPPORTS_THREADING

#if BASE_CRT_NONE
#	include <base/cpu.h>
#	include "crt0.h"
#elif  BASE_PLATFORM_ANDROID \
	|| BASE_PLATFORM_BSD     \
	|| BASE_PLATFORM_HAIKU   \
	|| BASE_PLATFORM_LINUX   \
	|| BASE_PLATFORM_IOS     \
	|| BASE_PLATFORM_OSX     \
	|| BASE_PLATFORM_PS4     \
	|| BASE_PLATFORM_RPI	   \
	|| BASE_PLATFORM_NX
#	include <pthread.h>
#elif  BASE_PLATFORM_WINDOWS \
	|| BASE_PLATFORM_WINRT   \
	|| BASE_PLATFORM_XBOXONE
#	ifndef WIN32_LEAN_AND_MEAN
#		define WIN32_LEAN_AND_MEAN
#	endif // WIN32_LEAN_AND_MEAN
#	include <windows.h>
#	include <errno.h>
#endif // BASE_PLATFORM_

namespace base
{
#if BASE_CRT_NONE
	struct State
	{
		enum Enum
		{
			Unlocked,
			Locked,
			Contested,
		};
	};

	Mutex::Mutex()
	{
		BASE_STATIC_ASSERT(sizeof(int32_t) <= sizeof(m_internal) );

		uint32_t* futex = (uint32_t*)m_internal;
		*futex = State::Unlocked;
	}

	Mutex::~Mutex()
	{
	}

	void Mutex::lock()
	{
		uint32_t* futex = (uint32_t*)m_internal;

		if (State::Unlocked == atomicCompareAndSwap<uint32_t>(futex, State::Unlocked, State::Locked) )
		{
			return;
		}

		while (State::Unlocked != atomicCompareAndSwap<uint32_t>(futex, State::Locked, State::Contested) )
		{
			crt0::futexWait(futex, State::Contested);
		}
	}

	void Mutex::unlock()
	{
		uint32_t* futex = (uint32_t*)m_internal;

		if (State::Contested == atomicCompareAndSwap<uint32_t>(futex, State::Locked, State::Unlocked) )
		{
			crt0::futexWake(futex, State::Locked);
		}
	}

#else

#	if BASE_PLATFORM_WINDOWS \
	|| BASE_PLATFORM_XBOXONE \
	|| BASE_PLATFORM_WINRT
	typedef CRITICAL_SECTION pthread_mutex_t;
	typedef unsigned pthread_mutexattr_t;

	inline int pthread_mutex_lock(pthread_mutex_t* _mutex)
	{
		EnterCriticalSection(_mutex);
		return 0;
	}

	inline int pthread_mutex_unlock(pthread_mutex_t* _mutex)
	{
		LeaveCriticalSection(_mutex);
		return 0;
	}

	inline int pthread_mutex_trylock(pthread_mutex_t* _mutex)
	{
		return TryEnterCriticalSection(_mutex) ? 0 : EBUSY;
	}

	inline int pthread_mutex_init(pthread_mutex_t* _mutex, pthread_mutexattr_t* /*_attr*/)
	{
#		if BASE_PLATFORM_WINRT
		InitializeCriticalSectionEx(_mutex, 4000, 0);   // docs recommend 4000 spincount as sane default
#		else
		InitializeCriticalSection(_mutex);
#		endif // BASE_PLATFORM_
		return 0;
	}

	inline int pthread_mutex_destroy(pthread_mutex_t* _mutex)
	{
		DeleteCriticalSection(_mutex);
		return 0;
	}
#	endif // BASE_PLATFORM_

	Mutex::Mutex()
	{
		BASE_STATIC_ASSERT(sizeof(pthread_mutex_t) <= sizeof(m_internal) );

		pthread_mutexattr_t attr;

#	if BASE_PLATFORM_WINDOWS \
	|| BASE_PLATFORM_XBOXONE \
	|| BASE_PLATFORM_WINRT
#	else
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
#	endif // BASE_PLATFORM_

		pthread_mutex_t* handle = (pthread_mutex_t*)m_internal;
		pthread_mutex_init(handle, &attr);
	}

	Mutex::~Mutex()
	{
		pthread_mutex_t* handle = (pthread_mutex_t*)m_internal;
		pthread_mutex_destroy(handle);
	}

	void Mutex::lock()
	{
		pthread_mutex_t* handle = (pthread_mutex_t*)m_internal;
		pthread_mutex_lock(handle);
	}

	void Mutex::unlock()
	{
		pthread_mutex_t* handle = (pthread_mutex_t*)m_internal;
		pthread_mutex_unlock(handle);
	}
#endif // BASE_CRT_NONE

} // namespace base

#else

namespace base
{
	Mutex::Mutex()
	{
	}

	Mutex::~Mutex()
	{
	}

	void Mutex::lock()
	{
	}

	void Mutex::unlock()
	{
	}

} // namespace base

#endif // BASE_CONFIG_SUPPORTS_THREADING
