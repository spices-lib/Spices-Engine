#pragma once
#include "Core/Core.h"
#include "Resources/Texture/Texture2D.h"

namespace Spiecs{

	struct VulkanState;

	struct RendererResourceCreateInfo
	{
		RendererResourceCreateInfo() {};
		virtual ~RendererResourceCreateInfo() {};

		VkAttachmentDescription description;
		uint32_t width;
		uint32_t height;
		bool isDepthResource = false;
	};

	class RendererResource
	{
	public:
		RendererResource(const RendererResourceCreateInfo& info);
		virtual ~RendererResource() {};

		void OnResized();

		std::shared_ptr<Texture2D> GetTexture() { return m_Texture; };

	private:

		bool isResizeable = true;
		std::shared_ptr<Texture2D> m_Texture;
	};
}