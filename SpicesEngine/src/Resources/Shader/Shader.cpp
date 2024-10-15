/**
* @file Shader.cpp.
* @brief The Shader Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "Shader.h"

namespace Spices {

	Shader::Shader(std::string name, ShaderStage stage)
		: m_ShaderName(name)
		, m_ShaderStage(stage)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @breif Call Dserialize() while in Constructor.
		*/
		Deserialize();
	}

	Shader::Shader(std::string name, const std::string& stage)
		: m_ShaderName(name)
		, m_ShaderStage(ShaderHelper::ToStage(stage))
	{
		SPICES_PROFILE_ZONE;

		/**
		* @breif Call Dserialize() while in Constructor.
		*/
		Deserialize();
	}

	Shader::~Shader()
	{}

	void Shader::Deserialize()
	{
		SPICES_PROFILE_ZONE;

		if (m_ShaderName.empty())
		{
			SPICES_CORE_WARN("Shader::m_ShaderName is empty.");
			return;
		}

		ShaderLoader::Load(m_ShaderName, m_ShaderStage, this);
	}
}