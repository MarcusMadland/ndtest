/*
 * Copyright 2010-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/base/blob/master/LICENSE
 */

#ifndef BASE_DEBUG_H_HEADER_GUARD
#define BASE_DEBUG_H_HEADER_GUARD

#include "base.h"

namespace base
{
	class StringView;

	///
	void debugBreak();

	///
	void debugOutput(const char* _out);

	///
	void debugOutput(const StringView& _str);

	///
	void debugPrintfVargs(const char* _format, va_list _argList);

	///
	void debugPrintf(const char* _format, ...);

	///
	void debugPrintfData(const void* _data, uint32_t _size, const char* _format, ...);

	///
	struct WriterI* getDebugOut();

} // namespace base

#endif // BASE_DEBUG_H_HEADER_GUARD
