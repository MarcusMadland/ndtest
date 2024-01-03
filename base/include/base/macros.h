/*
 * Copyright 2010-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/base/blob/master/LICENSE
 */

#ifndef BASE_H_HEADER_GUARD
#	error "Do not include macros.h directly #include <base/base.h> instead."
#endif // BASE_H_HEADER_GUARD

#ifndef BASE_MACROS_H_HEADER_GUARD
#define BASE_MACROS_H_HEADER_GUARD

///
#if BASE_COMPILER_MSVC
// Workaround MSVS bug...
#	define BASE_VA_ARGS_PASS(...) BASE_VA_ARGS_PASS_1_ __VA_ARGS__ BASE_VA_ARGS_PASS_2_
#	define BASE_VA_ARGS_PASS_1_ (
#	define BASE_VA_ARGS_PASS_2_ )
#else
#	define BASE_VA_ARGS_PASS(...) (__VA_ARGS__)
#endif // BASE_COMPILER_MSVC

#define BASE_VA_ARGS_COUNT(...) BASE_VA_ARGS_COUNT_ BASE_VA_ARGS_PASS(__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define BASE_VA_ARGS_COUNT_(_a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10, _a11, _a12, _a13, _a14, _a15, _a16, _last, ...) _last

///
#define BASE_MACRO_DISPATCHER(_func, ...) BASE_MACRO_DISPATCHER_1_(_func, BASE_VA_ARGS_COUNT(__VA_ARGS__) )
#define BASE_MACRO_DISPATCHER_1_(_func, _argCount) BASE_MACRO_DISPATCHER_2_(_func, _argCount)
#define BASE_MACRO_DISPATCHER_2_(_func, _argCount) BASE_CONCATENATE(_func, _argCount)

///
#define BASE_MAKEFOURCC(_a, _b, _c, _d) ( ( (uint32_t)(_a) | ( (uint32_t)(_b) << 8) | ( (uint32_t)(_c) << 16) | ( (uint32_t)(_d) << 24) ) )

///
#define BASE_STRINGIZE(_x) BASE_STRINGIZE_(_x)
#define BASE_STRINGIZE_(_x) #_x

///
#define BASE_CONCATENATE(_x, _y) BASE_CONCATENATE_(_x, _y)
#define BASE_CONCATENATE_(_x, _y) _x ## _y

///
#define BASE_FILE_LINE_LITERAL "" __FILE__ "(" BASE_STRINGIZE(__LINE__) "): "

///
#define BASE_ALIGNOF(_type) __alignof(_type)

#if defined(__has_feature)
#	define BASE_CLANG_HAS_FEATURE(_x) __has_feature(_x)
#else
#	define BASE_CLANG_HAS_FEATURE(_x) 0
#endif // defined(__has_feature)

#if defined(__has_extension)
#	define BASE_CLANG_HAS_EXTENSION(_x) __has_extension(_x)
#else
#	define BASE_CLANG_HAS_EXTENSION(_x) 0
#endif // defined(__has_extension)

#if BASE_COMPILER_GCC || BASE_COMPILER_CLANG
#	define BASE_ASSUME(_condition) BASE_MACRO_BLOCK_BEGIN if (!(_condition) ) { __builtin_unreachable(); } BASE_MACRO_BLOCK_END
#	define BASE_ALIGN_DECL(_align, _decl) _decl __attribute__( (aligned(_align) ) )
#	define BASE_ALLOW_UNUSED __attribute__( (unused) )
#	define BASE_FORCE_INLINE inline __attribute__( (__always_inline__) )
#	define BASE_FUNCTION __PRETTY_FUNCTION__
#	define BASE_LIKELY(_x)   __builtin_expect(!!(_x), 1)
#	define BASE_UNLIKELY(_x) __builtin_expect(!!(_x), 0)
#	define BASE_NO_INLINE   __attribute__( (noinline) )
#	define BASE_NO_RETURN   __attribute__( (noreturn) )
#	define BASE_CONST_FUNC  __attribute__( (pure) )
#	define BASE_UNREACHABLE __builtin_unreachable()

