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
	scl::linked_unordered_map<std::string, std::unique_ptr<Renderer>> RendererManager::m_Identities;

	RendererManager& RendererManager::Get()
	{
		return *m_RendererManager;
	}

	void RendererManager::Run(FrameInfo& frameInfo)
	{
		for (int i = 0; i < m_IterList.size(); i++)
		{
			m_Identities[m_IterList[i]]->Render(frameInfo);
		}
	}

	void RendererManager::OnWindowResizeOver()
	{
		for (auto& pair : m_Identities)
		{
			pair.second->OnWindowResizeOver();
		}
	}

	void RendererManager::OnSlateResize()
	{
		for (auto& pair : m_Identities)
		{
			pair.second->OnSlateResize();
		}
	}
}