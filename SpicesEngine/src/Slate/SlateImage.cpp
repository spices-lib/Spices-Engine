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
        auto ptr = VulkanRenderBackend::GetRendererResourcePool()->AccessRowResource({ textureName });
        ptr->CreateDescriptorSet(0);

        /**
        * @brief Create Texture DescriptorSet.
        */
        m_TextureID = reinterpret_cast<ImTextureID>(ptr->GetDescriptorSet());

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
        * @brief Unload useless material.
        */
        ResourcePool<Material>::UnLoad(m_Material->GetName());
    }
}