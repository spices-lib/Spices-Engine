/**
* @file BasePassRenderer.cpp.
* @brief The BasePassRenderer Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "BasePassRenderer.h"
#include "PreRenderer.h"

namespace Spiecs {

	void BasePassRenderer::CreateRendererPass()
	{
		ZoneScoped;

		RendererPassBuilder{ "BassPass", this }
		.AddSubPass("SkyBox")
		.AddColorAttachment("Diffuse", [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		})
		.AddColorAttachment("Position", [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			description.format = VK_FORMAT_R32G32B32A32_SFLOAT;
		})
		.AddColorAttachment("ID", [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			description.format = VK_FORMAT_R32_SFLOAT;
		})
		.AddDepthAttachment([](VkAttachmentDescription& description) {
			description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		})
		.EndSubPass()
		.AddSubPass("Mesh")
		.AddColorAttachment("Diffuse", [](bool& isEnableBlend, VkAttachmentDescription& description) {})
		.AddColorAttachment("Normal", [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		})
		.AddColorAttachment("Specular", [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		})
		.AddColorAttachment("Position", [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.format = VK_FORMAT_R32G32B32A32_SFLOAT;
		})
		.AddColorAttachment("ID", [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.format = VK_FORMAT_R32_SFLOAT;
		})
		.AddDepthAttachment([](VkAttachmentDescription& description) {})
		.EndSubPass()
		.Build();
	}

	void BasePassRenderer::CreateDescriptorSet()
	{
		ZoneScoped;

		DescriptorSetBuilder{ "SkyBox", this }
		.AddPushConstant<PreR::PushConstant>()
		.Build();

		DescriptorSetBuilder{ "Mesh", this }
		.AddPushConstant<PreR::PushConstant>()
		.Build();
	}

	std::shared_ptr<VulkanPipeline> BasePassRenderer::CreatePipeline(
		std::shared_ptr<Material> material,
		VkPipelineLayout& layout,
		std::shared_ptr<RendererSubPass> subPass
	)
	{
		ZoneScoped;

		PipelineConfigInfo pipelineConfig{};
		VulkanPipeline::DefaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = m_Pass->Get();
		pipelineConfig.subpass = subPass->GetIndex();
		pipelineConfig.pipelineLayout = layout;
		pipelineConfig.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
		pipelineConfig.colorBlendInfo.attachmentCount = (uint32_t)subPass->GetColorBlend().size();
		pipelineConfig.colorBlendInfo.pAttachments = subPass->GetColorBlend().data();
		return std::make_shared<VulkanPipeline>(
			m_VulkanState,
			GetSahderPath(material->GetShaderPath("vertShader"), "vert"),
			GetSahderPath(material->GetShaderPath("fragShader"), "frag"),
			pipelineConfig
		);
	}

	void BasePassRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		ZoneScoped;

		RenderBehaverBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.BeginRenderPass();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "SkyBox" }));

		IterWorldComp<SkyBoxComponent>(frameInfo, [&](int entityId, TransformComponent& transComp, SkyBoxComponent& skyboxComp) {
			const glm::mat4& modelMatrix = transComp.GetModelMatrix();

			skyboxComp.GetMesh()->Draw(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex], [&](uint32_t meshpackId, auto material) {
				builder.BindPipeline(material->GetName());

				builder.UpdatePushConstant<PreR::PushConstant>([&](auto& push) {
					push.model = modelMatrix;
					push.entityID = entityId;
				});

				builder.BindDescriptorSet(material->GetMaterialDescriptorSet(), material->GetName());
			});

			return false;
		});

		builder.BeginNextSubPass("Mesh");

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "Mesh" }));

		IterWorldComp<MeshComponent>(frameInfo, [&](int entityId, TransformComponent& transComp, MeshComponent& meshComp) {
			const glm::mat4& modelMatrix = transComp.GetModelMatrix();

			meshComp.GetMesh()->Draw(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex], [&](uint32_t meshpackId, auto material) {

				builder.BindPipeline(material->GetName());

				builder.UpdatePushConstant<PreR::PushConstant>([&](auto& push) {
					push.model = modelMatrix;
					push.entityID = entityId;
				});

				builder.BindDescriptorSet(material->GetMaterialDescriptorSet(), material->GetName());
			});

			return false;
		});

		builder.EndRenderPass();
	}
}