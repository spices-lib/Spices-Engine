#pragma once
#include "Core/Core.h"
#include "Render/FrameInfo.h"
#include "Core/Event/Event.h"
#include "Render/Vulkan/VulkanRenderBackend.h"

namespace Spices {

	class RenderFrontend
	{
	public:
		RenderFrontend();
		virtual ~RenderFrontend() = default;

		RenderFrontend(const RenderFrontend&) = delete;
		RenderFrontend& operator=(const RenderFrontend&) = delete;

		bool isWindowClosed() { return m_RenderBackend->isWindowClosed(); }
		void WaitIdle() { m_RenderBackend->WaitIdle(); }

		void BeginFrame(FrameInfo& frameInfo);
		void EndFrame(FrameInfo& frameInfo);

		void RenderFrame(TimeStep& ts, FrameInfo& frameInfo);
		void OnEvent(Event& event);

	private:
		std::unique_ptr<VulkanRenderBackend> m_RenderBackend;
	};
}