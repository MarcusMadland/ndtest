/*
 * Copyright 2010-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/base/blob/master/LICENSE
 */

#ifndef BASE_H_HEADER_GUARD
#	error "Must be included from base/os.h!"
#endif // BASE_H_HEADER_GUARD

namespace base
{
	template<typename ProtoT>
	inline ProtoT dlsym(void* _handle, const StringView& _symbol)
	{
		return reinterpret_cast<ProtoT>(dlsym(_handle, _symbol) );
	}

} // namespace base
