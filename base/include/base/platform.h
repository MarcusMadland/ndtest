/*
 * Copyright 2010-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/base/blob/master/LICENSE
 */

#ifndef BASE_PLATFORM_H_HEADER_GUARD
#define BASE_PLATFORM_H_HEADER_GUARD

// Architecture
#define BASE_ARCH_32BIT 0
#define BASE_ARCH_64BIT 0

// Compiler
#define BASE_COMPILER_CLANG          0
#define BASE_COMPILER_CLANG_ANALYZER 0
#define BASE_COMPILER_GCC            0
#define BASE_COMPILER_MSVC           0

// Endianness
#define BASE_CPU_ENDIAN_BIG    0
#define BASE_CPU_ENDIAN_LITTLE 0

// CPU
#define BASE_CPU_ARM   0
#define BASE_CPU_JIT   0
#define BASE_CPU_MIPS  0
#define BASE_CPU_PPC   0
#define BASE_CPU_RISCV 0
#define BASE_CPU_X86   0

// C Runtime
#define BASE_CRT_BIONIC 0
#define BASE_CRT_BSD    0
#define BASE_CRT_GLIBC  0
#define BASE_CRT_LIBCXX 0
#define BASE_CRT_MINGW  0
#define BASE_CRT_MSVC   0
#define BASE_CRT_NEWLIB 0

#ifndef BASE_CRT_NONE
#	define BASE_CRT_NONE 0
#endif // BASE_CRT_NONE

// Language standard version
#define BASE_LANGUAGE_CPP14 201402L
#define BASE_LANGUAGE_CPP17 201703L
#define BASE_LANGUAGE_CPP20 202002L
#define BASE_LANGUAGE_CPP23 202207L

// Platform
#define BASE_PLATFORM_ANDROID    0
#define BASE_PLATFORM_BSD        0
#define BASE_PLATFORM_EMSCRIPTEN 0
#define BASE_PLATFORM_HAIKU      0
#define BASE_PLATFORM_HURD       0
#define BASE_PLATFORM_IOS        0
#define BASE_PLATFORM_LINUX      0
#define BASE_PLATFORM_NX         0
#define BASE_PLATFORM_OSX        0
#define BASE_PLATFORM_PS4        0
#define BASE_PLATFORM_PS5        0
#define BASE_PLATFORM_RPI        0
#define BASE_PLATFORM_WINDOWS    0
#define BASE_PLATFORM_WINRT      0
#define BASE_PLATFORM_XBOXONE    0

// http://sourceforge.net/apps/mediawiki/predef/index.php?title=Compilers
#if defined(__clang__)
// clang defines __GNUC__ or _MSC_VER
#	undef  BASE_COMPILER_CLANG
#	define BASE_COMPILER_CLANG (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#	if defined(__clang_analyzer__)
#		undef  BASE_COMPILER_CLANG_ANALYZER
#		define BASE_COMPILER_CLANG_ANALYZER 1
#	endif // defined(__clang_analyzer__)
#elif defined(_MSC_VER)
#	undef  BASE_COMPILER_MSVC
#	define BASE_COMPILER_MSVC _MSC_VER
#elif defined(__GNUC__)
#	undef  BASE_COMPILER_GCC
#	define BASE_COMPILER_GCC (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#else
#	error "BASE_COMPILER_* is not defined!"
#endif //

// http://sourceforge.net/apps/mediawiki/predef/index.php?title=Architectures
#if defined(__arm__)     \
 || defined(__aarch64__) \
 || defined(_M_ARM)
#	undef  BASE_CPU_ARM
#	define BASE_CPU_ARM 1
#	define BASE_CACHE_LINE_SIZE 64
#elif defined(__MIPSEL__)     \
 ||   defined(__mips_isa_rev) \
 ||   defined(__mips64)
#	undef  BASE_CPU_MIPS
#	define BASE_CPU_MIPS 1
#	define BASE_CACHE_LINE_SIZE 64
#elif defined(_M_PPC)        \
 ||   defined(__powerpc__)   \
 ||   defined(__powerpc64__)
#	undef  BASE_CPU_PPC
#	define BASE_CPU_PPC 1
#	define BASE_CACHE_LINE_SIZE 128
#elif defined(__riscv)   \
 ||   defined(__riscv__) \
 ||   defined(RISCVEL)
