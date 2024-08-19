/**
* @file ShadowRenderer.cpp.
* @brief The ShadowRenderer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ShadowRenderer.h"
#include "Systems/SlateSystem.h"

namespace Spices {

	namespace ShadowR
	{
		struct DirectionalLightMatrixs
		{
			std::array<glm::mat4, MAX_DIRECTIONALLIGHT_NUM> Matrixs;
		};
	}

	ShadowRenderer::ShadowRenderer(
		const std::string&                           rendererName         ,
		VulkanState&                                 vulkanState          ,
		const std::shared_ptr<VulkanDescriptorPool>& descriptorPool       ,
		const std::shared_ptr<VulkanDevice>&         device               ,
		const std::shared_ptr<RendererResourcePool>& rendererResourcePool ,
		const std::shared_ptr<VulkanCmdThreadPool>&  cmdThreadPool
	)
		: Renderer(rendererName, vulkanState, descriptorPool, device, rendererResourcePool, cmdThreadPool)
	{}

	void ShadowRenderer::CreateRendererPass()
	{
		SPICES_PROFILE_ZONE;

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
		SPICES_PROFILE_ZONE;

		DescriptorSetBuilder{ "DirectionalLightShadow", this }
		.AddPushConstant(sizeof(uint64_t))
		.AddStorageBuffer(2, 0, sizeof(ShadowR::DirectionalLightMatrixs), VK_SHADER_STAGE_GEOMETRY_BIT)
		.Build();
	}

	void ShadowRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		if(frameInfo.m_RendererType != RendererType::Rasterization) return;
		
		RenderBehaveBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.BeginRenderPass();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "DirectionalLightShadow" }));

		builder.UpdateStorageBuffer<ShadowR::DirectionalLightMatrixs>(2, 0, [&](auto& ssbo) {
			GetDirectionalLightMatrix(frameInfo, ssbo.Matrixs);
		});

		builder.BindPipeline("ShadowRenderer.DirectionalLightShadow.Default");

		IterWorldCompWithBreak<MeshComponent>(frameInfo, [&](int entityId, TransformComponent& transComp, MeshComponent& meshComp) {

			meshComp.GetMesh()->Draw(m_VulkanState.m_GraphicCommandBuffer[frameInfo.m_FrameIndex], [&](uint32_t meshpackId, auto meshPack) {

				builder.UpdatePushConstant<uint64_t>([&](auto& push) {
					push = meshPack->GetMeshDesc().GetBufferAddress();
				});
			});

			return false;
		});

		builder.EndRenderPass();
	}
}