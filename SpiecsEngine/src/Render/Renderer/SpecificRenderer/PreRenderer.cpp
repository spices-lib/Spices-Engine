#include "Pchheader.h"
#include "PreRenderer.h"

namespace Spiecs {

	void PreRenderer::CreateDescriptorSet()
	{
		DescriptorSetBuilder{ this }
		.AddBuffer<PreR::View>(0, 0, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT)
		.AddBuffer<PreR::Input>(0, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT)
		.Build();
	}

	void PreRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		/**
		* @breif Instance a RenderBehaverBuilder.
		*/
		RenderBehaverBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		/**
		* @brief Update View.
		*/
		builder.UpdateBuffer<PreR::View>(0, 0, [&](auto& ubo) {
			auto& [invViewMatrix, projectionMatrix] = GetActiveCameraMatrix(frameInfo);
			ImVec2 sceneTextureSize = SlateSystem::GetRegister()->GetViewPort()->GetPanelSize();
			VkExtent2D windowSize = m_Device->GetSwapChainSupport().surfaceSize;

			PreR::View view;
			view.projection = projectionMatrix;
			view.view = glm::inverse(invViewMatrix);
			view.inView = invViewMatrix;
			view.sceneTextureSize = { sceneTextureSize.x, sceneTextureSize.y, 1.0f / sceneTextureSize.x, 1.0f / sceneTextureSize.y };
			view.windowSize = { windowSize.width, windowSize.height, 1.0f / windowSize.width, 1.0 / windowSize.height };
		});

		/**
		* @brief Update Input.
		*/
		builder.UpdateBuffer<PreR::Input>(0, 1, [&](auto& ubo) {
			auto& [x, y] = SlateSystem::GetRegister()->GetViewPort()->GetMousePosInViewport();

			PreR::Input input;
			input.gameTime = ts.gt();
			input.frameTime = ts.ft();
			input.mousePos = glm::vec4((float)x, (float)y, 1.0f / float(x), 1.0f / float(y));
		});
	}
}