/**
* @file RendererResourcePool.cpp.
* @brief The RendererResourcePool Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "RendererResourcePool.h"
#include "Render/Vulkan/VulkanImage.h"

namespace Spices {

	void RendererResourcePool::OnSlateResize(uint32_t width, uint32_t height) const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Call all Resource's OnResized().
		*/
		for (auto& pair : m_RendererResource)
		{
			pair.second->OnResized(width, height);
		}
	}

	VkDescriptorImageInfo* RendererResourcePool::AccessResource(const RendererResourceCreateInfo& info)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Create one if isn't exist.
		*/
		if (m_RendererResource.find(info.name) == m_RendererResource.end())
		{
			m_RendererResource[info.name] = std::make_unique<RendererResource>(info);
		}

		return m_RendererResource[info.name]->GetTexture()->GetResource<VulkanImage>()->GetImageInfo();
	}

	std::shared_ptr<VulkanImage> RendererResourcePool::AccessRowResource(const std::string& name)
	{
		SPICES_PROFILE_ZONE;

		return m_RendererResource[name]->GetTexture()->GetResource<VulkanImage>();
	}
}