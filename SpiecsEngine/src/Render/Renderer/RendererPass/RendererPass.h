#pragma once
#include "Core/Core.h"
#include "RendererSubPass.h"

namespace Spiecs
{
	class RendererPass
	{
	public:
		RendererPass(const std::string& passName) : m_PassName(passName) {};
		virtual ~RendererPass() {};



	private:

		std::string m_PassName;
		std::unordered_map<std::string, std::unique_ptr<RendererSubPass>> m_Passes;
	};
}