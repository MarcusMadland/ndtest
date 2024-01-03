/*
 * Copyright 2010-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/base/blob/master/LICENSE
 */

#include <base/debug.h>
#include <base/file.h>
#include <base/math.h>
#include <base/sort.h>
#include <base/timer.h>

#if BASE_CRT_NONE

#include "crt0.h"

#define NOT_IMPLEMENTED() \
	{ base::debugPrintf("crtnone: %s not implemented\n", BASE_FUNCTION); abort(); }

extern "C" void* memcpy(void* _dst, const void* _src, size_t _numBytes)
{
	base::memCopy(_dst, _src, _numBytes);
	return _dst;
}

extern "C" void* memmove(void* _dst, const void* _src, size_t _numBytes)
{
	base::memMove(_dst, _src, _numBytes);
	return _dst;
}

extern "C" void* memset(void* _dst, int _ch, size_t _numBytes)
{
	base::memSet(_dst, uint8_t(_ch), _numBytes);
	return _dst;
}

#if !BASE_PLATFORM_NONE

typedef int64_t off64_t;
typedef int32_t pid_t;

extern "C" int32_t memcmp(const void* _lhs, const void* _rhs, size_t _numBytes)
{
	return base::memCmp(_lhs, _rhs, _numBytes);
}

extern "C" size_t strlen(const char* _str)
{
	return base::strLen(_str);
}

extern "C" size_t strnlen(const char* _str, size_t _max)
{
	return base::strLen(_str, _max);
}

extern "C" void* strcpy(char* _dst, const char* _src)
{
	base::strCopy(_dst, INT32_MAX, _src, INT32_MAX);
	return _dst;
}

extern "C" void* strncpy(char* _dst, const char* _src, size_t _num)
{
	base::strCopy(_dst, INT32_MAX, _src, _num);
	return _dst;
}

extern "C" char* strcat(char* _dst, const char* _src)
{
	base::strCat(_dst, INT32_MAX, _src, INT32_MAX);
	return _dst;
}

extern "C" const char* strchr(const char* _str, int _ch)
{
	return base::strFind(_str, _ch).getPtr();
}

extern "C" int32_t strcmp(const char* _lhs, const char* _rhs)
{
	return base::strCmp(_lhs, _rhs);
}

extern "C" int32_t strncmp(const char* _lhs, const char* _rhs, size_t _max)
{
	return base::strCmp(_lhs, _rhs, _max);
}

extern "C" int32_t strcasecmp(const char* _lhs, const char* _rhs)
{
	return base::strCmpI(_lhs, _rhs);
}

extern "C" const char* strstr(const char* _str, const char* _find)
{
	return base::strFind(_str, _find).getPtr();
}

extern "C" void qsort(void* _base, size_t _num, size_t _size, base::ComparisonFn _fn)
{
	BASE_ASSERT(_num <= UINT32_MAX && _size <= UINT32_MAX, "");
	return base::quickSort(_base, _num, _size, _fn);
}

extern "C" int isprint(int _ch)
{
	return base::isPrint(_ch);
}

extern "C" int toupper(int _ch)
{
	return base::toUpper(_ch);
}

extern "C" size_t mbstowcs(wchar_t* _dst, const char* _src, size_t _max)
{
	BASE_UNUSED(_dst, _src, _max);
	return 0;
}

extern "C" char* strdup(const char* _src)
{
	uint32_t size = base::strLen(_src)+1;
	char* dup = (char*)malloc(size);
	base::strCopy(dup, size, _src);
	return dup;
}

extern "C" long int strtol(const char* _str, char** _end, int _base)
{
	BASE_UNUSED(_str, _end, _base);
	NOT_IMPLEMENTED();
	return -1;
}

extern "C" int abs(int _value)
{
	return _value >= 0 ? _value : -_value;
}

extern "C" float fabsf(float _x)
{
	return base::abs(_x);
}

extern "C" double fabs(double _x)
{
	return base::abs(_x);
}

extern "C" double ldexp(double _x, int _exp)
{
	return base::ldexp(float(_x), _exp);
}

extern "C" float expf(float _x)
{
	return base::exp(_x);
}

