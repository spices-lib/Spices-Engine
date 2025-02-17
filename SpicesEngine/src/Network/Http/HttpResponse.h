/**
* @file HttpResponse.h.
* @brief The HttpResponse Class Definitions.
* @author Spices & Muduo.
*/

#pragma once
#include "Core/Core.h"

namespace Spices {

namespace Net {

	/**
	* @brief Forward Declare.
	*/
	class Buffer;

	/**
	* @brief Http Response.
	*/
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

		/**
		* @brief Constructor Function.
		* @param[in] close Is should be close.
		*/
		explicit HttpResponse(bool close)
			: m_StatusCode(StatusCode::UnKnown)
			, m_CloseConnection(close)
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~HttpResponse() = default;

		/**
		* @brief Set StatusCode.
		* @param[in] code StatusCode.
		*/
		void SetStatusCode(StatusCode code);

		/**
		* @brief Set StatusMessage.
		* @param[in] message StatusMessage.
		*/
		void SetStatusMessage(const std::string& message);

		/**
		* @brief Set CloseConnection.
		* @param[in] on CloseConnection.
		*/
		void SetCloseConnection(bool on);

		/**
		* @brief Get CloseConnection.
		* @return Returns CloseConnection.
		*/
		bool CloseConnection() const;

		/**
		* @brief Set ContentType.
		* @param[in] contentType ContentType.
		*/
		void SetContentType(const std::string& contentType);

		/**
		* @brief Add a header.
		* @param[in] name header name.
		* @param[in] value header value.
		*/
		void AddHeader(const std::string& name, const std::string& value);

		/**
		* @brief Set body.
		* @param[in] body .
		*/
		void SetBody(const std::string& body);

		/**
		* @brief Append this response to Buffer.
		* @param[in] output output buffer.
		*/
		void AppendToBuffer(Buffer* output);

	private:

		/**
		* @brief This response header.
		*/
		std::unordered_map<std::string, std::string> m_Header;

		/**
		* @brief StatusCode.
		*/
		StatusCode m_StatusCode;

		/**
		* @brief StatusMessage.
		*/
		std::string m_StatusMessage;

		/**
		* @brief CloseConnection.
		*/
		bool m_CloseConnection;

		/**
		* @brief Body.
		*/
		std::string m_Body;
	};

}

}