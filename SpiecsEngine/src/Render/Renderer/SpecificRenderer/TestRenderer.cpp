#include "Pchheader.h"
#include "TestRenderer.h"
#include "PreRenderer.h"
#include "Systems/SlateSystem.h"

namespace Spiecs {

	TestRenderer::TestRenderer(
		const std::string&                     rendererName   ,
		VulkanState&                           vulkanState    ,
		std::shared_ptr<VulkanDescriptorPool>  desctiptorPool ,
		std::shared_ptr<VulkanDevice>          device         ,
		std::shared_ptr<RendererResourcePool>  rendererResourcePool
	)
		: Renderer(rendererName, vulkanState, desctiptorPool, device, rendererResourcePool)
	{
		SPIECS_PROFILE_ZONE;

		m_Square = std::make_unique<SquarePack>();
		m_Square->OnCreatePack();
	}

	void TestRenderer::CreateRendererPass()
	{
		SPIECS_PROFILE_ZONE;

		RendererPassBuilder{ "TestPass", this }
		.AddSubPass("Test")
		.AddColorAttachment("TestCube", TextureType::Texture2DCube, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			description.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		})
		.EndSubPass()
		.Build();
	}

	void TestRenderer::CreateDescriptorSet()
	{
		SPIECS_PROFILE_ZONE;

		DescriptorSetBuilder{ "Test", this }
		.Build();
	}

	void TestRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPIECS_PROFILE_ZONE;

		RenderBehaverBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.BeginRenderPass();

		builder.BindPipeline("TestRenderer.Test.Default");

		m_Square->OnBind(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex]);
		m_Square->OnDraw(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex]);

		builder.EndRenderPass();
	}
}