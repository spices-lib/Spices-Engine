/**
* @file ShaderManager.cpp
* @brief The ShaderManager Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ShaderManager.h"
#include "Render/Vulkan/VulkanRenderBackend.h"

namespace Spices {

	ShaderManagerContainer ShaderManager::m_Shaders;

	std::shared_ptr<VulkanShaderModule> ShaderManager::Registry(const std::string& name, const std::string& stage)
	{
		SPICES_PROFILE_ZONE;

		String2 s2{ name , stage };

		if (m_Shaders.find(s2) != m_Shaders.end())
		{
			return m_Shaders[s2];
		}
		
		m_Shaders[s2] = std::make_shared<VulkanShaderModule>(VulkanRenderBackend::GetState(), name, stage);

		return m_Shaders[s2];
	}

	void ShaderManager::UnLoad(const std::string& name, const std::string& stage)
	{
		SPICES_PROFILE_ZONE;

		String2 s2{ name , stage };

		if (m_Shaders.find(s2) != m_Shaders.end())
		{
			m_Shaders.erase(s2);
		}
	}
}