#pragma once
#include "Core/Core.h"

namespace Spices {

namespace Net {

	class Buffer;

	class HttpResponse
	{
	public:

		enum StatusCode
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

		void SetStatusCode(StatusCode code);
		void SetStatusMessage(const std::string& message);
		void SetCloseConnection(bool on);
		bool CloseConnection() const;
		void SetContentType(const std::string& contentType);
		void AddHeader(const std::string& name, const std::string& value);
		void SetBody(const std::string& body);
		void AppendToBuffer(Buffer* output);

	private:

		std::unordered_map<std::string, std::string> m_Header;
		StatusCode m_StatusCode;
		std::string m_StatusMessage;
		bool m_CloseConnection;
		std::string m_Body;
	};

}

}