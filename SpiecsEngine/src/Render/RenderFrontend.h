#pragma once
#include "Core/Core.h"

#include "Render/Vulkan/VulkanRenderBackend.h"

namespace Spiecs {

	class RenderFrontend
	{
	public:
		RenderFrontend();
		virtual ~RenderFrontend() {};

		RenderFrontend(const RenderFrontend&) = delete;
		RenderFrontend& operator=(const RenderFrontend&) = delete;

	private:
		std::unique_ptr<VulkanRenderBackend> m_RenderBackend;
	};
}