/*
 * Copyright 2010-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/base/blob/master/LICENSE
 */

#ifndef BASE_CONFIG_H_HEADER_GUARD
#define BASE_CONFIG_H_HEADER_GUARD

#ifndef BASE_CONFIG_DEBUG
#	error "BASE_CONFIG_DEBUG must be defined in build script!"
#endif // BASE_CONFIG_DEBUG

#ifndef BASE_CONFIG_ALLOCATOR_DEBUG
#	define BASE_CONFIG_ALLOCATOR_DEBUG BASE_CONFIG_DEBUG
#endif // BASE_CONFIG_ALLOCATOR_DEBUG

#ifndef BASE_CONFIG_SUPPORTS_THREADING
#	define BASE_CONFIG_SUPPORTS_THREADING !(0 \
			|| BASE_PLATFORM_EMSCRIPTEN       \
			)
#endif // BASE_CONFIG_SUPPORTS_THREADING

#endif // BASE_CONFIG_H_HEADER_GUARD