#	if BASE_COMPILER_GCC >= 70000
#		define BASE_FALLTHROUGH __attribute__( (fallthrough) )
#	else
#		define BASE_FALLTHROUGH BASE_NOOP()
#	endif // BASE_COMPILER_GCC >= 70000

#	define BASE_NO_VTABLE
#	define BASE_PRINTF_ARGS(_format, _args) __attribute__( (format(__printf__, _format, _args) ) )

#	if BASE_CLANG_HAS_FEATURE(cxx_thread_local) \
	|| (!BASE_PLATFORM_OSX && (BASE_COMPILER_GCC >= 40200) ) \
	|| (BASE_COMPILER_GCC >= 40500)
#		define BASE_THREAD_LOCAL __thread
#	endif // BASE_COMPILER_GCC

#	define BASE_ATTRIBUTE(_x) __attribute__( (_x) )

#	if BASE_CRT_MSVC
#		define __stdcall
#	endif // BASE_CRT_MSVC
#elif BASE_COMPILER_MSVC
#	define BASE_ASSUME(_condition) __assume(_condition)
#	define BASE_ALIGN_DECL(_align, _decl) __declspec(align(_align) ) _decl
#	define BASE_ALLOW_UNUSED
#	define BASE_FORCE_INLINE __forceinline
#	define BASE_FUNCTION __FUNCTION__
#	define BASE_LIKELY(_x)   (_x)
#	define BASE_UNLIKELY(_x) (_x)
#	define BASE_NO_INLINE __declspec(noinline)
#	define BASE_NO_RETURN
#	define BASE_CONST_FUNC  __declspec(noalias)
#	define BASE_UNREACHABLE __assume(false)
#	define BASE_FALLTHROUGH BASE_NOOP()
#	define BASE_NO_VTABLE __declspec(novtable)
#	define BASE_PRINTF_ARGS(_format, _args)
#	define BASE_THREAD_LOCAL __declspec(thread)
#	define BASE_ATTRIBUTE(_x)
#else
#	error "Unknown BASE_COMPILER_?"
#endif

/// The return value of the function is solely a function of the arguments.
///
#define BASE_CONSTEXPR_FUNC constexpr BASE_CONST_FUNC

///
#define BASE_STATIC_ASSERT(_condition, ...) static_assert(_condition, "" __VA_ARGS__)

///
#define BASE_ALIGN_DECL_16(_decl) BASE_ALIGN_DECL(16, _decl)
#define BASE_ALIGN_DECL_256(_decl) BASE_ALIGN_DECL(256, _decl)
#define BASE_ALIGN_DECL_CACHE_LINE(_decl) BASE_ALIGN_DECL(BASE_CACHE_LINE_SIZE, _decl)

///
#define BASE_MACRO_BLOCK_BEGIN for(;;) {
#define BASE_MACRO_BLOCK_END break; }
#define BASE_NOOP(...) BASE_MACRO_BLOCK_BEGIN BASE_MACRO_BLOCK_END

///
#define BASE_UNUSED_1(_a1)                                              \
	BASE_MACRO_BLOCK_BEGIN                                              \
		BASE_PRAGMA_DIAGNOSTIC_PUSH();                                  \
		/*BASE_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wuseless-cast");*/ \
		(void)(true ? (void)0 : ( (void)(_a1) ) );                    \
		BASE_PRAGMA_DIAGNOSTIC_POP();                                   \
	BASE_MACRO_BLOCK_END