#	undef  BASE_CPU_RISCV
#	define BASE_CPU_RISCV 1
#	define BASE_CACHE_LINE_SIZE 64
#elif defined(_M_IX86)    \
 ||   defined(_M_X64)     \
 ||   defined(__i386__)   \
 ||   defined(__x86_64__)
#	undef  BASE_CPU_X86
#	define BASE_CPU_X86 1
#	define BASE_CACHE_LINE_SIZE 64
#else // PNaCl doesn't have CPU defined.
#	undef  BASE_CPU_JIT
#	define BASE_CPU_JIT 1
#	define BASE_CACHE_LINE_SIZE 64
#endif //

#if defined(__x86_64__)    \
 || defined(_M_X64)        \
 || defined(__aarch64__)   \
 || defined(__64BIT__)     \
 || defined(__mips64)      \
 || defined(__powerpc64__) \
 || defined(__ppc64__)     \
 || defined(__LP64__)
#	undef  BASE_ARCH_64BIT
#	define BASE_ARCH_64BIT 64
#else
#	undef  BASE_ARCH_32BIT
#	define BASE_ARCH_32BIT 32
#endif //

#if BASE_CPU_PPC
// __BIG_ENDIAN__ is gcc predefined macro
#	if defined(__BIG_ENDIAN__)
#		undef  BASE_CPU_ENDIAN_BIG
#		define BASE_CPU_ENDIAN_BIG 1
#	else
#		undef  BASE_CPU_ENDIAN_LITTLE
#		define BASE_CPU_ENDIAN_LITTLE 1
#	endif
#else
#	undef  BASE_CPU_ENDIAN_LITTLE
#	define BASE_CPU_ENDIAN_LITTLE 1
#endif // BASE_CPU_PPC

// http://sourceforge.net/apps/mediawiki/predef/index.php?title=Operating_Systems
#if defined(_DURANGO) || defined(_XBOX_ONE)
#	undef  BASE_PLATFORM_XBOXONE
#	define BASE_PLATFORM_XBOXONE 1
#elif defined(_WIN32) || defined(_WIN64)
// http://msdn.microsoft.com/en-us/library/6sehtctf.aspx
#	ifndef NOMINMAX
#		define NOMINMAX
#	endif // NOMINMAX
//  If _USING_V110_SDK71_ is defined it means we are using the v110_xp or v120_xp toolset.
#	if defined(_MSC_VER) && (_MSC_VER >= 1700) && !defined(_USING_V110_SDK71_)
#		include <winapifamily.h>
#	endif // defined(_MSC_VER) && (_MSC_VER >= 1700) && (!_USING_V110_SDK71_)
#	if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
#		undef  BASE_PLATFORM_WINDOWS
#		if !defined(WINVER) && !defined(_WIN32_WINNT)
#			if BASE_ARCH_64BIT
//				When building 64-bit target Win7 and above.
#				define WINVER 0x0601
#				define _WIN32_WINNT 0x0601
#			else
//				Windows Server 2003 with SP1, Windows XP with SP2 and above
#				define WINVER 0x0502
#				define _WIN32_WINNT 0x0502
#			endif // BASE_ARCH_64BIT
#		endif // !defined(WINVER) && !defined(_WIN32_WINNT)
#		define BASE_PLATFORM_WINDOWS _WIN32_WINNT
#	else
#		undef  BASE_PLATFORM_WINRT
#		define BASE_PLATFORM_WINRT 1
#	endif
#elif defined(__ANDROID__)
// Android compiler defines __linux__
#	include <sys/cdefs.h> // Defines __BIONIC__ and includes android/api-level.h
#	undef  BASE_PLATFORM_ANDROID
#	define BASE_PLATFORM_ANDROID __ANDROID_API__
#elif defined(__VCCOREVER__)
// RaspberryPi compiler defines __linux__
#	undef  BASE_PLATFORM_RPI
#	define BASE_PLATFORM_RPI 1
#elif  defined(__linux__)
#	undef  BASE_PLATFORM_LINUX
#	define BASE_PLATFORM_LINUX 1
#elif  defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) \
	|| defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__)
