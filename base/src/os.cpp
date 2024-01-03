/*
 * Copyright 2010-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/base/blob/master/LICENSE
 */

#include <base/string.h>
#include <base/os.h>
#include <base/uint32_t.h>

#if BASE_CRT_MSVC
#	include <direct.h>
#else
#	include <unistd.h>
#endif // BASE_CRT_MSVC

#if BASE_PLATFORM_WINDOWS || BASE_PLATFORM_WINRT
#	ifndef WIN32_LEAN_AND_MEAN
#		define WIN32_LEAN_AND_MEAN
#	endif // WIN32_LEAN_AND_MEAN
#	include <windows.h>
#	include <psapi.h>
#elif  BASE_PLATFORM_ANDROID    \
	|| BASE_PLATFORM_BSD        \
	|| BASE_PLATFORM_EMSCRIPTEN \
	|| BASE_PLATFORM_HAIKU      \
	|| BASE_PLATFORM_HURD       \
	|| BASE_PLATFORM_IOS        \
	|| BASE_PLATFORM_LINUX      \
	|| BASE_PLATFORM_NX         \
	|| BASE_PLATFORM_OSX        \
	|| BASE_PLATFORM_PS4        \
	|| BASE_PLATFORM_RPI
#	include <sched.h> // sched_yield
#	if BASE_PLATFORM_BSD       \
	|| BASE_PLATFORM_HAIKU     \
	|| BASE_PLATFORM_IOS       \
	|| BASE_PLATFORM_OSX       \
	|| BASE_PLATFORM_PS4
#		include <pthread.h> // mach_port_t
#	endif // BASE_PLATFORM_*

#	include <time.h> // nanosleep
#	if !BASE_PLATFORM_PS4
#		include <dlfcn.h> // dlopen, dlclose, dlsym
#	endif // !BASE_PLATFORM_PS4

#	if BASE_PLATFORM_ANDROID
#		include <malloc.h> // mallinfo
#	elif   BASE_PLATFORM_LINUX     \
		|| BASE_PLATFORM_RPI
#		include <stdio.h>  // fopen
#		include <unistd.h> // syscall
#		include <sys/syscall.h>
#	elif   BASE_PLATFORM_HAIKU
#		include <stdio.h>  // fopen
#		include <unistd.h> // syscall
#	elif BASE_PLATFORM_OSX
#		include <mach/mach.h> // mach_task_basic_info
#	elif BASE_PLATFORM_HURD
#		include <stdio.h>           // fopen
#		include <pthread/pthread.h> // pthread_self
#	elif BASE_PLATFORM_ANDROID
#		include "debug.h" // getTid is not implemented...
#	endif // BASE_PLATFORM_ANDROID
#endif // BASE_PLATFORM_

namespace base
{
	void sleep(uint32_t _ms)
	{
#if BASE_PLATFORM_WINDOWS
		::Sleep(_ms);
#elif  BASE_PLATFORM_XBOXONE \
	|| BASE_PLATFORM_WINRT   \
	|| BASE_CRT_NONE
		BASE_UNUSED(_ms);
		debugOutput("sleep is not implemented"); debugBreak();
#else
		timespec req = { (time_t)_ms/1000, (long)( (_ms%1000)*1000000) };
		timespec rem = { 0, 0 };
		::nanosleep(&req, &rem);
#endif // BASE_PLATFORM_
	}

	void yield()
	{
#if BASE_PLATFORM_WINDOWS
		::SwitchToThread();
#elif  BASE_PLATFORM_XBOXONE \
	|| BASE_PLATFORM_WINRT   \
	|| BASE_CRT_NONE
		debugOutput("yield is not implemented"); debugBreak();
#else
		::sched_yield();
#endif // BASE_PLATFORM_
	}

	uint32_t getTid()
	{
#if BASE_PLATFORM_WINDOWS
		return ::GetCurrentThreadId();
#elif  BASE_PLATFORM_LINUX \
	|| BASE_PLATFORM_RPI
		return (pid_t)::syscall(SYS_gettid);
#elif  BASE_PLATFORM_IOS \
	|| BASE_PLATFORM_OSX
		return (mach_port_t)::pthread_mach_thread_np(pthread_self() );
#elif BASE_PLATFORM_BSD
		return *(uint32_t*)::pthread_self();
#elif BASE_PLATFORM_HURD
		return (pthread_t)::pthread_self();
#else
		debugOutput("getTid is not implemented"); debugBreak();
		return 0;
#endif // BASE_PLATFORM_
	}

