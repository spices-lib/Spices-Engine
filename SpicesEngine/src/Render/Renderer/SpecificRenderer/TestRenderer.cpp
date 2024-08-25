#include "Pchheader.h"
#include "TestRenderer.h"
#include "PreRenderer.h"
#include "Systems/SlateSystem.h"

namespace Spices {

	TestRenderer::TestRenderer(
		const std::string&                           rendererName   ,
		VulkanState&                                 vulkanState    ,
		const std::shared_ptr<VulkanDescriptorPool>& desctiptorPool ,
		const std::shared_ptr<VulkanDevice>&         device         ,
		const std::shared_ptr<RendererResourcePool>& rendererResourcePool,
		const std::shared_ptr<VulkanCmdThreadPool>&  cmdThreadPool
	)
		: Renderer(rendererName, vulkanState, desctiptorPool, device, rendererResourcePool, cmdThreadPool)
	{
		SPICES_PROFILE_ZONE;

		m_Plane = std::make_unique<PlanePack>();
		m_Plane->OnCreatePack();
	}

	void TestRenderer::CreateRendererPass()
	{
		SPICES_PROFILE_ZONE;

		RendererPassBuilder{ "TestPass", this }
		.AddSubPass("Test")
		.AddColorAttachment("TestCube", TextureType::Texture2DCube, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout                             = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp                                    = VK_ATTACHMENT_LOAD_OP_CLEAR;
			description.finalLayout                               = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		})
		.EndSubPass()
		.Build();
	}

	void TestRenderer::CreateDescriptorSet()
	{
		SPICES_PROFILE_ZONE;

		DescriptorSetBuilder{ "Test", this }
		.Build();
	}

	void TestRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		RenderBehaveBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.BeginRenderPass();

		builder.BindPipeline("TestRenderer.Test.Default");

		m_Plane->OnBind(m_VulkanState.m_GraphicCommandBuffer[frameInfo.m_FrameIndex]);
		m_Plane->OnDraw(m_VulkanState.m_GraphicCommandBuffer[frameInfo.m_FrameIndex]);

		builder.EndRenderPass();
	}
}