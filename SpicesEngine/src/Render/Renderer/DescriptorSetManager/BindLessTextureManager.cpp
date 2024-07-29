/**
* @file BindLessTextureManager.cpp
* @brief The BindLessTextureManager Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "BindLessTextureManager.h"
#include "Render/Vulkan/VulkanRenderBackend.h"
#include "../../../assets/Shaders/src/Header/ShaderCommon.h"

namespace Spices {

	std::unordered_map<std::string, uint32_t> BindLessTextureManager::m_TextureIDMap;
	std::unordered_map<uint32_t, std::string> BindLessTextureManager::m_TextureInfoMap;

	uint32_t BindLessTextureManager::Registry(const std::string& name)
	{
		SPICES_PROFILE_ZONE;
		
		/**
		* @brief Return ID if texture already registry.
		*/
		if (m_TextureIDMap.find(name) != m_TextureIDMap.end())
		{
			return m_TextureIDMap[name];
		}

		/**
		* @brief Find a empty slot to store texture.
		*/
		else
		{
			for (uint32_t i = 0; i < MAXBINDLESSTEXTURECOUNT; i++)
			{
				if (m_TextureInfoMap.find(i) == m_TextureInfoMap.end())
				{
					m_TextureInfoMap[i] = "";
					m_TextureIDMap[name] = i;
					return i;
				}
			}

			SPICES_CORE_ERROR("Up to Maxium Bindless Texture Number : 65536");
		}
	}

	void BindLessTextureManager::UnRegistry(const std::string& name)
	{
		SPICES_PROFILE_ZONE;
		
		if (m_TextureIDMap.find(name) != m_TextureIDMap.end())
		{
			m_TextureInfoMap.erase(m_TextureIDMap[name]);
			m_TextureIDMap.erase(name);
		}
	}
}