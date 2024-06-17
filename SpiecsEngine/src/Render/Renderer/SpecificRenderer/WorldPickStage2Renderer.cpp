/**
* @file WorldPickRenderer.cpp.
* @brief The WorldPickRenderer Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "WorldPickStage2Renderer.h"
#include "PreRenderer.h"
#include "Systems/SlateSystem.h"

namespace Spiecs {

	WorldPickStage2Renderer::WorldPickStage2Renderer(
		const std::string&                     rendererName           , 
		VulkanState&                           vulkanState            , 
		std::shared_ptr<VulkanDescriptorPool>  desctiptorPool         , 
		std::shared_ptr<VulkanDevice>          device                 , 
		std::shared_ptr<RendererResourcePool>  rendererResourcePool
	)
		: Renderer(rendererName, vulkanState, desctiptorPool, device, rendererResourcePool)
	{
		SPIECS_PROFILE_ZONE;

		m_Square = std::make_unique<SquarePack>();
		m_Square->OnCreatePack();
	}

	void WorldPickStage2Renderer::CreateRendererPass()
	{
		SPIECS_PROFILE_ZONE;

		RendererPassBuilder{ "WorldPickStage2", this }
		.AddSubPass("WorldPickStage2")
		.AddColorAttachment("SceneColor", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
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
		SPIECS_PROFILE_ZONE;

		DescriptorSetBuilder{ "WorldPickStage2", this }
		.AddAttachmentTexture(1, 0, VK_SHADER_STAGE_FRAGMENT_BIT, {"SelectBuffer"})
		.Build();
	}

	void WorldPickStage2Renderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPIECS_PROFILE_ZONE;

		RenderBehaverBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.BeginRenderPass();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "WorldPickStage2" }));

		builder.BindPipeline("WorldPickStage2Renderer.WorldPickStage2.Default");

		m_Square->OnBind(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex]);
		m_Square->OnDraw(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex]);

		builder.EndRenderPass();
	}
}