	size_t getProcessMemoryUsed()
	{
#if BASE_PLATFORM_ANDROID
		struct mallinfo mi = mallinfo();
		return mi.uordblks;
#elif  BASE_PLATFORM_LINUX \
	|| BASE_PLATFORM_HURD
		FILE* file = fopen("/proc/self/statm", "r");
		if (NULL == file)
		{
			return 0;
		}

		long pages = 0;
		int items = fscanf(file, "%*s%ld", &pages);
		fclose(file);
		return 1 == items
			? pages * sysconf(_SC_PAGESIZE)
			: 0
			;
#elif BASE_PLATFORM_OSX
#	if defined(MACH_TASK_BASIC_INFO)
		mach_task_basic_info info;
		mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;

		int const result = task_info(mach_task_self()
				, MACH_TASK_BASIC_INFO
				, (task_info_t)&info
				, &infoCount
				);
#	else
		task_basic_info info;
		mach_msg_type_number_t infoCount = TASK_BASIC_INFO_COUNT;

		int const result = task_info(mach_task_self()
				, TASK_BASIC_INFO
				, (task_info_t)&info
				, &infoCount
				);
#	endif // defined(MACH_TASK_BASIC_INFO)
		if (KERN_SUCCESS != result)
		{
			return 0;
		}

		return info.resident_size;
#elif BASE_PLATFORM_WINDOWS
		PROCESS_MEMORY_COUNTERS pmc;
		GetProcessMemoryInfo(GetCurrentProcess()
			, &pmc
			, sizeof(pmc)
			);
		return pmc.WorkingSetSize;
#else
		return 0;
#endif // BASE_PLATFORM_*
	}

	void* dlopen(const FilePath& _filePath)
	{
#if BASE_PLATFORM_WINDOWS
		return (void*)::LoadLibraryA(_filePath.getCPtr() );
#elif  BASE_PLATFORM_EMSCRIPTEN \
	|| BASE_PLATFORM_PS4        \
	|| BASE_PLATFORM_XBOXONE    \
	|| BASE_PLATFORM_WINRT      \
	|| BASE_CRT_NONE
		BASE_UNUSED(_filePath);
		return NULL;
#else
		void* so = ::dlopen(_filePath.getCPtr(), RTLD_LOCAL|RTLD_LAZY);
		BASE_WARN(NULL != so, "dlopen failed: \"%s\".", ::dlerror() );
		return so;
#endif // BASE_PLATFORM_
	}

	void dlclose(void* _handle)
	{
		if (NULL == _handle)
		{
			return;
		}

#if BASE_PLATFORM_WINDOWS
		::FreeLibrary( (HMODULE)_handle);
#elif  BASE_PLATFORM_EMSCRIPTEN \
	|| BASE_PLATFORM_PS4        \
	|| BASE_PLATFORM_XBOXONE    \
	|| BASE_PLATFORM_WINRT      \
	|| BASE_CRT_NONE
		BASE_UNUSED(_handle);
#else
		::dlclose(_handle);
#endif // BASE_PLATFORM_
	}

	void* dlsym(void* _handle, const StringView& _symbol)
	{
		const int32_t symbolMax = _symbol.getLength()+1;
		char* symbol = (char*)alloca(symbolMax);
		strCopy(symbol, symbolMax, _symbol);

#if BASE_PLATFORM_WINDOWS
		return (void*)::GetProcAddress( (HMODULE)_handle, symbol);
#elif  BASE_PLATFORM_EMSCRIPTEN \
	|| BASE_PLATFORM_PS4        \
	|| BASE_PLATFORM_XBOXONE    \
	|| BASE_PLATFORM_WINRT      \
	|| BASE_CRT_NONE
		BASE_UNUSED(_handle, symbol);
		return NULL;
#else
		return ::dlsym(_handle, symbol);
#endif // BASE_PLATFORM_
	}

