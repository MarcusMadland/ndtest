/*
 * Copyright 2010-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/base/blob/master/LICENSE
 */

#ifndef BASE_URL_H_HEADER_GUARD
#define BASE_URL_H_HEADER_GUARD

#include "string.h"

namespace base
{
	///
	class UrlView
	{
	public:
		enum Enum
		{
			Scheme,
			UserName,
			Password,
			Host,
			Port,
			Path,
			Query,
			Fragment,

			Count
		};

		///
		UrlView();

		///
		void clear();

		///
		bool parse(const StringView& _url);

		///
		const StringView& get(Enum _token) const;

	private:
		StringView m_tokens[Count];
	};

	///
	void urlEncode(char* _out, uint32_t _max, const StringView& _str);

} // namespace base

#endif // BASE_URL_H_HEADER_GUARD
