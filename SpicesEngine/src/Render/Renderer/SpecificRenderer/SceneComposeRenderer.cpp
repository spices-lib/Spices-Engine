/**
* @file SceneComposeRenderer.cpp.
* @brief The SceneComposeRenderer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "SceneComposeRenderer.h"
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
		const std::shared_ptr<RendererResourcePool>& rendererResourcePool
	)
		: Renderer(rendererName, vulkanState, descriptorPool, device, rendererResourcePool)
	{}

	void SceneComposeRenderer::CreateRendererPass()
	{
		SPICES_PROFILE_ZONE;

		RendererPassBuilder{ "SceneCompose", this }
		.AddSubPass("SceneCompose")
		.AddColorAttachment("SceneColor", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout                 = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp                        = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			description.format                        = VK_FORMAT_R16G16B16A16_SFLOAT;
		})
		.AddInputAttachment("Albedo", TextureType::Texture2D, [](VkAttachmentDescription& description) {
			description.finalLayout                   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			description.format                        = VK_FORMAT_R16G16B16A16_SFLOAT;
		})
		.AddInputAttachment("Normal", TextureType::Texture2D, [](VkAttachmentDescription& description) {
			description.finalLayout                   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			description.format                        = VK_FORMAT_R16G16B16A16_SFLOAT;
		})
		.AddInputAttachment("Roughness", TextureType::Texture2D, [](VkAttachmentDescription& description) {
			description.finalLayout                   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			description.format                        = VK_FORMAT_R16G16B16A16_SFLOAT;
		})
		.AddInputAttachment("Metallic", TextureType::Texture2D, [](VkAttachmentDescription& description) {
			description.finalLayout                   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			description.format                        = VK_FORMAT_R16G16B16A16_SFLOAT;
		})
		.AddInputAttachment("Position", TextureType::Texture2D, [](VkAttachmentDescription& description) {
			description.finalLayout                   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			description.format                        = VK_FORMAT_R32G32B32A32_SFLOAT;
		})
		.EndSubPass()
		.Build();
	}

	void SceneComposeRenderer::CreateDescriptorSet()
	{
		SPICES_PROFILE_ZONE;

		DescriptorSetBuilder{ "SceneCompose", this }
		.AddInput(2, 0, VK_SHADER_STAGE_FRAGMENT_BIT, { "Albedo", "Normal", "Roughness", "Metallic", "Position" })
		.AddStorageBuffer(3, 0, sizeof(RayTracingR::DirectionalLightBuffer), VK_SHADER_STAGE_FRAGMENT_BIT)                        /* @brief World Directional Light Buffer. */
		.AddStorageBuffer(3, 1, sizeof(RayTracingR::PointLightBuffer), VK_SHADER_STAGE_FRAGMENT_BIT)                              /* @brief World PointLight Buffer.        */
		.AddAccelerationStructure(4, 0, VK_SHADER_STAGE_FRAGMENT_BIT)                                                             /* @brief Acceleration Structure.         */
		.Build(RayTracingRenderer::GetAccelerationStructure());
	}

	void SceneComposeRenderer::CreatePipeline(
		std::shared_ptr<Material>        material ,
		VkPipelineLayout&                layout   ,
		std::shared_ptr<RendererSubPass> subPass
	)
	{
		SPICES_PROFILE_ZONE;

		PipelineBuilder{ subPass, material, this }
		.SetDefault()
		.NullBindingDescriptions()
		.NullAttributeDescriptions()
		.SetRenderPass()
		.SetSubPassIndex()
		.SetPipelineLayout(layout)
		.SetCullMode(VK_CULL_MODE_NONE)
		.SetColorAttachments()
		.Build();
	}

	void SceneComposeRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		if(frameInfo.m_RendererType != RendererType::Rasterization) return;

		RenderBehaveBuilder builder{ this, frameInfo.m_FrameIndex, frameInfo.m_ImageIndex };

		builder.BeginRenderPass();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));
		
		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "SceneCompose" }));

		builder.UpdateStorageBuffer<RayTracingR::DirectionalLightBuffer>(3, 0, [&](auto& ssbo) {
			GetDirectionalLight(frameInfo, ssbo.lights);
		});

		builder.UpdateStorageBuffer<RayTracingR::PointLightBuffer>(3, 1, [&](auto& ssbo) {
			GetPointLight(frameInfo, ssbo.lights);
		});

		builder.UpdateAccelerationStructure(4, 0, RayTracingRenderer::GetAccelerationStructure());
		
		builder.BindPipeline("SceneComposeRenderer.SceneCompose.Default");

		builder.DrawFullScreenTriangle();

		builder.EndRenderPass();
	}
}