#	undef  BASE_PLATFORM_IOS
#	define BASE_PLATFORM_IOS 1
#elif defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__)
#	undef  BASE_PLATFORM_OSX
#	define BASE_PLATFORM_OSX __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
#elif defined(__EMSCRIPTEN__)
#	undef  BASE_PLATFORM_EMSCRIPTEN
#	define BASE_PLATFORM_EMSCRIPTEN 1
#elif defined(__ORBIS__)
#	undef  BASE_PLATFORM_PS4
#	define BASE_PLATFORM_PS4 1
#elif defined(__PROSPERO__)
#	undef  BASE_PLATFORM_PS5
#	define BASE_PLATFORM_PS5 1
#elif  defined(__FreeBSD__)        \
	|| defined(__FreeBSD_kernel__) \
	|| defined(__NetBSD__)         \
	|| defined(__OpenBSD__)        \
	|| defined(__DragonFly__)
#	undef  BASE_PLATFORM_BSD
#	define BASE_PLATFORM_BSD 1
#elif defined(__GNU__)
#	undef  BASE_PLATFORM_HURD
#	define BASE_PLATFORM_HURD 1
#elif defined(__NX__)
#	undef  BASE_PLATFORM_NX
#	define BASE_PLATFORM_NX 1
#elif defined(__HAIKU__)
#	undef  BASE_PLATFORM_HAIKU
#	define BASE_PLATFORM_HAIKU 1
#endif //

#if !BASE_CRT_NONE
// https://sourceforge.net/p/predef/wiki/Libraries/
#	if defined(__BIONIC__)
#		undef  BASE_CRT_BIONIC
#		define BASE_CRT_BIONIC 1
#	elif defined(_MSC_VER)
#		undef  BASE_CRT_MSVC
#		define BASE_CRT_MSVC 1
#	elif defined(__GLIBC__)
#		undef  BASE_CRT_GLIBC
#		define BASE_CRT_GLIBC (__GLIBC__ * 10000 + __GLIBC_MINOR__ * 100)
#	elif defined(__MINGW32__) || defined(__MINGW64__)
#		undef  BASE_CRT_MINGW
#		define BASE_CRT_MINGW 1
#	elif defined(__apple_build_version__) || defined(__ORBIS__) || defined(__EMSCRIPTEN__) || defined(__llvm__) || defined(__HAIKU__)
#		undef  BASE_CRT_LIBCXX
#		define BASE_CRT_LIBCXX 1
#	elif BASE_PLATFORM_BSD
#		undef  BASE_CRT_BSD
#		define BASE_CRT_BSD 1
#	endif //

#	if !BASE_CRT_BIONIC \
	&& !BASE_CRT_BSD    \
	&& !BASE_CRT_GLIBC  \
	&& !BASE_CRT_LIBCXX \
	&& !BASE_CRT_MINGW  \
	&& !BASE_CRT_MSVC   \
	&& !BASE_CRT_NEWLIB
#		undef  BASE_CRT_NONE
#		define BASE_CRT_NONE 1
#	endif // BASE_CRT_*
#endif // !BASE_CRT_NONE

///
#define BASE_PLATFORM_POSIX (0   \
	||  BASE_PLATFORM_ANDROID    \
	||  BASE_PLATFORM_BSD        \
	||  BASE_PLATFORM_EMSCRIPTEN \
	||  BASE_PLATFORM_HAIKU      \
	||  BASE_PLATFORM_HURD       \
	||  BASE_PLATFORM_IOS        \
	||  BASE_PLATFORM_LINUX      \
	||  BASE_PLATFORM_NX         \
	||  BASE_PLATFORM_OSX        \
	||  BASE_PLATFORM_PS4        \
	||  BASE_PLATFORM_PS5        \
	||  BASE_PLATFORM_RPI        \
	)

///
#define BASE_PLATFORM_NONE !(0   \
	||  BASE_PLATFORM_ANDROID    \
	||  BASE_PLATFORM_BSD        \
	||  BASE_PLATFORM_EMSCRIPTEN \
	||  BASE_PLATFORM_HAIKU      \
	||  BASE_PLATFORM_HURD       \
	||  BASE_PLATFORM_IOS        \
	||  BASE_PLATFORM_LINUX      \
	||  BASE_PLATFORM_NX         \
	||  BASE_PLATFORM_OSX        \
	||  BASE_PLATFORM_PS4        \
	||  BASE_PLATFORM_PS5        \
	||  BASE_PLATFORM_RPI        \
	||  BASE_PLATFORM_WINDOWS    \
	||  BASE_PLATFORM_WINRT      \
	||  BASE_PLATFORM_XBOXONE    \
	)

