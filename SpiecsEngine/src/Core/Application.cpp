#include "pchheader.h"
#include "Application.h"
#include "Systems/SystemManager.h"
#include "Systems/RenderSystem.h"
#include "Systems/MaterialSystem.h"
#include "Systems/MeshSystem.h"
#include "Systems/UISystem.h"

namespace Spiecs {

	Application::Application()
	{
		// Init All Systems
		SystemManager::Get()
			.PushSystem<RenderSystem>("RenderSystem")
			.PushSystem<MeshSystem>("MeshSystem")
			.PushSystem<MaterialSystem>("MaterialSystem")
			.PushSystem<UISystem>("UISystem");
	}

	Application::~Application()
	{
		SystemManager::Get()
			.PopSystem("RenderSystem")
			.PopSystem("MeshSystem")
			.PopSystem("MaterialSystem")
			.PopSystem("UISystem");
	}

	void Application::Run()
	{
		// init our world
		m_EditorWorld = std::make_shared<EditorWorld>();
		m_RuntimeWorld = std::make_shared<RuntimeWorld>();

		// temp TODO: Remove
		//m_EditorWorld->OnPreActivate();
		//m_EditorWorld->OnActivate();
		//m_EditorWorld->OnDeactivate();
		SystemManager::Run();
	}
}