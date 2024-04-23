/**
* @file RendererResourcePool.cpp.
* @brief The RendererResourcePool Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "RendererResourcePool.h"
#include "Render/Vulkan/VulkanImage.h"

namespace Spiecs {

	void RendererResourcePool::OnWindowResized(uint32_t width, uint32_t height)
	{
		/**
		* @brief Call all Resource's OnResized().
		*/
		for (auto& pair : m_RendererResource)
		{
			pair.second->OnResized(width, height);
		}
	}

	VkImageView& RendererResourcePool::AccessResource(const std::string& name, const RendererResourceCreateInfo& info)
	{
		/**
		* @brief Create one if isn't exist.
		*/
		if (m_RendererResource.find(name) == m_RendererResource.end())
		{
			m_RendererResource[name] = std::make_unique<RendererResource>(info);
		}

		return m_RendererResource[name]->GetTexture()->GetResource<VulkanImage>()->GetView();
	}

	VkImageView& RendererResourcePool::AccessDepthResource(const RendererResourceCreateInfo& info)
	{
		/**
		* @brief Create one if isn't exist.
		*/
		if (m_RendererResource.find("Depth") == m_RendererResource.end())
		{
			m_RendererResource["Depth"] = std::make_unique<RendererResource>(info);
		}

		return m_RendererResource["Depth"]->GetTexture()->GetResource<VulkanImage>()->GetView();
	}
}