///
#define BASE_PLATFORM_OS_CONSOLE  (0 \
	||  BASE_PLATFORM_NX             \
	||  BASE_PLATFORM_PS4            \
	||  BASE_PLATFORM_PS5            \
	||  BASE_PLATFORM_WINRT          \
	||  BASE_PLATFORM_XBOXONE        \
	)

///
#define BASE_PLATFORM_OS_DESKTOP  (0 \
	||  BASE_PLATFORM_BSD            \
	||  BASE_PLATFORM_HAIKU          \
	||  BASE_PLATFORM_HURD           \
	||  BASE_PLATFORM_LINUX          \
	||  BASE_PLATFORM_OSX            \
	||  BASE_PLATFORM_WINDOWS        \
	)

///
#define BASE_PLATFORM_OS_EMBEDDED (0 \
	||  BASE_PLATFORM_RPI            \
	)

///
#define BASE_PLATFORM_OS_MOBILE   (0 \
	||  BASE_PLATFORM_ANDROID        \
	||  BASE_PLATFORM_IOS            \
	)

///
#define BASE_PLATFORM_OS_WEB      (0 \
	||  BASE_PLATFORM_EMSCRIPTEN     \
	)

///
#if BASE_COMPILER_GCC
#	define BASE_COMPILER_NAME "GCC "       \
		BASE_STRINGIZE(__GNUC__) "."       \
		BASE_STRINGIZE(__GNUC_MINOR__) "." \
		BASE_STRINGIZE(__GNUC_PATCHLEVEL__)
#elif BASE_COMPILER_CLANG
#	define BASE_COMPILER_NAME "Clang "      \
		BASE_STRINGIZE(__clang_major__) "." \
		BASE_STRINGIZE(__clang_minor__) "." \
		BASE_STRINGIZE(__clang_patchlevel__)
#elif BASE_COMPILER_MSVC
#	if BASE_COMPILER_MSVC >= 1930 // Visual Studio 2022
#		define BASE_COMPILER_NAME "MSVC 17.0"
#	elif BASE_COMPILER_MSVC >= 1920 // Visual Studio 2019
#		define BASE_COMPILER_NAME "MSVC 16.0"
#	elif BASE_COMPILER_MSVC >= 1910 // Visual Studio 2017
#		define BASE_COMPILER_NAME "MSVC 15.0"
#	elif BASE_COMPILER_MSVC >= 1900 // Visual Studio 2015
#		define BASE_COMPILER_NAME "MSVC 14.0"
#	elif BASE_COMPILER_MSVC >= 1800 // Visual Studio 2013
#		define BASE_COMPILER_NAME "MSVC 12.0"
#	elif BASE_COMPILER_MSVC >= 1700 // Visual Studio 2012
#		define BASE_COMPILER_NAME "MSVC 11.0"
#	elif BASE_COMPILER_MSVC >= 1600 // Visual Studio 2010
#		define BASE_COMPILER_NAME "MSVC 10.0"
#	elif BASE_COMPILER_MSVC >= 1500 // Visual Studio 2008
#		define BASE_COMPILER_NAME "MSVC 9.0"
#	else
#		define BASE_COMPILER_NAME "MSVC"
#	endif //
#endif // BASE_COMPILER_

#if BASE_PLATFORM_ANDROID
#	define BASE_PLATFORM_NAME "Android " \
				BASE_STRINGIZE(BASE_PLATFORM_ANDROID)
#elif BASE_PLATFORM_BSD
#	define BASE_PLATFORM_NAME "BSD"
#elif BASE_PLATFORM_EMSCRIPTEN
#	define BASE_PLATFORM_NAME "asm.js "          \
		BASE_STRINGIZE(__EMSCRIPTEN_major__) "." \
		BASE_STRINGIZE(__EMSCRIPTEN_minor__) "." \
		BASE_STRINGIZE(__EMSCRIPTEN_tiny__)
