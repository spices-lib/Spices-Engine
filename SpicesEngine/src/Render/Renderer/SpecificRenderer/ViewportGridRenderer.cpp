/**
* @file ViewportGridRenderer.cpp.
* @brief The ViewportGridRenderer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ViewportGridRenderer.h"

namespace Spices {

	void ViewportGridRenderer::CreateRendererPass()
	{
		SPICES_PROFILE_ZONE;

		RendererPassBuilder{ "ViewportGrid", this }
		.AddSubPass("ViewportGrid")
		.AddColorAttachment("SceneColor", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			isEnableBlend                            = true;
			description.format                       = VK_FORMAT_R16G16B16A16_SFLOAT;
		})
		.AddDepthAttachment("Depth", TextureType::Texture2D, [](VkAttachmentDescription& description) {})
		.EndSubPass()
		.Build();
	}

	void ViewportGridRenderer::CreateDescriptorSet()
	{
		SPICES_PROFILE_ZONE;

		DescriptorSetBuilder{ "ViewportGrid", this }
		.AddPushConstant(sizeof(uint64_t))
		.Build();
	}

	void ViewportGridRenderer::CreatePipeline(
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

	void ViewportGridRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		RenderBehaveBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_ImageIndex };

		builder.BeginRenderPass();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "ViewportGrid" }));
		
		builder.BindPipeline("ViewportGridRenderer.ViewportGrid.Default");

		builder.UpdatePushConstant<uint64_t>([&](auto& push) {
			push = GetDefaultMaterial("ViewportGrid")->GetMaterialParamsAddress();
		});
		
		builder.DrawFullScreenTriangle();

		builder.EndRenderPass();
	}
}