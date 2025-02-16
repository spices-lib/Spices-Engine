#pragma once
#include "Core/Core.h"
#include "HttpRequest.h"

namespace Spices {

namespace Net {

	class HttpContext
	{
	public:

		enum class RequestParseState
		{
			ExpectRequestLine = 0,
			ExpectHeaders     = 1,
			ExpectBody        = 2,
			GotAll            = 3,
		};

	public:

		HttpContext()
			: m_RequestParseState(RequestParseState::ExpectRequestLine)
			, m_HttpRequest("", HttpRequest::Method::Get, HttpRequest::Version::HTTP11)
		{}

		virtual ~HttpContext() = default;


	private:

		RequestParseState m_RequestParseState;
		HttpRequest m_HttpRequest;
	};

}

}