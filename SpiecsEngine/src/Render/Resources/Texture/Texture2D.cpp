#include "pchheader.h"
#include "Texture2D.h"
#include "Render/Vulkan/VulkanRenderBackend.h"

namespace Spiecs {

	Texture2D::Texture2D(const std::string& texturePath)
	{
		m_Resources = std::make_unique<VulkanImage>(VulkanRenderBackend::GetState(), texturePath);
	}
}