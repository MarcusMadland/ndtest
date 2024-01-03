/*
 * Copyright 2010-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/base/blob/master/LICENSE
 */

#ifndef BASE_SEM_H_HEADER_GUARD
#define BASE_SEM_H_HEADER_GUARD

#include "base.h"

namespace base
{
	///
	class Semaphore
	{
		BASE_CLASS(Semaphore
			, NO_COPY
			);

	public:
		///
		Semaphore();

		///
		~Semaphore();

		///
		void post(uint32_t _count = 1);

		///
		bool wait(int32_t _msecs = -1);

	private:
		BASE_ALIGN_DECL(16, uint8_t) m_internal[128];
	};

} // namespace base

#endif // BASE_SEM_H_HEADER_GUARD
