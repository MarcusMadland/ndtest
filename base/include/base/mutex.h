/*
 * Copyright 2010-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/base/blob/master/LICENSE
 */

#ifndef BASE_MUTEX_H_HEADER_GUARD
#define BASE_MUTEX_H_HEADER_GUARD

#include "base.h"

namespace base
{
	///
	class Mutex
	{
		BASE_CLASS(Mutex
			, NO_COPY
			);

	public:
		///
		Mutex();

		///
		~Mutex();

		///
		void lock();

		///
		void unlock();

	private:
		BASE_ALIGN_DECL(16, uint8_t) m_internal[64];
	};

	///
	class MutexScope
	{
		BASE_CLASS(MutexScope
			, NO_DEFAULT_CTOR
			, NO_COPY
			);

	public:
		///
		MutexScope(Mutex& _mutex);

		///
		~MutexScope();

	private:
		Mutex& m_mutex;
	};

} // namespace base

#include "inline/mutex.inl"

#endif // BASE_MUTEX_H_HEADER_GUARD
