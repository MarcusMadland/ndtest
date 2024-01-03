/*
 * Copyright 2010-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/base/blob/master/LICENSE
 */

#ifndef BASE_THREAD_H_HEADER_GUARD
#define BASE_THREAD_H_HEADER_GUARD

#include "allocator.h"
#include "mpscqueue.h"

#if BASE_CONFIG_SUPPORTS_THREADING

namespace base
{
	///
	typedef int32_t (*ThreadFn)(class Thread* _self, void* _userData);

	///
	class Thread
	{
		BASE_CLASS(Thread
			, NO_COPY
			);

	public:
		///
		Thread();

		///
		virtual ~Thread();

		/// Create and initialize thread.
		///
		/// @param[in] _fn Thread function.
		/// @param[in] _userData User data passed to thread function.
		/// @param[in] _stackSize Stack size, if zero is passed it will use OS default thread stack
		///   size.
		/// @param[in] _name Thread name used by debugger.
		/// @returns True if thread is created, otherwise returns false.
		///
		bool init(ThreadFn _fn, void* _userData = NULL, uint32_t _stackSize = 0, const char* _name = NULL);

		///
		void shutdown();

		///
		bool isRunning() const;

		///
		int32_t getExitCode() const;

		///
		void setThreadName(const char* _name);

		///
		void push(void* _ptr);

		///
		void* pop();

	private:
		friend struct ThreadInternal;
		int32_t entry();

		BASE_ALIGN_DECL(16, uint8_t) m_internal[64];

		ThreadFn  m_fn;
		void*     m_userData;
		MpScUnboundedBlockingQueue<void> m_queue;
		Semaphore m_sem;
		uint32_t  m_stackSize;
		int32_t   m_exitCode;
		bool      m_running;
	};

	///
	class TlsData
	{
	public:
		///
		TlsData();

		///
		~TlsData();

		///
		void* get() const;

		///
		void set(void* _ptr);

	private:
		BASE_ALIGN_DECL(16, uint8_t) m_internal[64];
	};

} // namespace base

#endif

#endif // BASE_THREAD_H_HEADER_GUARD
