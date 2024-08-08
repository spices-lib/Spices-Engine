#pragma once
#include "Core/Core.h"
#include "Render/Renderer/Renderer.h"

namespace Spices {

	class TestRenderer : public Renderer
	{
	public:

		TestRenderer(
			const std::string&                           rendererName   ,
			VulkanState&                                 vulkanState    ,
			const std::shared_ptr<VulkanDescriptorPool>& desctiptorPool ,
			const std::shared_ptr<VulkanDevice>&         device         ,
			const std::shared_ptr<RendererResourcePool>& rendererResourcePool,
			const std::shared_ptr<VulkanCmdThreadPool>&  cmdThreadPool
		);

		virtual ~TestRenderer() {};
		virtual void Render(TimeStep& ts, FrameInfo& frameInfo) override;

	private:

		virtual void CreateRendererPass() override;
		virtual void CreateDescriptorSet() override;

	private:

		std::unique_ptr<SquarePack> m_Square;
	};

}