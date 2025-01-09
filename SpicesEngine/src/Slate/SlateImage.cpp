/**
* @file SlateImage.cpp.
* @brief The SlateImage Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "SlateImage.h"
#include "Render/Vulkan/VulkanRenderBackend.h"
#include "Resources/Material/Material.h"
#include "Resources/ResourcePool/ResourcePool.h"

namespace Spices {

	SlateImage::SlateImage(const std::string& textureName, const std::string& materialName)
        : m_TextureName(textureName)
        , m_MaterialName(materialName)
        , m_TextureID(0)
        , m_Material(nullptr)
	{
		SPICES_PROFILE_ZONE;

        ReBuildTextureID();
        ReBuildMaterial();
	}

    SlateImage::~SlateImage()
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Free old Texture image DescriptorSet.
        */
        if (m_TextureID)
        {
            ImGui_ImplVulkan_RemoveTexture(reinterpret_cast<VkDescriptorSet>(m_TextureID));
            m_TextureID = 0;
        }
        
        /**
        * @brief Unload useless material.
        */
        if (m_Material)
        {
            ResourcePool<Material>::UnLoad(m_Material->GetName());
        }
    }

    void SlateImage::ReBuildTextureID()
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Free old Texture image DescriptorSet.
        */
        if (m_TextureID)
        {
            ImGui_ImplVulkan_RemoveTexture(reinterpret_cast<VkDescriptorSet>(m_TextureID));
            m_TextureID = 0;
        }

        /**
        * @brief Get Texture Info.
        */
        VkDescriptorImageInfo* info = VulkanRenderBackend::GetRendererResourcePool()->AccessResource({ m_TextureName });

        /**
        * @brief Create Texture DescriptorSet.
        */
        m_TextureID = reinterpret_cast<ImTextureID>(ImGui_ImplVulkan_AddTexture(info->sampler, info->imageView, info->imageLayout));
    }

    void SlateImage::ReBuildMaterial()
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Unload useless material.
        */
        if (m_Material)
        {
            ResourcePool<Material>::UnLoad(m_Material->GetName());
        }

        /**
        * @brief Load material.
        */
        m_Material = ResourcePool<Material>::Load<Material>(m_MaterialName, m_MaterialName);
        m_Material->BuildMaterial();
    }
}