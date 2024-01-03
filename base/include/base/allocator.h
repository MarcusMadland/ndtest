/*
 * Copyright 2010-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/base/blob/master/LICENSE
 */

#ifndef BASE_ALLOCATOR_H_HEADER_GUARD
#define BASE_ALLOCATOR_H_HEADER_GUARD

#include "base.h"
#include "uint32_t.h"

#define BASE_NEW(_allocator, _type)                 BASE_PLACEMENT_NEW(base::alloc(_allocator, sizeof(_type) ), _type)
#define BASE_ALIGNED_NEW(_allocator, _type, _align) BASE_PLACEMENT_NEW(base::alloc(_allocator, sizeof(_type), _align), _type)
#define BASE_PLACEMENT_NEW(_ptr, _type)             ::new(base::PlacementNew, _ptr) _type

void* operator new(size_t, base::PlacementNewTag, void* _ptr);
void  operator delete(void*, base::PlacementNewTag, void*) throw();

namespace base
{
	/// Abstract allocator interface.
	///
	struct BASE_NO_VTABLE AllocatorI
	{
		///
		virtual ~AllocatorI() = 0;

		/// Allocates, resizes, or frees memory block.
		///
		/// @remark
		///  - Allocate memory block: _ptr == NULL && size > 0
		///  -   Resize memory block: _ptr != NULL && size > 0
		///  -     Free memory block: _ptr != NULL && size == 0
		///
		/// @param[in] _ptr If _ptr is NULL new block will be allocated. If _ptr is not-NULL, and
		///   _size is not 0, memory block will be resized.
		/// @param[in] _size If _ptr is set, and _size is 0, memory will be freed.
		/// @param[in] _align Alignment.
		/// @param[in] _filePath Debug file path info.
		/// @param[in] _line Debug file line info.
		///
		virtual void* realloc(
			  void* _ptr
			, size_t _size
			, size_t _align
			, const char* _filePath
			, uint32_t _line
			) = 0;
	};

	///
	class DefaultAllocator : public AllocatorI
	{
	public:
		///
		DefaultAllocator();

		///
		virtual ~DefaultAllocator();

		///
		virtual void* realloc(
			  void* _ptr
			, size_t _size
			, size_t _align
			, const char* _filePath
			, uint32_t _line
			) override;
	};

	/// Aligns pointer to nearest next aligned address. _align must be power of two.
	void* alignPtr(
		  void* _ptr
		, size_t _extra
		, size_t _align = 0
		);

	/// Allocate memory.
	void* alloc(
		  AllocatorI* _allocator
		, size_t _size
		, size_t _align = 0
		, const Location& _location = Location::current()
		);

	/// Free memory.
	void free(
		  AllocatorI* _allocator
		, void* _ptr
		, size_t _align = 0
		, const Location& _location = Location::current()
		);

	/// Resize memory block.
	void* realloc(
		  AllocatorI* _allocator
		, void* _ptr
		, size_t _size
		, size_t _align = 0
		, const Location& _location = Location::current()
		);

	/// Allocate memory with specific alignment.
	void* alignedAlloc(
		  AllocatorI* _allocator
		, size_t _size
		, size_t _align
		, const Location& _location = Location::current()
		);

	/// Free memory that was allocated with aligned allocator.
	void alignedFree(
		  AllocatorI* _allocator
		, void* _ptr
		, size_t /*_align*/
		, const Location& _location = Location::current()
		);

	/// Resize memory block that was allocated with aligned allocator.
	void* alignedRealloc(
		  AllocatorI* _allocator
		, void* _ptr
		, size_t _size
		, size_t _align
		, const Location& _location = Location::current()
		);

	/// Delete object with specific allocator.
	template <typename ObjectT>
	void deleteObject(
		  AllocatorI* _allocator
		, ObjectT* _object
		, size_t _align = 0
		, const Location& _location = Location::current()
		);

} // namespace base

#include "inline/allocator.inl"

#endif // BASE_ALLOCATOR_H_HEADER_GUARD
