#include "Pchheader.h"
#include "RendererResourcePool.h"
#include "Render/Vulkan/VulkanImage.h"

namespace Spiecs {

	VkImageView& RendererResourcePool::AccessResource(const std::string& name, const RendererResourceCreateInfo& info)
	{
		if (m_RendererResource.find(name) == m_RendererResource.end())
		{
			m_RendererResource[name] = std::make_unique<RendererResource>(info);
		}

		return m_RendererResource[name]->GetTexture()->GetResource<VulkanImage>()->GetView();
	}

	VkImageView& RendererResourcePool::AccessDepthResource(const RendererResourceCreateInfo& info)
	{
		if (m_RendererResource.find("Depth") == m_RendererResource.end())
		{
			m_RendererResource["Depth"] = std::make_unique<RendererResource>(info);
		}

		return m_RendererResource["Depth"]->GetTexture()->GetResource<VulkanImage>()->GetView();
	}
}