#elif BASE_PLATFORM_HAIKU
#	define BASE_PLATFORM_NAME "Haiku"
#elif BASE_PLATFORM_HURD
#	define BASE_PLATFORM_NAME "Hurd"
#elif BASE_PLATFORM_IOS
#	define BASE_PLATFORM_NAME "iOS"
#elif BASE_PLATFORM_LINUX
#	define BASE_PLATFORM_NAME "Linux"
#elif BASE_PLATFORM_NONE
#	define BASE_PLATFORM_NAME "None"
#elif BASE_PLATFORM_NX
#	define BASE_PLATFORM_NAME "NX"
#elif BASE_PLATFORM_OSX
#	define BASE_PLATFORM_NAME "OSX"
#elif BASE_PLATFORM_PS4
#	define BASE_PLATFORM_NAME "PlayStation 4"
#elif BASE_PLATFORM_PS5
#	define BASE_PLATFORM_NAME "PlayStation 5"
#elif BASE_PLATFORM_RPI
#	define BASE_PLATFORM_NAME "RaspberryPi"
#elif BASE_PLATFORM_WINDOWS
#	define BASE_PLATFORM_NAME "Windows"
#elif BASE_PLATFORM_WINRT
#	define BASE_PLATFORM_NAME "WinRT"
#elif BASE_PLATFORM_XBOXONE
#	define BASE_PLATFORM_NAME "Xbox One"
#else
#	error "Unknown BASE_PLATFORM!"
#endif // BASE_PLATFORM_

#if BASE_CPU_ARM
#	define BASE_CPU_NAME "ARM"
#elif BASE_CPU_JIT
#	define BASE_CPU_NAME "JIT-VM"
#elif BASE_CPU_MIPS
#	define BASE_CPU_NAME "MIPS"
#elif BASE_CPU_PPC
#	define BASE_CPU_NAME "PowerPC"
#elif BASE_CPU_RISCV
#	define BASE_CPU_NAME "RISC-V"
#elif BASE_CPU_X86
#	define BASE_CPU_NAME "x86"
#endif // BASE_CPU_

#if BASE_CRT_BIONIC
#	define BASE_CRT_NAME "Bionic libc"
#elif BASE_CRT_BSD
#	define BASE_CRT_NAME "BSD libc"
#elif BASE_CRT_GLIBC
#	define BASE_CRT_NAME "GNU C Library"
#elif BASE_CRT_MSVC
#	define BASE_CRT_NAME "MSVC C Runtime"
#elif BASE_CRT_MINGW
#	define BASE_CRT_NAME "MinGW C Runtime"
#elif BASE_CRT_LIBCXX
#	define BASE_CRT_NAME "Clang C Library"
#elif BASE_CRT_NEWLIB
#	define BASE_CRT_NAME "Newlib"
#elif BASE_CRT_NONE
#	define BASE_CRT_NAME "None"
#else
#	error "Unknown BASE_CRT!"
#endif // BASE_CRT_

#if BASE_ARCH_32BIT
#	define BASE_ARCH_NAME "32-bit"
#elif BASE_ARCH_64BIT
#	define BASE_ARCH_NAME "64-bit"
#endif // BASE_ARCH_

#if defined(__cplusplus)
#	if   __cplusplus < BASE_LANGUAGE_CPP14
#		error "C++14 standard support is required to build."
#	elif __cplusplus < BASE_LANGUAGE_CPP17
#		define BASE_CPP_NAME "C++14"
#	elif __cplusplus < BASE_LANGUAGE_CPP20
#		define BASE_CPP_NAME "C++17"
#	elif __cplusplus < BASE_LANGUAGE_CPP23
#		define BASE_CPP_NAME "C++20"
#	else
// See: https://gist.github.com/bkaradzic/2e39896bc7d8c34e042b#orthodox-c
#		define BASE_CPP_NAME "C++WayTooModern"
#	endif // BASE_CPP_NAME
#else
#	define BASE_CPP_NAME "C++Unknown"
#endif // defined(__cplusplus)

#if BASE_PLATFORM_BSD   \
 || BASE_PLATFORM_HAIKU \
 || BASE_PLATFORM_HURD
static_assert(false, "\n\n"
	"\t** IMPORTANT! **\n\n"
	"\tYou're compiling for unsupported platform!\n"
	"\tIf you wish to support this platform, make your own fork, and modify code for _yourself_.\n"
	"\t\n"
	"\tDo not submit PR to main repo, it won't be considered, and it would code rot anyway. I have no ability\n"
	"\tto test on these platforms, and over years there wasn't any serious contributor who wanted to take\n"
	"\tburden of maintaining code for these platforms.\n"
	"\t\n");
#endif // BASE_PLATFORM_*

#endif // BASE_PLATFORM_H_HEADER_GUARD