	bool getEnv(char* _out, uint32_t* _inOutSize, const StringView& _name)
	{
		const int32_t nameMax = _name.getLength()+1;
		char* name = (char*)alloca(nameMax);
		strCopy(name, nameMax, _name);

#if BASE_PLATFORM_WINDOWS
		DWORD len = ::GetEnvironmentVariableA(name, _out, *_inOutSize);
		bool result = len != 0 && len < *_inOutSize;
		*_inOutSize = len;
		return result;
#elif  BASE_PLATFORM_EMSCRIPTEN \
	|| BASE_PLATFORM_PS4        \
	|| BASE_PLATFORM_XBOXONE    \
	|| BASE_PLATFORM_WINRT      \
	|| BASE_CRT_NONE
		BASE_UNUSED(name, _out, _inOutSize);
		return false;
#else
		const char* ptr = ::getenv(name);
		uint32_t len = 0;
		bool result = false;
		if (NULL != ptr)
		{
			len = (uint32_t)strLen(ptr);

			result = len != 0 && len < *_inOutSize;
			if (len < *_inOutSize)
			{
				strCopy(_out, *_inOutSize, ptr);
			}
		}

		*_inOutSize = len;
		return result;
#endif // BASE_PLATFORM_
	}

	void setEnv(const StringView& _name, const StringView& _value)
	{
		const int32_t nameMax = _name.getLength()+1;
		char* name = (char*)alloca(nameMax);
		strCopy(name, nameMax, _name);

		char* value = NULL;
		if (!_value.isEmpty() )
		{
			int32_t valueMax = _value.getLength()+1;
			value = (char*)alloca(valueMax);
			strCopy(value, valueMax, _value);
		}

#if BASE_PLATFORM_WINDOWS
		::SetEnvironmentVariableA(name, value);
#elif  BASE_PLATFORM_EMSCRIPTEN \
	|| BASE_PLATFORM_PS4        \
	|| BASE_PLATFORM_XBOXONE    \
	|| BASE_PLATFORM_WINRT      \
	|| BASE_CRT_NONE
		BASE_UNUSED(name, value);
#else
		if (NULL != value)
		{
			::setenv(name, value, 1);
		}
		else
		{
			::unsetenv(name);
		}
#endif // BASE_PLATFORM_
	}

	int chdir(const char* _path)
	{
#if BASE_PLATFORM_PS4     \
 || BASE_PLATFORM_XBOXONE \
 || BASE_PLATFORM_WINRT   \
 || BASE_CRT_NONE
		BASE_UNUSED(_path);
		return -1;
#elif BASE_CRT_MSVC
		return ::_chdir(_path);
#else
		return ::chdir(_path);
#endif // BASE_COMPILER_
	}

	void* exec(const char* const* _argv)
	{
#if BASE_PLATFORM_LINUX \
 || BASE_PLATFORM_HURD
		pid_t pid = fork();

		if (0 == pid)
		{
			int result = execvp(_argv[0], const_cast<char *const*>(&_argv[1]) );
			BASE_UNUSED(result);
			return NULL;
		}

		return (void*)uintptr_t(pid);
#elif BASE_PLATFORM_WINDOWS
		STARTUPINFOA si;
		memSet(&si, 0, sizeof(STARTUPINFOA) );
		si.cb = sizeof(STARTUPINFOA);

		PROCESS_INFORMATION pi;
		memSet(&pi, 0, sizeof(PROCESS_INFORMATION) );

		int32_t total = 0;
		for (uint32_t ii = 0; NULL != _argv[ii]; ++ii)
		{
			total += (int32_t)strLen(_argv[ii]) + 1;
		}

		char* temp = (char*)alloca(total);
		int32_t len = 0;
		for(uint32_t ii = 0; NULL != _argv[ii]; ++ii)
		{
			len += snprintf(&temp[len], uint32_imax(0, total-len)
				, "%s "
				, _argv[ii]
				);
		}

		bool ok = !!CreateProcessA(_argv[0]
			, temp
			, NULL
			, NULL
			, false
			, 0
			, NULL
			, NULL
			, &si
			, &pi
			);
		if (ok)
		{
			return pi.hProcess;
		}

		return NULL;
#else
		BASE_UNUSED(_argv);
		return NULL;
#endif // BASE_PLATFORM_LINUX || BASE_PLATFORM_HURD
	}

	void exit(int32_t _exitCode)
	{
		::exit(_exitCode);
	}

} // namespace base
