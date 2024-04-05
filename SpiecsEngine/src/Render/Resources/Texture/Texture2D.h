#pragma once
#include "Core/Core.h"
#include "Render/Vulkan/VulkanImage.h"

namespace Spiecs {

	class Texture2D
	{
	public:
		Texture2D() {};
		Texture2D(const std::string& texturePath);
		virtual ~Texture2D() {};

	private:
		std::unique_ptr<VulkanImage> m_Resources;
	};
}