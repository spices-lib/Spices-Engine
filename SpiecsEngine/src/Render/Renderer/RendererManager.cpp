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
	scl::linked_unordered_map<std::string, std::shared_ptr<Renderer>> RendererManager::m_Identities;

	RendererManager& RendererManager::Get()
	{
		if(!m_RendererManager)
		{
			m_RendererManager = std::make_unique<RendererManager>();
		}

		return *m_RendererManager;
	}
	
	void RendererManager::Run(TimeStep& ts, FrameInfo& frameInfo)
	{
		ZoneScoped;

		m_Identities.for_each([&](auto& k, auto& v) {
			v->Render(ts, frameInfo);
			return false;
		});
	}

	void RendererManager::OnWindowResizeOver()
	{
		ZoneScoped;

		m_Identities.for_each([](auto& k, auto& v) {
			v->OnWindowResizeOver();
			return false;
		});
	}

	void RendererManager::OnSlateResize()
	{
		ZoneScoped;

		m_Identities.for_each([](auto& k, auto& v) {
			v->OnSlateResize();
			return false;
		});
	}

	std::shared_ptr<Renderer> RendererManager::GetRenderer(const std::string name)
	{
		ZoneScoped;

		if (m_Identities.has_key(name))
		{
			return m_Identities.find_value(name);
		}

		std::stringstream ss;
		ss << "RendererManager::GetRenderer: Not such a renderer called: " << name;

		SPIECS_CORE_ERROR(ss.str());

		return nullptr;
	}
}