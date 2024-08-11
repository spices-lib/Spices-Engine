/**
* @file SceneComposeRenderer.cpp.
* @brief The SceneComposeRenderer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "SceneComposeRenderer.h"
#include "Systems/SlateSystem.h"
#include "RayTracingRenderer.h"

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
		const std::shared_ptr<RendererResourcePool>& rendererResourcePool ,
		const std::shared_ptr<VulkanCmdThreadPool>&  cmdThreadPool
	)
		: Renderer(rendererName, vulkanState, descriptorPool, device, rendererResourcePool, cmdThreadPool)
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
		.AddInputAttachment("Albedo", TextureType::Texture2D, [](VkAttachmentDescription& description) {
			description.finalLayout                   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		})
		.AddInputAttachment("Normal", TextureType::Texture2D, [](VkAttachmentDescription& description) {
			description.finalLayout                   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		})
		.AddInputAttachment("Roughness", TextureType::Texture2D, [](VkAttachmentDescription& description) {
			description.finalLayout                   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		})
		.AddInputAttachment("Metallic", TextureType::Texture2D, [](VkAttachmentDescription& description) {
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
		.AddInput(2, 0, VK_SHADER_STAGE_FRAGMENT_BIT, { "Albedo", "Normal", "Roughness", "Metallic", "Position", "Depth" })
		.AddStorageBuffer<RayTracingR::DirectionalLightBuffer>(3, 0, VK_SHADER_STAGE_FRAGMENT_BIT)                        /* @brief World Directional Light Buffer. */
		.AddStorageBuffer<RayTracingR::PointLightBuffer>(3, 1, VK_SHADER_STAGE_FRAGMENT_BIT)                              /* @brief World PointLight Buffer.        */
		.Build();
	}

	void SceneComposeRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		if(frameInfo.m_RendererType != RendererType::Rasterization) return;
		
		RenderBehaveBuilder builder{ this, frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.BeginRenderPass();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));
		
		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "SceneCompose" }));

		builder.UpdateStorageBuffer<RayTracingR::DirectionalLightBuffer>(3, 0, [&](auto& ssbo) {
			GetDirectionalLight(frameInfo, ssbo.lights);
		});

		builder.UpdateStorageBuffer<RayTracingR::PointLightBuffer>(3, 1, [&](auto& ssbo) {
			GetPointLight(frameInfo, ssbo.lights);
		});

		builder.BindPipeline("SceneComposeRenderer.SceneCompose.Default");

		m_Square->OnBind(m_VulkanState.m_GraphicCommandBuffer[frameInfo.m_FrameIndex]);
		m_Square->OnDraw(m_VulkanState.m_GraphicCommandBuffer[frameInfo.m_FrameIndex]);


		builder.EndRenderPass();
	}
}