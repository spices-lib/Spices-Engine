/**
* @file HttpRequest.h.
* @brief The HttpRequest Class Definitions.
* @author Spices & Muduo.
*/

#pragma once
#include "Core/Core.h"

namespace Spices {

	/**
	* @brief Http Request body.
	*/
	class HttpRequest
	{
	public:

		enum class Method
		{
			Invalid = 0,
			Get     = 1,
			Post    = 2,
			Head    = 3,
			Put     = 4,
			Delete  = 5
		};

		enum class Version
		{
			UnKonwn = 0,
			HTTP10  = 1,
			HTTP11  = 2
		};

	public:

		/**
		* @brief Constructor Function.
		* @param[in] method Http Method.
		* @param[in] version Http Version.
		* @param[in] path Http Path.
		*/
		HttpRequest(Method method, Version version, const std::string& path)
			: m_Method(method)
			, m_Version(version)
			, m_Path(path)
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~HttpRequest() = default;

		/**
		* @brief Add a Parameter to this HttpRequest.
		* @param[in] name Parameter name.
		* @param[in] value Parameter value.
		*/
		void AddParameter(const std::string& name, const std::string& value)
		{
			m_Parameters[name] = value;
		}

		/**
		* @brief Add a Header to this HttpRequest.
		* @param[in] name Header name.
		* @param[in] value Header value.
		*/
		void AddHeader(const std::string& name, const std::string& value)
		{
			m_Headers[name] = value;
		}

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