#define BASE_UNUSED_2(_a1, _a2) BASE_UNUSED_1(_a1); BASE_UNUSED_1(_a2)
#define BASE_UNUSED_3(_a1, _a2, _a3) BASE_UNUSED_2(_a1, _a2); BASE_UNUSED_1(_a3)
#define BASE_UNUSED_4(_a1, _a2, _a3, _a4) BASE_UNUSED_3(_a1, _a2, _a3); BASE_UNUSED_1(_a4)
#define BASE_UNUSED_5(_a1, _a2, _a3, _a4, _a5) BASE_UNUSED_4(_a1, _a2, _a3, _a4); BASE_UNUSED_1(_a5)
#define BASE_UNUSED_6(_a1, _a2, _a3, _a4, _a5, _a6) BASE_UNUSED_5(_a1, _a2, _a3, _a4, _a5); BASE_UNUSED_1(_a6)
#define BASE_UNUSED_7(_a1, _a2, _a3, _a4, _a5, _a6, _a7) BASE_UNUSED_6(_a1, _a2, _a3, _a4, _a5, _a6); BASE_UNUSED_1(_a7)
#define BASE_UNUSED_8(_a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8) BASE_UNUSED_7(_a1, _a2, _a3, _a4, _a5, _a6, _a7); BASE_UNUSED_1(_a8)
#define BASE_UNUSED_9(_a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9) BASE_UNUSED_8(_a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8); BASE_UNUSED_1(_a9)
#define BASE_UNUSED_10(_a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10) BASE_UNUSED_9(_a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9); BASE_UNUSED_1(_a10)
#define BASE_UNUSED_11(_a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10, _a11) BASE_UNUSED_10(_a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10); BASE_UNUSED_1(_a11)
#define BASE_UNUSED_12(_a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10, _a11, _a12) BASE_UNUSED_11(_a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10, _a11); BASE_UNUSED_1(_a12)

#if BASE_COMPILER_MSVC
// Workaround MSVS bug...
#	define BASE_UNUSED(...) BASE_MACRO_DISPATCHER(BASE_UNUSED_, __VA_ARGS__) BASE_VA_ARGS_PASS(__VA_ARGS__)
#else
#	define BASE_UNUSED(...) BASE_MACRO_DISPATCHER(BASE_UNUSED_, __VA_ARGS__)(__VA_ARGS__)
#endif // BASE_COMPILER_MSVC

///
#if BASE_COMPILER_CLANG
#	define BASE_PRAGMA_DIAGNOSTIC_PUSH_CLANG_()     _Pragma("clang diagnostic push")
#	define BASE_PRAGMA_DIAGNOSTIC_POP_CLANG_()      _Pragma("clang diagnostic pop")
#	define BASE_PRAGMA_DIAGNOSTIC_IGNORED_CLANG(_x) _Pragma(BASE_STRINGIZE(clang diagnostic ignored _x) )
#else
#	define BASE_PRAGMA_DIAGNOSTIC_PUSH_CLANG_()
#	define BASE_PRAGMA_DIAGNOSTIC_POP_CLANG_()
#	define BASE_PRAGMA_DIAGNOSTIC_IGNORED_CLANG(_x)
#endif // BASE_COMPILER_CLANG

#if BASE_COMPILER_GCC && BASE_COMPILER_GCC >= 40600
#	define BASE_PRAGMA_DIAGNOSTIC_PUSH_GCC_()       _Pragma("GCC diagnostic push")
#	define BASE_PRAGMA_DIAGNOSTIC_POP_GCC_()        _Pragma("GCC diagnostic pop")
#	define BASE_PRAGMA_DIAGNOSTIC_IGNORED_GCC(_x)   _Pragma(BASE_STRINGIZE(GCC diagnostic ignored _x) )
#else
#	define BASE_PRAGMA_DIAGNOSTIC_PUSH_GCC_()
#	define BASE_PRAGMA_DIAGNOSTIC_POP_GCC_()
#	define BASE_PRAGMA_DIAGNOSTIC_IGNORED_GCC(_x)
#endif // BASE_COMPILER_GCC

#if BASE_COMPILER_MSVC
#	define BASE_PRAGMA_DIAGNOSTIC_PUSH_MSVC_()     __pragma(warning(push) )
#	define BASE_PRAGMA_DIAGNOSTIC_POP_MSVC_()      __pragma(warning(pop) )
#	define BASE_PRAGMA_DIAGNOSTIC_IGNORED_MSVC(_x) __pragma(warning(disable:_x) )
#else
#	define BASE_PRAGMA_DIAGNOSTIC_PUSH_MSVC_()
#	define BASE_PRAGMA_DIAGNOSTIC_POP_MSVC_()
#	define BASE_PRAGMA_DIAGNOSTIC_IGNORED_MSVC(_x)
#endif // BASE_COMPILER_CLANG

