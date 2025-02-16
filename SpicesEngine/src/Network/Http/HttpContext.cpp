#include "Pchheader.h"
#include "HttpContext.h"

namespace Spices {

namespace Net {

	bool HttpContext::ParseRequest(Buffer* buf)
	{
		return false;
	}

	void HttpContext::Reset()
	{
		m_RequestParseState = RequestParseState::ExpectRequestLine;

		HttpRequest dummy;
		m_HttpRequest.Swap(dummy);
	}

}

}