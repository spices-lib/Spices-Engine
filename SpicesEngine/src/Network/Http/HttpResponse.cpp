/**
* @file HttpResponse.cpp.
* @brief The HttpResponse Class Implementation.
* @author Spices & Muduo.
*/

#include "Pchheader.h"
#include "HttpResponse.h"
#include "../Net/Buffer.h"

namespace Spices {

namespace Net {

	void HttpResponse::SetStatusCode(StatusCode code)
	{
		SPICES_PROFILE_ZONE;

		m_StatusCode = code;
	}

	void HttpResponse::SetStatusMessage(const std::string& message)
	{
		SPICES_PROFILE_ZONE;

		m_StatusMessage = message;
	}

	void HttpResponse::SetCloseConnection(bool on)
	{
		SPICES_PROFILE_ZONE;

		m_CloseConnection = on;
	}

	bool HttpResponse::CloseConnection() const
	{
		SPICES_PROFILE_ZONE;

		return m_CloseConnection;
	}

	void HttpResponse::SetContentType(const std::string& contentType)
	{
		SPICES_PROFILE_ZONE;

		AddHeader("Content-Type", contentType);
	}

	void HttpResponse::AddHeader(const std::string& name, const std::string& value)
	{
		SPICES_PROFILE_ZONE;

		m_Header[name] = value;
	}

	void HttpResponse::SetBody(const std::string& body)
	{
		SPICES_PROFILE_ZONE;

		m_Body = body;
	}

	void HttpResponse::AppendToBuffer(Buffer* output)
	{
		SPICES_PROFILE_ZONE;

		std::stringstream ss;
		ss << "HTTP/1.1 " << m_StatusCode << m_StatusMessage << "\r\n";

		output->Append(ss.str());

		if (m_CloseConnection)
		{
			output->Append("Connection: close\r\n");
		}
		else
		{
			std::stringstream v;
			v << "Content-Length: " << m_Body.size() << "\r\n" << "Connection: Keep-Alive\r\n";

			output->Append(v.str());
		}

		for (const auto& header : m_Header)
		{
			std::stringstream v;
			v << header.first << ": " << header.second << "\r\n";

			output->Append(v.str());
		}

		output->Append("\r\n");
		output->Append(m_Body);

	}

}

}