#if BASE_COMPILER_CLANG
#	define BASE_PRAGMA_DIAGNOSTIC_PUSH              BASE_PRAGMA_DIAGNOSTIC_PUSH_CLANG_
#	define BASE_PRAGMA_DIAGNOSTIC_POP               BASE_PRAGMA_DIAGNOSTIC_POP_CLANG_
#	define BASE_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC BASE_PRAGMA_DIAGNOSTIC_IGNORED_CLANG
#elif BASE_COMPILER_GCC
#	define BASE_PRAGMA_DIAGNOSTIC_PUSH              BASE_PRAGMA_DIAGNOSTIC_PUSH_GCC_
#	define BASE_PRAGMA_DIAGNOSTIC_POP               BASE_PRAGMA_DIAGNOSTIC_POP_GCC_
#	define BASE_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC BASE_PRAGMA_DIAGNOSTIC_IGNORED_GCC
#elif BASE_COMPILER_MSVC
#	define BASE_PRAGMA_DIAGNOSTIC_PUSH              BASE_PRAGMA_DIAGNOSTIC_PUSH_MSVC_
#	define BASE_PRAGMA_DIAGNOSTIC_POP               BASE_PRAGMA_DIAGNOSTIC_POP_MSVC_
#	define BASE_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC(_x)
#endif // BASE_COMPILER_

/// No default constructor.
#define BASE_CLASS_NO_DEFAULT_CTOR(_class) \
	_class() = delete

/// No copy constructor.
#define BASE_CLASS_NO_COPY_CTOR(_class) \
	_class(const _class& _rhs) = delete

/// No copy assignment operator.
#define BASE_CLASS_NO_COPY_ASSIGNMENT(_class) \
	_class& operator=(const _class& _rhs) = delete

/// No copy construcor, and copy assignment operator.
#define BASE_CLASS_NO_COPY(_class)   \
	BASE_CLASS_NO_COPY_CTOR(_class); \
	BASE_CLASS_NO_COPY_ASSIGNMENT(_class)

///
#define BASE_CLASS_ALLOCATOR(_class)              \
	public: void* operator new(size_t _size);   \
	public: void  operator delete(void* _ptr);  \
	public: void* operator new[](size_t _size); \
	public: void  operator delete[](void* _ptr)

#define BASE_CLASS_1(_class, _a1) BASE_CONCATENATE(BASE_CLASS_, _a1)(_class)
#define BASE_CLASS_2(_class, _a1, _a2) BASE_CLASS_1(_class, _a1); BASE_CLASS_1(_class, _a2)
#define BASE_CLASS_3(_class, _a1, _a2, _a3) BASE_CLASS_2(_class, _a1, _a2); BASE_CLASS_1(_class, _a3)
#define BASE_CLASS_4(_class, _a1, _a2, _a3, _a4) BASE_CLASS_3(_class, _a1, _a2, _a3); BASE_CLASS_1(_class, _a4)

#if BASE_COMPILER_MSVC
#	define BASE_CLASS(_class, ...) BASE_MACRO_DISPATCHER(BASE_CLASS_, __VA_ARGS__) BASE_VA_ARGS_PASS(_class, __VA_ARGS__)
#else
#	define BASE_CLASS(_class, ...) BASE_MACRO_DISPATCHER(BASE_CLASS_, __VA_ARGS__)(_class, __VA_ARGS__)
#endif // BASE_COMPILER_MSVC

#ifndef BASE_ASSERT
#	if BASE_CONFIG_DEBUG
#		define BASE_ASSERT _BASE_ASSERT
#	else
#		define BASE_ASSERT(_condition, ...) BASE_NOOP()
#	endif // BASE_CONFIG_DEBUG
#endif // BASE_ASSERT

