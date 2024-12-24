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
	{
		SPICES_PROFILE_ZONE;

        /**
        * @brief Get Texture Info.
        */
        VkDescriptorImageInfo* info = VulkanRenderBackend::GetRendererResourcePool()->AccessResource({ textureName });

        /**
        * @brief Create Texture DescriptorSet.
        */
        m_TextureID = reinterpret_cast<ImTextureID>(ImGui_ImplVulkan_AddTexture(info->sampler, info->imageView, info->imageLayout));

        /**
        * @brief Load material.
        */
        m_Material = ResourcePool<Material>::Load<Material>(materialName, materialName);
        m_Material->BuildMaterial();
	}

    SlateImage::~SlateImage()
    {
        SPICES_PROFILE_ZONE;

        /**
       * @brief Free old Texture image DescriptorSet.
       */
        ImGui_ImplVulkan_RemoveTexture(reinterpret_cast<VkDescriptorSet>(m_TextureID));

        /**
        * @brief Unload useless material.
        */
        ResourcePool<Material>::UnLoad(m_Material->GetName());
    }
}