#include "Pchheader.h"
#include "HttpRequest.h"

namespace Spices {

namespace Net {

	void HttpRequest::SetPath(const std::string& path)
	{
		SPICES_PROFILE_ZONE;

		m_Path = path;
	}

	void HttpRequest::SetMethod(Method method)
	{
		SPICES_PROFILE_ZONE;

		m_Method = method;
	}

	void HttpRequest::SetVersion(Version version)
	{
		SPICES_PROFILE_ZONE;

		m_Version = version;
	}

	void HttpRequest::AddParameter(const std::string& name, const std::string& value)
	{
		SPICES_PROFILE_ZONE;

		m_Parameters[name] = value;
	}

	void HttpRequest::AddHeader(const std::string& name, const std::string& value)
	{
		SPICES_PROFILE_ZONE;

		m_Headers[name] = value;
	}

	std::string HttpRequest::GetHeader(const std::string& name) const
	{
		SPICES_PROFILE_ZONE;

		if (m_Headers.find(name) == m_Headers.end())
		{
			std::stringstream ss;
			ss << name << " is not found in Http Header";

			SPICES_CORE_WARN(ss.str())
			return "";
		}

		return m_Headers.find(name)->second;
	}

	void HttpRequest::Swap(HttpRequest& rhs)
	{
		SPICES_PROFILE_ZONE;

		std::swap(m_Method, rhs.m_Method);
		std::swap(m_Version, rhs.m_Version);
		m_Path.swap(rhs.m_Path);
		
		m_Headers.swap(rhs.m_Headers);
	}

}

}