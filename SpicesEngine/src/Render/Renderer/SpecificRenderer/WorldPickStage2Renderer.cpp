/**
* @file WorldPickRenderer.cpp.
* @brief The WorldPickRenderer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "WorldPickStage2Renderer.h"
#include "PreRenderer.h"
#include "Systems/SlateSystem.h"

namespace Spices {

	WorldPickStage2Renderer::WorldPickStage2Renderer(
		const std::string&                           rendererName           , 
		VulkanState&                                 vulkanState            ,
		const std::shared_ptr<VulkanDescriptorPool>& descriptorPool         ,
		const std::shared_ptr<VulkanDevice>&         device                 ,
		const std::shared_ptr<RendererResourcePool>& rendererResourcePool   ,
		const std::shared_ptr<VulkanCmdThreadPool>&  cmdThreadPool
	)
		: Renderer(rendererName, vulkanState, descriptorPool, device, rendererResourcePool, cmdThreadPool)
	{
		SPICES_PROFILE_ZONE;

		m_Square = std::make_unique<SquarePack>();
		m_Square->OnCreatePack();
	}

	void WorldPickStage2Renderer::CreateRendererPass()
	{
		SPICES_PROFILE_ZONE;

		RendererPassBuilder{ "WorldPickStage2", this }
		.AddSubPass("WorldPickStage2")
		.AddColorAttachment("SceneColor", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.finalLayout                          = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		})
		.EndSubPass()
		.Build();

		/**
		* @brief create renderresource's descriptorset.
		*/
		m_RendererResourcePool->AccessRowResource("SelectBuffer")->CreateDescriptorSet(0);
	}

	void WorldPickStage2Renderer::CreateDescriptorSet()
	{
		SPICES_PROFILE_ZONE;

		DescriptorSetBuilder{ "WorldPickStage2", this }
		.AddAttachmentTexture(2, 0, VK_SHADER_STAGE_FRAGMENT_BIT, {"SelectBuffer"})
		.Build();
	}

	void WorldPickStage2Renderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		RenderBehaveBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.BeginRenderPass();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "WorldPickStage2" }));

		builder.BindPipeline("WorldPickStage2Renderer.WorldPickStage2.Default");

		m_Square->OnBind(m_VulkanState.m_GraphicCommandBuffer[frameInfo.m_FrameIndex]);
		m_Square->OnDraw(m_VulkanState.m_GraphicCommandBuffer[frameInfo.m_FrameIndex]);

		builder.EndRenderPass();
	}
}