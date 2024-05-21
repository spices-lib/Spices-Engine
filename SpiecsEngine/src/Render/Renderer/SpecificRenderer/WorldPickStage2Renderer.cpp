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
		m_Square = std::make_unique<SquarePack>();
		m_Square->OnCreatePack();
	}

	void WorldPickStage2Renderer::CreateRendererPass()
	{
		RendererPassBuilder{ "WorldPickStage2", this }
		.AddSubPass("WorldPickStage2")
		.AddColorAttachment("SceneColor", [](bool& isEnableBlend, VkAttachmentDescription& description) {
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
		DescriptorSetBuilder{ "WorldPickStage2", "WorldPickStage2", this }
		.AddAttachmentTexture(1, 0, VK_SHADER_STAGE_FRAGMENT_BIT, {"SelectBuffer"})
		.Build();
	}

	void WorldPickStage2Renderer::OnSlateResize()
	{
		/**
		* @brief Recreate RenderPass.
		*/
		CreateRendererPass();

		/**
		* @brief Free unused desctiptorSet and descriptorsetlayout.
		*/
		DescriptorSetManager::UnLoad(m_RendererName);

		/**
		* @brief Create descriptorSet again.
		*/
		CreateDescriptorSet();
	}

	void WorldPickStage2Renderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		RenderBehaverBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.BeginRenderPass("WorldPickStage2");

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName(m_RendererName));

		builder.BindPipeline("WorldPickStage2Renderer.Default");

		m_Square->OnBind(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex]);
		m_Square->OnDraw(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex]);

		builder.EndRenderPass();
	}
}