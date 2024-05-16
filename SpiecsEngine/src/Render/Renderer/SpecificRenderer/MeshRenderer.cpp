/**
* @file MeshRenderer.cpp.
* @brief The MeshRenderer Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "MeshRenderer.h"
#include "Render/Vulkan/VulkanDescriptor.h"

namespace Spiecs {

	namespace MeshR {

		/**
		* @brief PointLight Render Data.
		*/
		struct PointLightUBO
		{
			/**
			* @brief Each data is copyed from PointLightComponent.
			*/
			std::array<PointLightComponent::PointLight, 10> pointLights;
		};
	}

	void MeshRenderer::CreateRenderPass()
	{
		/**
		* @brief Declear an empty VulkanRenderPass Object.
		*/
		m_RenderPass = std::make_unique<VulkanRenderPass>(m_VulkanState, m_Device, m_RendererResourcePool);

		/**
		* @brief Add Albedo Attachment.
		*/
		m_RenderPass->AddColorAttachment("Diffuse", [](bool& isEnableBlend, VkAttachmentDescription& description) {
		});

		/**
		* @brief Add Normal Attachment.
		*/
		m_RenderPass->AddColorAttachment("Normal", [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		});

		/**
		* @brief Add Specular Attachment.
		*/
		m_RenderPass->AddColorAttachment("Specular", [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		});

		/**
		* @brief Add ID Attachment.
		*/
		m_RenderPass->AddColorAttachment("ID", [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.format = VK_FORMAT_R32_SFLOAT;
		});

		/**
		* @brief Add Depth Attachment.
		*/
		m_RenderPass->AddDepthAttachment([](VkAttachmentDescription& description) {
		});

		/**
		* @brief Create VkRenderPass, Resource, FrameBuffer.
		*/
		m_RenderPass->Build();
	}

	void MeshRenderer::CreateDescriptorSet()
	{
		DescriptorSetBuilder{ this }
		.AddPushConstant<PreR::PushConstant>()
		.AddTexture<Texture2D>(1, 0, 3, VK_SHADER_STAGE_FRAGMENT_BIT)
		.AddBuffer<DirectionalLightComponent::DirectionalLight>(2, 0, VK_SHADER_STAGE_FRAGMENT_BIT)
		.AddBuffer<MeshR::PointLightUBO>(2, 1, VK_SHADER_STAGE_FRAGMENT_BIT)
		.Build();
	}

	void MeshRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		RenderBehaverBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.BeginRenderPass();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName(m_RendererName));

		builder.UpdateBuffer<DirectionalLightComponent::DirectionalLight>(2, 0, [&](auto& ubo) {
			ubo = GetDirectionalLight(frameInfo);
		});

		builder.UpdateBuffer<MeshR::PointLightUBO>(2, 1, [&](auto& ubo) {
			ubo.pointLights = GetPointLight(frameInfo);
		});

		IterWorldComp<MeshComponent>(frameInfo, [&](int entityId, TransformComponent& transComp, MeshComponent& meshComp) {
			const glm::mat4& modelMatrix = transComp.GetModelMatrix();

			meshComp.GetMesh()->Draw(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex], [&](uint32_t meshpackId, auto material) {
				builder.BindPipeline(m_Pipelines[material->GetName()]);

				builder.UpdatePushConstant<MeshR::PushConstant>([&](auto& push) {
					push.model = modelMatrix;
					push.entityID = entityId;
				});

				builder.BindDescriptorSet(material->GetMaterialDescriptorSet());
			});

			return false;
		});

		builder.EndRenderPass();
	}
}