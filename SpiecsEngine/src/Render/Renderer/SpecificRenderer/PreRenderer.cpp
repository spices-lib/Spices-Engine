#include "Pchheader.h"
#include "PreRenderer.h"

namespace Spiecs {

	void PreRenderer::CreateRendererPass()
	{
		RendererPassBuilder{ "PreRenderer", this }
		.AddSubPass("PreRenderer")
		.EndSubPass()	
		.Build();
	}

	void PreRenderer::CreateDescriptorSet()
	{
 		DescriptorSetBuilder{ "PreRenderer", this }
		.AddBuffer<PreR::View>(0, 0, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT)
		.AddBuffer<PreR::SpiecsInput>(0, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT)
		.Build();
	}

	void PreRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		/**
		* @breif Instance a RenderBehaverBuilder.
		*/
		RenderBehaverBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.BeginRenderPass();

		/**
		* @brief Update View.
		*/
		builder.UpdateBuffer<PreR::View>(0, 0, [&](auto& ubo) {
			auto& [invViewMatrix, projectionMatrix] = GetActiveCameraMatrix(frameInfo);
			ImVec2 sceneTextureSize = SlateSystem::GetRegister()->GetViewPort()->GetPanelSize();
			VkExtent2D windowSize = m_Device->GetSwapChainSupport().surfaceSize;

			ubo.projection = projectionMatrix;
			ubo.view = glm::inverse(invViewMatrix);
			ubo.inView = invViewMatrix;
			ubo.sceneTextureSize = { sceneTextureSize.x, sceneTextureSize.y, 1.0f / sceneTextureSize.x, 1.0f / sceneTextureSize.y };
			ubo.windowSize = { windowSize.width, windowSize.height, 1.0f / windowSize.width, 1.0 / windowSize.height };
		});

		/**
		* @brief Update Input.
		*/
		builder.UpdateBuffer<PreR::SpiecsInput>(0, 1, [&](auto& ubo) {
			auto& [x, y] = SlateSystem::GetRegister()->GetViewPort()->GetMousePosInViewport();

			ubo.gameTime = ts.gt();
			ubo.frameTime = ts.ft();
			ubo.mousePos = glm::vec4((float)x, (float)y, 1.0f / float(x), 1.0f / float(y));
		});

		builder.EndRenderPass();
	}
}