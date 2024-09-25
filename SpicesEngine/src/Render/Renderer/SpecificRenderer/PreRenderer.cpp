#include "Pchheader.h"
#include "PreRenderer.h"

namespace Spices {

	void PreRenderer::CreateRendererPass()
	{
		SPICES_PROFILE_ZONE;

		RendererPassBuilder{ "PreRenderer", this }
		.AddSubPass("PreRenderer")
		.EndSubPass()	
		.Build();
	}

	void PreRenderer::CreateDescriptorSet()
	{
		SPICES_PROFILE_ZONE;

 		DescriptorSetBuilder{ "PreRenderer", this }
		.AddUniformBuffer(0, 0, sizeof(SpicesShader::View), VK_SHADER_STAGE_ALL)
		.AddUniformBuffer(0, 1, sizeof(SpicesShader::Input), VK_SHADER_STAGE_ALL)
		.AddBindLessTexture<Texture2D>(BINDLESS_TEXTURE_SET, BINDLESS_TEXTURE_BINDING, VK_SHADER_STAGE_ALL, { "default.jpg" })
		.Build();
	}

	PreRenderer::~PreRenderer()
	{
		DescriptorSetManager::UnLoadForce(m_RendererName);
	}

	void PreRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		RenderBehaveBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.BeginRenderPass();

		builder.UpdateUniformBuffer<SpicesShader::View>(0, 0, [&](auto& ubo) {
			auto [ invViewMatrix, projectionMatrix, stableFrames, fov ] = GetActiveCameraMatrix(frameInfo);
			ImVec2 sceneTextureSize = SlateSystem::GetRegister()->GetViewPort()->GetPanelSize();
			const VkExtent2D windowSize = m_Device->GetSwapChainSupport().surfaceSize;

			ubo.projection = projectionMatrix;
			
			ubo.nprojection = projectionMatrix;
			ubo.nprojection[1][1] *= -1.0f;
			
			ubo.view = glm::inverse(invViewMatrix);
			
			ubo.inView = invViewMatrix;
			
			ubo.sceneTextureSize = {
				sceneTextureSize.x,
				sceneTextureSize.y,
				1.0f / sceneTextureSize.x,
				1.0f / sceneTextureSize.y
			};
			
			ubo.windowSize = {
				static_cast<float>(windowSize.width),
				static_cast<float>(windowSize.height),
				1.0f / static_cast<float>(windowSize.width),
				1.0f / static_cast<float>(windowSize.height)
			};

			ubo.stableFrames = stableFrames;

			ubo.fov = fov;
		});

		builder.UpdateUniformBuffer<SpicesShader::Input>(0, 1, [&](auto& ubo) {
			auto [x, y] = SlateSystem::GetRegister()->GetViewPort()->GetMousePosInViewport();

			ubo.gameTime  = ts.gt();
			ubo.frameTime = ts.ft();
			ubo.mousePos  = glm::vec4(
				static_cast<float>(x),
				static_cast<float>(y),
				1.0f / static_cast<float>(x),
				1.0f / static_cast<float>(y)
			);
		});

		builder.EndRenderPass();
	}
}