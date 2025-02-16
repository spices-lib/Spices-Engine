#pragma once
#include "Core/Core.h"
#include "HttpRequest.h"

namespace Spices {

namespace Net {

	class Buffer;

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
		{}

		virtual ~HttpContext() = default;

		/**
		* @brief Get HttpRequest.
		* @reutrn Returns HttpRequest.
		*/
		const HttpRequest& GetRequest() { return m_HttpRequest; }

		bool ParseRequest(Buffer* buf);

		bool GotAll() { return m_RequestParseState == RequestParseState::GotAll; }

		void Reset();

	private:

		RequestParseState m_RequestParseState;
		HttpRequest m_HttpRequest;
	};

}

}