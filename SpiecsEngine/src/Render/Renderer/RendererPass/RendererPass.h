#pragma once
#include "Core/Core.h"
#include "RendererSubPass.h"

#include <list>

namespace Spiecs
{
	class RendererPass
	{
	public:
		RendererPass(const std::string& passName) : m_PassName(passName) {};
		virtual ~RendererPass() {};


	private:

		std::string m_PassName;
		std::list<std::unique_ptr<RendererSubPass>> m_Passes;
		
		std::unique_ptr<VulkanRenderPass> m_RenderPass;
	};
}