extern "C" float logf(float _x)
{
	return base::log(_x);
}

extern "C" float log10f(float _x)
{
	BASE_UNUSED(_x);
	return 0.0f;
}

extern "C" float powf(float _x, float _y)
{
	return base::pow(_x, _y);
}

extern "C" double pow(double _x, float _y)
{
	return base::pow(_x, _y);
}

extern "C" float sinf(float _x)
{
	return base::sin(_x);
}

extern "C" float cosf(float _x)
{
	return base::cos(_x);
}

extern "C" float tanf(float _x)
{
	return base::tan(_x);
}

extern "C" float atan2f(float _y, float _x)
{
	return base::atan2(_y, _x);
}

extern "C" float sqrtf(float _x)
{
	return base::sqrt(_x);
}

extern "C" double sqrt(double _x)
{
	return base::sqrt(_x);
}

extern "C" float ceilf(float _x)
{
	return base::ceil(_x);
}

extern "C" double ceil(double _x)
{
	return base::ceil(_x);
}

extern "C" float floorf(float _x)
{
	return base::floor(_x);
}

extern "C" double floor(double _x)
{
	return base::floor(_x);
}

extern "C" float acosf(float _x)
{
	return base::acos(_x);
}

extern "C" float fmodf(float _numer, float _denom)
{
	return base::mod(_numer, _denom);
}

extern "C" int atoi(const char* _str)
{
	int32_t result = 0;
	base::fromString(&result, _str);
	return result;
}

extern "C" double atof(const char* _str)
{
	double result = 0.0;
	base::fromString(&result, _str);
	return result;
}

extern "C" struct DIR* opendir(const char* _dirname)
{
	BASE_UNUSED(_dirname);
//	NOT_IMPLEMENTED();
	return NULL;
}

extern "C" struct dirent* readdir(struct DIR* _dirp)
{
	BASE_UNUSED(_dirp);
	NOT_IMPLEMENTED();
	return NULL;
}

extern "C" int closedir(struct DIR* _dirp)
{
	BASE_UNUSED(_dirp);
	NOT_IMPLEMENTED();
	return 0;
}

extern "C" int vsnprintf(char* _out, size_t _max, const char* _format, va_list _argList)
{
	return base::vsnprintf(_out, _max, _format, _argList);
}

extern "C" int sprintf(char* _out, const char* _format, ...)
{
	va_list argList;
	va_start(argList, _format);
	int32_t len = base::vsnprintf(_out, INT32_MAX, _format, argList);
	va_end(argList);
	return len;
}

extern "C" int snprintf(char* _out, size_t _max, const char* _format, ...)
{
	va_list argList;
	va_start(argList, _format);
	int32_t len = base::vsnprintf(_out, _max, _format, argList);
	va_end(argList);
	return len;
}

extern "C" int printf(const char* _format, ...)
{
	va_list argList;
	va_start(argList, _format);
	base::WriterI* writer = base::getStdOut();
	base::Error err;
	int32_t len = base::write(writer, &err, _format, argList);
	va_end(argList);
	return len;
}

struct FILE
{
};

extern "C" int fprintf(FILE* _stream, const char* _format, ...)
{
	BASE_UNUSED(_stream, _format);
	return -1;
}

extern "C" int vfprintf(FILE* _stream, const char* _format, va_list _argList)
{
	BASE_UNUSED(_stream, _format, _argList);
	return -1;
}

extern "C" int sscanf(const char* _str, const char* _format, ...)
{
	BASE_UNUSED(_str, _format);
	return -1;
}

extern "C" int fscanf(FILE* _stream, const char* _format, ...)
{
	BASE_UNUSED(_stream, _format);
	return -1;
}

extern "C" int __isoc99_fscanf(FILE* _stream, const char* _format, ...)
{
	BASE_UNUSED(_stream, _format);
	return -1;
}

FILE * stdout;

extern "C" FILE* fopen(const char* _filename, const char* _mode)
{
	BASE_UNUSED(_filename, _mode);
	base::debugPrintf("fopen(\"%s\", \"%s\");\n", _filename, _mode);
//	NOT_IMPLEMENTED();
	return NULL;
}

