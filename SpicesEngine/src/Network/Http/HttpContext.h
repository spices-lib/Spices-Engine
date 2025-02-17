/**
* @file HttpContext.h.
* @brief The HttpContext Class Definitions.
* @author Spices & Muduo.
*/

#pragma once
#include "Core/Core.h"
#include "HttpRequest.h"

namespace Spices {

namespace Net {

	class Buffer;

	/**
	* @brief Http Context.
	*/
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

		/**
		* @brief Constructor Function.
		*/
		HttpContext() : m_RequestParseState(RequestParseState::ExpectRequestLine) {}

		/**
		* @brief Destructor Function.
		*/
		virtual ~HttpContext() = default;

		/**
		* @brief Get HttpRequest.
		* @reutrn Returns HttpRequest.
		*/
		const HttpRequest& GetRequest() { return m_HttpRequest; }

		bool ParseRequest(Buffer* buf);

		/**
		* @brief Determine if RequestParseState is GotAll.
		* @return Returns true if RequestParseState is GotAll.
		*/
		bool GotAll() { return m_RequestParseState == RequestParseState::GotAll; }

		void Reset();

	private:

		bool ProcessRequestLine(const char* begin, const char* end);

	private:

		/**
		* @brief RequestParseState.
		*/
		RequestParseState m_RequestParseState;

		/**
		* @brief HttpRequest.
		*/
		HttpRequest m_HttpRequest;
	};

}

}