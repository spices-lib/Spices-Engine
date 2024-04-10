#include "Pchheader.h"
#include "Texture2D.h"
#include "Render/Vulkan/VulkanRenderBackend.h"

namespace Spiecs {

	Texture2D::Texture2D(const std::string& path)
		: Texture(path)
	{
		TextureLoader::Load(m_ResourcePath, this);
	}
}