#ifndef BASE_ASSERT_LOC
#	if BASE_CONFIG_DEBUG
#		define BASE_ASSERT_LOC _BASE_ASSERT_LOC
#	else
#		define BASE_ASSERT_LOC(...) BASE_NOOP()
#	endif // BASE_CONFIG_DEBUG
#endif // BASE_ASSERT_LOC

#ifndef BASE_TRACE
#	if BASE_CONFIG_DEBUG
#		define BASE_TRACE _BASE_TRACE
#	else
#		define BASE_TRACE(...) BASE_NOOP()
#	endif // BASE_CONFIG_DEBUG
#endif // BASE_TRACE

#ifndef BASE_TRACE_LOC
#	if BASE_CONFIG_DEBUG
#		define BASE_TRACE_LOC _BASE_TRACE_LOC
#	else
#		define BASE_TRACE_LOC(...) BASE_NOOP()
#	endif // BASE_CONFIG_DEBUG
#endif // BASE_TRACE_LOC

#ifndef BASE_WARN
#	if BASE_CONFIG_DEBUG
#		define BASE_WARN _BASE_WARN
#	else
#		define BASE_WARN(_condition, ...) BASE_NOOP()
#	endif // BASE_CONFIG_DEBUG
#endif // BASE_ASSERT

#ifndef BASE_WARN_LOC
#	if BASE_CONFIG_DEBUG
#		define BASE_WARN_LOC _BASE_WARN_LOC
#	else
#		define BASE_WARN_LOC(...) BASE_NOOP()
#	endif // BASE_CONFIG_DEBUG
#endif // BASE_WARN_LOC

#define _BASE_TRACE(_format, ...)                                                                    \
	BASE_MACRO_BLOCK_BEGIN                                                                           \
		base::debugPrintf(__FILE__ "(" BASE_STRINGIZE(__LINE__) "): BASE " _format "\n", ##__VA_ARGS__); \
	BASE_MACRO_BLOCK_END

#define _BASE_TRACE_LOC(_location, _format, ...)                                                          \
	BASE_MACRO_BLOCK_BEGIN                                                                                \
		base::debugPrintf("%s(%d): BASE " _format "\n", _location.filePath, _location.line, ##__VA_ARGS__); \
	BASE_MACRO_BLOCK_END

#define _BASE_WARN(_condition, _format, ...)            \
	BASE_MACRO_BLOCK_BEGIN                              \
		if (!BASE_IGNORE_C4127(_condition) )            \
		{                                             \
			BASE_TRACE("WARN " _format, ##__VA_ARGS__); \
		}                                             \
	BASE_MACRO_BLOCK_END

#define _BASE_ASSERT(_condition, _format, ...)            \
	BASE_MACRO_BLOCK_BEGIN                                \
		if (!BASE_IGNORE_C4127(_condition) )              \
		{                                               \
			BASE_TRACE("ASSERT " _format, ##__VA_ARGS__); \
			base::debugBreak();                           \
		}                                               \
	BASE_MACRO_BLOCK_END

#define _BASE_ASSERT_LOC(_location, _condition, _format, ...)             \
	BASE_MACRO_BLOCK_BEGIN                                                \
		if (!BASE_IGNORE_C4127(_condition) )                              \
		{                                                               \
			_BASE_TRACE_LOC(_location, "ASSERT " _format, ##__VA_ARGS__); \
			base::debugBreak();                                           \
		}                                                               \
	BASE_MACRO_BLOCK_END

#define _BASE_WARN_LOC(_location, _condition, _format, ...)             \
	BASE_MACRO_BLOCK_BEGIN                                              \
		if (!BASE_IGNORE_C4127(_condition) )                            \
		{                                                             \
			_BASE_TRACE_LOC(_location, "WARN " _format, ##__VA_ARGS__); \
		}                                                             \
	BASE_MACRO_BLOCK_END

// static_assert sometimes causes unused-local-typedef...
BASE_PRAGMA_DIAGNOSTIC_IGNORED_CLANG("-Wunused-local-typedef")

#endif // BASE_MACROS_H_HEADER_GUARD
