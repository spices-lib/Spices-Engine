/**
* @file HttpContext.cpp.
* @brief The HttpContext Class Implementation.
* @author Spices & Muduo.
*/

#include "Pchheader.h"
#include "HttpContext.h"

namespace Spices {

namespace Net {

	bool HttpContext::ParseRequest(Buffer* buf)
	{
		SPICES_PROFILE_ZONE;

		bool ok = true;
		bool hasMore = true;
		while (hasMore)
		{
			if (m_RequestParseState == RequestParseState::ExpectRequestLine)
			{

			}
		}

		return ok;
	}

	void HttpContext::Reset()
	{
		SPICES_PROFILE_ZONE;

		m_RequestParseState = RequestParseState::ExpectRequestLine;

		HttpRequest dummy;
		m_HttpRequest.Swap(dummy);
	}

	bool HttpContext::ProcessRequestLine(const char* begin, const char* end)
	{
		SPICES_PROFILE_ZONE;

		bool succeed = false;
		const char* start = begin;
		const char* space = std::find(start, end, ' ');

		return succeed;
	}

}

}