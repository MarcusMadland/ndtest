/*
 * Copyright 2010-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/base/blob/master/LICENSE
 */

#ifndef BASE_SPSCQUEUE_H_HEADER_GUARD
#define BASE_SPSCQUEUE_H_HEADER_GUARD

#include "allocator.h"
#include "cpu.h"
#include "semaphore.h"

namespace base
{
	///
	class SpScUnboundedQueue
	{
		BASE_CLASS(SpScUnboundedQueue
			, NO_DEFAULT_CTOR
			, NO_COPY
			);

	public:
		///
		SpScUnboundedQueue(AllocatorI* _allocator);

		///
		~SpScUnboundedQueue();

		///
		void push(void* _ptr);

		///
		void* peek();

		///
		void* pop();

	private:
		struct Node
		{
			///
			Node(void* _ptr);

			void* m_ptr;
			Node* m_next;
		};

		AllocatorI* m_allocator;
		Node* m_first;
		Node* m_divider;
		Node* m_last;
	};

	///
	template<typename Ty>
	class SpScUnboundedQueueT
	{
		BASE_CLASS(SpScUnboundedQueueT
			, NO_DEFAULT_CTOR
			, NO_COPY
			);

	public:
		///
		SpScUnboundedQueueT(AllocatorI* _allocator);

		///
		~SpScUnboundedQueueT();

		///
		void push(Ty* _ptr);

		///
		Ty* peek();

		///
		Ty* pop();

	private:
		SpScUnboundedQueue m_queue;
	};

#if BASE_CONFIG_SUPPORTS_THREADING
	///
	class SpScBlockingUnboundedQueue
	{
		BASE_CLASS(SpScBlockingUnboundedQueue
			, NO_DEFAULT_CTOR
			, NO_COPY
			);

	public:
		///
		SpScBlockingUnboundedQueue(AllocatorI* _allocator);

		///
		~SpScBlockingUnboundedQueue();

		///
		void push(void* _ptr); // producer only

		///
		void* peek(); // consumer only

		///
		void* pop(int32_t _msecs = -1); // consumer only

	private:
		Semaphore m_count;
		SpScUnboundedQueue m_queue;
	};

	///
	template<typename Ty>
	class SpScBlockingUnboundedQueueT
	{
		BASE_CLASS(SpScBlockingUnboundedQueueT
			, NO_DEFAULT_CTOR
			, NO_COPY
			);

	public:
		///
		SpScBlockingUnboundedQueueT(AllocatorI* _allocator);

		///
		~SpScBlockingUnboundedQueueT();

		///
		void push(Ty* _ptr); // producer only

		///
		Ty* peek(); // consumer only

		///
		Ty* pop(int32_t _msecs = -1); // consumer only

	private:
		SpScBlockingUnboundedQueue m_queue;
	};
#endif // BASE_CONFIG_SUPPORTS_THREADING

} // namespace base

#include "inline/spscqueue.inl"

#endif // BASE_SPSCQUEUE_H_HEADER_GUARD