extern "C" int fclose(FILE* _stream)
{
	BASE_UNUSED(_stream);
	base::debugPrintf("fclose(%p);\n", _stream);
//	NOT_IMPLEMENTED();
	return 0;
}

extern "C" size_t fread(void* _ptr, size_t _size, size_t _count, FILE* _stream)
{
	BASE_UNUSED(_ptr, _size, _count, _stream);
	NOT_IMPLEMENTED();
	return -1;
}

extern "C" size_t fwrite(const void* _ptr, size_t _size, size_t _count, FILE* _stream)
{
	BASE_UNUSED(_ptr, _size, _count, _stream);
	NOT_IMPLEMENTED();
	return -1;
}

extern "C" int fseek(FILE* _stream, long int _offset, int _origin)
{
	BASE_UNUSED(_stream, _offset, _origin);
	NOT_IMPLEMENTED();
	return -1;
}

extern "C" int fseeko64(FILE* _stream, off64_t _offset, int _whence)
{
	BASE_UNUSED(_stream, _offset, _whence);
	NOT_IMPLEMENTED();
	return -1;
}

extern "C" long int ftell(FILE* _stream)
{
	BASE_UNUSED(_stream);
	NOT_IMPLEMENTED();
	return -1;
}

extern "C" off64_t ftello64(FILE* _stream)
{
	BASE_UNUSED(_stream);
	NOT_IMPLEMENTED();
	return -1;
}

extern "C" int feof(FILE* _stream)
{
	BASE_UNUSED(_stream);
	NOT_IMPLEMENTED();
	return -1;
}

extern "C" int ferror(FILE* _stream)
{
	BASE_UNUSED(_stream);
	NOT_IMPLEMENTED();
	return -1;
}

extern "C" FILE* popen(const char* _command, const char* _type)
{
	BASE_UNUSED(_command, _type);
	NOT_IMPLEMENTED();
	return NULL;
}

extern "C" int pclose(FILE* _stream)
{
	BASE_UNUSED(_stream);
	NOT_IMPLEMENTED();
	return -1;
}

extern "C" int execvp(const char* _file, char* const _argv[])
{
	BASE_UNUSED(_file, _argv);
	NOT_IMPLEMENTED();
	return -1;
}

typedef int32_t clockid_t;

inline void toTimespecNs(timespec& _ts, int64_t _nsecs)
{
	_ts.tv_sec  = _nsecs/INT64_C(1000000000);
	_ts.tv_nsec = _nsecs%INT64_C(1000000000);
}

extern "C" int clock_gettime(clockid_t _clock, struct timespec* _ts)
{
	BASE_UNUSED(_clock);
	int64_t now = crt0::getHPCounter();
	toTimespecNs(*_ts, now);
	return 0;
}

extern "C" long syscall(long _num, ...)
{
	va_list argList;
	va_start(argList, _num);

	long result = -1;

	switch (_num)
	{
	case 39:
		result = crt0::processGetId();
		break;

	case 228:
		{
			clockid_t arg0 = va_arg(argList, clockid_t);
			timespec* arg1 = va_arg(argList, timespec*);
			result = clock_gettime(arg0, arg1);
		}
		break;

	default:
		base::debugPrintf("? syscall %d\n", _num);
		break;
	}

	va_end(argList);

	return result;
}

extern "C" long sysconf(int name)
{
	BASE_UNUSED(name);
	NOT_IMPLEMENTED();
	return -1;
}

extern "C" pid_t fork(void)
{
	NOT_IMPLEMENTED();
	return -1;
}

extern "C" int sched_yield(void)
{
	NOT_IMPLEMENTED();
	return -1;
}

extern "C" int prctl(int _option, unsigned long _arg2, unsigned long _arg3, unsigned long _arg4, unsigned long _arg5)
{
	BASE_UNUSED(_option, _arg2, _arg3, _arg4, _arg5);
	NOT_IMPLEMENTED();
	return -1;
}

extern "C" int chdir(const char* _path)
{
	BASE_UNUSED(_path);
	base::debugPrintf("chdir(%s) not implemented!\n", _path);
	return -1;
}

