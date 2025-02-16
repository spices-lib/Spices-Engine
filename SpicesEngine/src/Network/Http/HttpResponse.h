#pragma once
#include "Core/Core.h"

namespace Spices {

namespace Net {

	class HttpResponse
	{
	public:

		enum class StatusCode
		{
			UnKnown          = 0,
			Ok               = 200,
			MovedPermanently = 301,
			BadRequest       = 400,
			NotFound         = 404,
		};

	public:

		explicit HttpResponse(bool close)
			: m_StatusCode(StatusCode::UnKnown)
			, m_CloseConnection(close)
		{}

		virtual ~HttpResponse() = default;

	private:

		std::unordered_map<std::string, std::string> m_Header;
		StatusCode m_StatusCode;
		std::string m_StatusMessage;
		bool m_CloseConnection;
		std::string m_Body;
	};

}

}