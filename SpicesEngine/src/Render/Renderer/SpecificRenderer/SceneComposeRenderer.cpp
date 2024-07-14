/**
* @file SceneComposeRenderer.cpp.
* @brief The SceneComposeRenderer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "SceneComposeRenderer.h"
#include "Systems/SlateSystem.h"

namespace Spices {

	namespace SceneCR
	{
		struct LightsBuffer
		{
			SpicesShader::DirectionalLight directionalLight;
			std::array<SpicesShader::PointLight, 1000> pointLights;
		};
	}

	SceneComposeRenderer::SceneComposeRenderer(
		const std::string&                           rendererName         , 
		VulkanState&                                 vulkanState          ,
		const std::shared_ptr<VulkanDescriptorPool>& descriptorPool       ,
		const std::shared_ptr<VulkanDevice>&         device               ,
		const std::shared_ptr<RendererResourcePool>& rendererResourcePool
	)
		: Renderer(rendererName, vulkanState, descriptorPool, device, rendererResourcePool)
	{
		SPICES_PROFILE_ZONE;

		m_Square = std::make_unique<SquarePack>();
		m_Square->OnCreatePack();
	}

	void SceneComposeRenderer::CreateRendererPass()
	{
		SPICES_PROFILE_ZONE;

		RendererPassBuilder{ "SceneCompose", this }
		.AddSubPass("SceneCompose")
		.AddColorAttachment("SceneColor", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout                 = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp                        = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		})
		.AddInputAttachment("Diffuse", TextureType::Texture2D, [](VkAttachmentDescription& description) {
			description.finalLayout                   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		})
		.AddInputAttachment("Normal", TextureType::Texture2D, [](VkAttachmentDescription& description) {
			description.finalLayout                   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		})
		.AddInputAttachment("Specular", TextureType::Texture2D, [](VkAttachmentDescription& description) {
			description.finalLayout                   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		})
		.AddInputAttachment("Position", TextureType::Texture2D, [](VkAttachmentDescription& description) {
			description.finalLayout                   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			description.format                        = VK_FORMAT_R32G32B32A32_SFLOAT;
		})
		.AddInputAttachment("Depth", TextureType::Texture2D, [&](VkAttachmentDescription& description) {
			description.format                        = VulkanSwapChain::FindDepthFormat(m_VulkanState.m_PhysicalDevice);
			description.initialLayout                 = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			description.finalLayout                   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		})
		.EndSubPass()
		.Build();
	}

	void SceneComposeRenderer::CreateDescriptorSet()
	{
		SPICES_PROFILE_ZONE;

		DescriptorSetBuilder{ "SceneCompose", this }
		.AddInput(1, 0, VK_SHADER_STAGE_FRAGMENT_BIT, { "Diffuse", "Normal", "Specular", "Position", "Depth" })
		.Build();
	}

	void SceneComposeRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		RenderBehaveBuilder builder{ this, frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.BeginRenderPass();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "SceneCompose" }));

		/*builder.UpdateStorageBuffer<SceneCR::LightsBuffer>(1, 1, [&](auto& ubo) {
			GetDirectionalLight(frameInfo, ubo.directionalLight);
			GetPointLight(frameInfo, ubo.pointLights);
		});*/

		builder.BindPipeline("SceneComposeRenderer.SceneCompose.Default");
		
		m_Square->OnBind(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex]);
		m_Square->OnDraw(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex]);


		builder.EndRenderPass();
	}
}