#pragma once
#include "Core/Core.h"
#include "Render/FrameInfo.h"

#include "Render/Vulkan/VulkanRenderBackend.h"

namespace Spiecs {

	class RenderFrontend
	{
	public:
		RenderFrontend();
		virtual ~RenderFrontend() {};

		RenderFrontend(const RenderFrontend&) = delete;
		RenderFrontend& operator=(const RenderFrontend&) = delete;

		bool isWindowClosed() { return m_RenderBackend->isWindowClosed(); };
		void WaitIdle() { m_RenderBackend->WaitIdle(); };

		void beginFrame(FrameInfo& frameInfo);
		void endFrame(FrameInfo& frameInfo);

		void DrawTest(FrameInfo& frameInfo);

		// Temp:
		VulkanState& GetState() { return VulkanRenderBackend::GetState(); };

	private:
		std::unique_ptr<VulkanRenderBackend> m_RenderBackend;
	};
}