extern "C" char* getcwd(char* _buf, size_t _size)
{
	BASE_UNUSED(_buf, _size);
	NOT_IMPLEMENTED();
	return NULL;
}

extern "C" char* getenv(const char* _name)
{
	return const_cast<char*>(crt0::getEnv(_name) );
}

extern "C" int setenv(const char* _name, const char* _value, int _overwrite)
{
	BASE_UNUSED(_name, _value, _overwrite);
	base::debugPrintf("setenv(%s, %s, %d) not implemented!\n", _name, _value, _overwrite);
	return -1;
}

extern "C" int unsetenv(const char* _name)
{
	BASE_UNUSED(_name);
	base::debugPrintf("unsetenv(%s) not implemented!\n", _name);
	return -1;
}

#if 0
struct timeval
{
	time_t tv_sec;
	suseconds_t tv_usec;
};

struct timespec
{
	time_t tv_sec;
	long tv_nsec;
};
#endif //

extern "C" int gettimeofday(struct timeval* _tv, struct timezone* _tz)
{
	BASE_UNUSED(_tz);

	timespec ts;

	if (NULL == _tv)
	{
		return 0;
	}

	clock_gettime(0 /*CLOCK_REALTIME*/, &ts);
	_tv->tv_sec = ts.tv_sec;
	_tv->tv_usec = (int)ts.tv_nsec / 1000;
	return 0;
}

typedef int64_t time_t;

extern "C" time_t time(time_t* _arg)
{
	timespec ts;
	clock_gettime(0 /*CLOCK_REALTIME*/, &ts);
	time_t result = ts.tv_sec;

	if (NULL != _arg)
	{
		*_arg = result;
	}

	return result;
}

extern "C" void* realloc(void* _ptr, size_t _size)
{
	return crt0::realloc(_ptr, _size);
}

extern "C" void* malloc(size_t _size)
{
	return crt0::realloc(NULL, _size);
}

extern "C" void free(void* _ptr)
{
	crt0::realloc(_ptr, 0);
}

extern "C" void exit(int _exitCode)
{
	crt0::exit(_exitCode);
}

#endif // BASE_PLATFORM_*

extern "C" void abort(void)
{
	base::debugPrintf("crtnone: abort called!\n");
	crt0::exit(base::kExitFailure);
}

extern "C" void __assert_fail(const char* _assertion, const char* _file, uint32_t _line, const char* _function)
{
	BASE_UNUSED(_assertion, _file, _line, _function);
	abort();
}

void* __dso_handle = (void*)&__dso_handle;

void operator delete(void*)
{
}

void operator delete(void*, size_t)
{
}

extern "C" void __cxa_pure_virtual(void)
{
}

extern "C" int __cxa_atexit(void (*_dtorFn)(void*), void* _arg, void* _dsoHandle)
{
	BASE_UNUSED(_dtorFn, _arg, _dsoHandle);
	return 0;
}

extern "C" void __gxx_personality_v0(void)
{
}

extern "C" void _Unwind_Resume(void*)
{
}

extern "C" int __gcc_personality_v0(int _version, ...)
{
	BASE_UNUSED(_version);
	return 0;
}

namespace __cxxabiv1
{
	class __class_type_info
	{
	public:
		virtual ~__class_type_info();

		const char* m_name;
	};

	__class_type_info::~__class_type_info()
	{
	}

	class __si_class_type_info : public __class_type_info
	{
	public:
		virtual ~__si_class_type_info();
	};

	__si_class_type_info::~__si_class_type_info()
	{
	}

	class __vmi_class_type_info : public __class_type_info
	{
	public:
		virtual ~__vmi_class_type_info();
	};

	__vmi_class_type_info::~__vmi_class_type_info()
	{
	}

	__extension__ typedef int __guard __attribute__( (mode(__DI__) ) );

	extern "C" int __cxa_guard_acquire(__guard* _g)
	{
		return !*(char*)(_g);
	}

	extern "C" void __cxa_guard_release(__guard* _g)
	{
		*(char*)_g = 1;
	}

	extern "C" void __cxa_guard_abort(__guard* _g)
	{
		BASE_UNUSED(_g);
	}

} // namespace __cxxabiv1

#endif // BASE_CRT_NONE
