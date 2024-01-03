/*
 * Copyright 2010-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/base/blob/master/LICENSE
 */

#ifndef BASE_OS_H_HEADER_GUARD
#define BASE_OS_H_HEADER_GUARD

#include "filepath.h"

#if BASE_PLATFORM_OSX
#	define BASE_DL_EXT "dylib"
#elif BASE_PLATFORM_WINDOWS
#	define BASE_DL_EXT "dll"
#else
#	define BASE_DL_EXT "so"
#endif //

namespace base
{
	///
	void sleep(uint32_t _ms);

	///
	void yield();

	///
	uint32_t getTid();

	///
	size_t getProcessMemoryUsed();

	///
	void* dlopen(const FilePath& _filePath);

	///
	void dlclose(void* _handle);

	///
	void* dlsym(void* _handle, const StringView& _symbol);

	///
	template<typename ProtoT>
	ProtoT dlsym(void* _handle, const StringView& _symbol);

	///
	bool getEnv(char* _out, uint32_t* _inOutSize, const StringView& _name);

	///
	void setEnv(const StringView& _name, const StringView& _value);

	///
	int chdir(const char* _path);

	///
	void* exec(const char* const* _argv);

	///
	BASE_NO_RETURN void exit(int32_t _exitCode);

} // namespace base

#include "inline/os.inl"

#endif // BASE_OS_H_HEADER_GUARD
