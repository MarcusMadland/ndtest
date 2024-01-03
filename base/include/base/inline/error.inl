/*
 * Copyright 2010-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/base/blob/master/LICENSE
 */

#ifndef BASE_ERROR_H_HEADER_GUARD
#	error "Must be included from base/error!"
#endif // BASE_ERROR_H_HEADER_GUARD

#include <base/debug.h>

namespace base
{
	inline Error::Error()
		: m_code(0)
	{
	}

	inline void Error::reset()
	{
		m_code = 0;
		m_msg.clear();
	}

	inline void Error::setError(ErrorResult _errorResult, const StringView& _msg)
	{
		BASE_ASSERT(0 != _errorResult.code, "Invalid ErrorResult passed to setError!");

		if (!isOk() )
		{
			return;
		}

		m_code = _errorResult.code;
		m_msg  = _msg;
	}

	inline bool Error::isOk() const
	{
		return 0 == m_code;
	}

	inline ErrorResult Error::get() const
	{
		ErrorResult result = { m_code };
		return result;
	}

	inline const StringView& Error::getMessage() const
	{
		return m_msg;
	}

	inline bool Error::operator==(const ErrorResult& _rhs) const
	{
		return _rhs.code == m_code;
	}

	inline bool Error::operator!=(const ErrorResult& _rhs) const
	{
		return _rhs.code != m_code;
	}

	inline ErrorIgnore::operator Error*()
	{
		return this;
	}

	inline ErrorAssert::~ErrorAssert()
	{
		BASE_ASSERT(isOk(), "Error: 0x%08x `%S`"
			, get().code
			, &getMessage()
			);
	}

	inline ErrorFatal::operator Error*()
	{
		return this;
	}

	inline ErrorFatal::~ErrorFatal()
	{
		if (!isOk() )
		{
			printf("Error: 0x%08x `%S`"
				, get().code
				, &getMessage()
				);

			exit(kExitFailure);
		}
	}

	inline ErrorAssert::operator Error*()
	{
		return this;
	}

	inline ErrorScope::ErrorScope(Error* _err, const StringView& _name)
		: m_err(_err)
		, m_name(_name)
	{
		BASE_UNUSED(m_err);
		BASE_ASSERT(NULL != _err, "_err can't be NULL");
	}

	inline ErrorScope::~ErrorScope()
	{
		if (m_name.isEmpty() )
		{
			BASE_ASSERT(m_err->isOk(), "Error: 0x%08x `%S`"
				, m_err->get().code
				, &m_err->getMessage()
				);
		}
		else
		{
			BASE_ASSERT(m_err->isOk(), "Error: %S - 0x%08x `%S`"
				, &m_name
				, m_err->get().code
				, &m_err->getMessage()
				);
		}
	}

	inline const StringView& ErrorScope::getName() const
	{
		return m_name;
	}

} // namespace base
