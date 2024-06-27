/**
* @file ShadowRenderer.cpp.
* @brief The ShadowRenderer Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "ShadowRenderer.h"
#include "PreRenderer.h"
#include "Systems/SlateSystem.h"

namespace Spiecs {

	namespace ShadowR
	{
		struct DirectionalLightMatrixs
		{
			std::array<glm::mat4, MAX_DIRECTIONALLIGHT_NUM> Matrixs;
		};
	}

	ShadowRenderer::ShadowRenderer(
		const std::string&                     rendererName         ,
		VulkanState&                           vulkanState          ,
		std::shared_ptr<VulkanDescriptorPool>  desctiptorPool       ,
		std::shared_ptr<VulkanDevice>          device               ,
		std::shared_ptr<RendererResourcePool>  rendererResourcePool
	)
		: Renderer(rendererName, vulkanState, desctiptorPool, device, rendererResourcePool)
	{}

	void ShadowRenderer::CreateRendererPass()
	{
		SPIECS_PROFILE_ZONE;

		RendererPassBuilder{ "Shadow", this }
		.AddSubPass("DirectionalLightShadow")
		.AddDepthAttachment("DirectionalLightShadow", TextureType::Texture2DArray, [](VkAttachmentDescription& description) {
			description.initialLayout                           = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp                                  = VK_ATTACHMENT_LOAD_OP_CLEAR;
		})
		.EndSubPass()
		.Build();
	}

	void ShadowRenderer::CreateDescriptorSet()
	{
		SPIECS_PROFILE_ZONE;

		DescriptorSetBuilder{ "DirectionalLightShadow", this }
		.AddPushConstant<PreR::PushConstant>()
		.AddStorageBuffer<ShadowR::DirectionalLightMatrixs>(1, 0, VK_SHADER_STAGE_GEOMETRY_BIT)
		.Build();
	}

	void ShadowRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPIECS_PROFILE_ZONE;

		RenderBehaverBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.BeginRenderPass();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "DirectionalLightShadow" }));

		builder.UpdateStorageBuffer<ShadowR::DirectionalLightMatrixs>(1, 0, [&](auto& ubo) {
			GetDirectionalLightMatrix(frameInfo, ubo.Matrixs);
		});

		builder.BindPipeline("ShadowRenderer.DirectionalLightShadow.Default");

		IterWorldComp<MeshComponent>(frameInfo, [&](int entityId, TransformComponent& transComp, MeshComponent& meshComp) {
			const glm::mat4& modelMatrix = transComp.GetModelMatrix();

			meshComp.GetMesh()->Draw(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex], [&](uint32_t meshpackId, auto material) {

				builder.UpdatePushConstant<PreR::PushConstant>([&](auto& push) {
					push.model = modelMatrix;
					push.entityID = entityId;
				});
			});

			return false;
		});

		builder.EndRenderPass();
	}
}