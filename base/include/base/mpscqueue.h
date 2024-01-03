/*
 * Copyright 2010-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/base/blob/master/LICENSE
 */

#ifndef BASE_MPSCQUEUE_H_HEADER_GUARD
#define BASE_MPSCQUEUE_H_HEADER_GUARD

#include "allocator.h"
#include "mutex.h"
#include "spscqueue.h"

namespace base
{
	///
	template <typename Ty>
	class MpScUnboundedQueueT
	{
		BASE_CLASS(MpScUnboundedQueueT
			, NO_DEFAULT_CTOR
			, NO_COPY
			);

	public:
		///
		MpScUnboundedQueueT(AllocatorI* _allocator);

		///
		~MpScUnboundedQueueT();

		///
		void push(Ty* _ptr); // producer only

		///
		Ty* peek(); // consumer only

		///
		Ty* pop(); // consumer only

	private:
		Mutex m_write;
		SpScUnboundedQueueT<Ty> m_queue;
	};

	///
	template <typename Ty>
	class MpScUnboundedBlockingQueue
	{
		BASE_CLASS(MpScUnboundedBlockingQueue
			, NO_DEFAULT_CTOR
			, NO_COPY
			);

	public:
		///
		MpScUnboundedBlockingQueue(AllocatorI* _allocator);

		///
		~MpScUnboundedBlockingQueue();

		///
		void push(Ty* _ptr); // producer only

		///
		Ty* pop(); // consumer only

	private:
		MpScUnboundedQueueT<Ty> m_queue;
		Semaphore m_sem;
	};

} // namespace base

#include "inline/mpscqueue.inl"

#endif // BASE_MPSCQUEUE_H_HEADER_GUARD
