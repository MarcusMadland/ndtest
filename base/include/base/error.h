/*
 * Copyright 2010-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/base/blob/master/LICENSE
 */

#ifndef BASE_ERROR_H_HEADER_GUARD
#define BASE_ERROR_H_HEADER_GUARD

#include "string.h"

#define BASE_ERROR_SET(_ptr, _result, _msg)    \
	BASE_MACRO_BLOCK_BEGIN                     \
		(_ptr)->setError(_result,  "" _msg); \
	BASE_MACRO_BLOCK_END

#define BASE_ERROR_USE_TEMP_WHEN_NULL(_ptr)                           \
	const base::Error tmpError; /* It should not be used directly! */ \
	_ptr = NULL == _ptr ? const_cast<base::Error*>(&tmpError) : _ptr

#define BASE_ERROR_SCOPE(_ptr, ...)                                  \
	BASE_ERROR_USE_TEMP_WHEN_NULL(_ptr);                             \
	base::ErrorScope baseErrorScope(const_cast<base::Error*>(&tmpError), "" __VA_ARGS__)

#define BASE_ERROR_RESULT(_err, _code)                          \
	BASE_STATIC_ASSERT(_code != 0, "ErrorCode 0 is reserved!"); \
	static constexpr base::ErrorResult _err = { _code }

namespace base
{
	///
	struct ErrorResult
	{
		uint32_t code;
	};

	///
	class Error
	{
		BASE_CLASS(Error
			, NO_COPY
			);

	public:
		///
		Error();

		///
		void reset();

		///
		void setError(ErrorResult _errorResult, const StringView& _msg);

		///
		bool isOk() const;

		///
		ErrorResult get() const;

		///
		const StringView& getMessage() const;

		///
		bool operator==(const ErrorResult& _rhs) const;

		///
		bool operator!=(const ErrorResult& _rhs) const;

	private:
		StringView m_msg;
		uint32_t   m_code;
	};

	/// Do nothing even if error is set.
	class ErrorIgnore : public Error
	{
	public:
		///
		operator Error*();
	};

	/// In debug build assert if error is set.
	class ErrorAssert : public Error
	{
	public:
		///
		~ErrorAssert();

		///
		operator Error*();
	};

	/// Exit application if error is set.
	class ErrorFatal : public Error
	{
	public:
		///
		~ErrorFatal();

		///
		operator Error*();
	};

	///
	class ErrorScope
	{
		BASE_CLASS(ErrorScope
			, NO_DEFAULT_CTOR
			, NO_COPY
			);

	public:
		///
		ErrorScope(Error* _err, const StringView& _name);

		///
		~ErrorScope();

		///
		const StringView& getName() const;

	private:
		Error* m_err;
		const StringView m_name;
	};

} // namespace base

#include "inline/error.inl"

#endif // BASE_ERROR_H_HEADER_GUARD
