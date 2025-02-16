/**
* @file HttpRequest.h.
* @brief The HttpRequest Class Definitions.
* @author Spices & Muduo.
*/

#pragma once
#include "Core/Core.h"

namespace Spices {

namespace Net {

	/**
	* @brief Http Request body.
	*/
	class HttpRequest
	{
	public:

		enum class Method
		{
			Invalid = 0,
			Get = 1,
			Post = 2,
			Head = 3,
			Put = 4,
			Delete = 5
		};

		enum class Version
		{
			UnKonwn = 0,
			HTTP10 = 1,
			HTTP11 = 2
		};

	public:

		/**
		* @brief Constructor Function.
		* @param[in] version Http Version.
		*/
		HttpRequest()
			: m_Method(Method::Invalid)
			, m_Version(Version::UnKonwn)
			, m_Path("")
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~HttpRequest() = default;

		/**
		* @brief Set Http path.
		* @param[in] path Http Path.
		*/
		void SetPath(const std::string& path);

		/**
		* @brief Get Http path.
		* @return Returns Http Path.
		*/
		const std::string& GetPath() { return m_Path; }

		/**
		* @brief Set Http Method.
		* @param[in] method Http Method.
		*/
		void SetMethod(Method method);

		/**
		* @brief Get Http Method.
		* @return Returns Http Method.
		*/
		const Method GetMethod() const { return m_Method; }
 
		/**
		* @brief Set Http Version.
		* @param[in] version Http Version.
		*/
		void SetVersion(Version version);

		/**
		* @brief Get Http Version.
		* @return Returns Http Version.
		*/
		const Version GetVersion() const { return m_Version; }

		/**
		* @brief Add a Parameter to this HttpRequest.
		* @param[in] name Parameter name.
		* @param[in] value Parameter value.
		*/
		void AddParameter(const std::string& name, const std::string& value);

		/**
		* @brief Add a Header to this HttpRequest.
		* @param[in] name Header name.
		* @param[in] value Header value.
		*/
		void AddHeader(const std::string& name, const std::string& value);

		/**
		* @brief Get a Header from this HttpRequest.
		* @param[in] name Header name.
		* return Returns Header name.
		*/
		std::string GetHeader(const std::string& name) const;

		/**
		* @brief Swap with another HttpRequest.
		* @param[in] rhs another HttpRequest.
		*/
		void Swap(HttpRequest& rhs);

	private:

		/**
		* @brief Http Method.
		*/
		Method m_Method;

		/**
		* @brief Http Version.
		*/
		Version m_Version;

		/**
		* @brief Http Path.
		*/
		std::string m_Path;

		/**
		* @brief Http Parameters.
		*/
		std::unordered_map<std::string, std::string> m_Parameters;

		/**
		* @brief Http Headers.
		*/
		std::unordered_map<std::string, std::string> m_Headers;

	};

}

}
