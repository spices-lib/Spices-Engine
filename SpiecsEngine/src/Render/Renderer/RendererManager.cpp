/**
* @file RendererManager.cpp.
* @brief The RendererManager Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "Renderer.h"
#include "RendererManager.h"

namespace Spiecs {

	std::unique_ptr<RendererManager> RendererManager::m_RendererManager;
	std::unordered_map<std::string, std::unique_ptr<Renderer>> RendererManager::m_Identities;

	RendererManager& RendererManager::Get()
	{
		return *m_RendererManager;
	}

	void RendererManager::Run(FrameInfo& frameInfo)
	{
		for (auto& pair : m_Identities)
		{
			pair.second->Render(frameInfo);
		}
	}

	void RendererManager::OnWindowResized()
	{
		for (auto& pair : m_Identities)
		{
			pair.second->OnWindowResized();
		}
	}
}