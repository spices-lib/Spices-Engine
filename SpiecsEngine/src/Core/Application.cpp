#include "pchheader.h"
#include "Application.h"
#include "Render/FrameInfo.h"
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
		FrameInfo::Get().m_World = nullptr;

		SystemManager::Get()
			.PopSystem("RenderSystem")
			.PopSystem("MeshSystem")
			.PopSystem("MaterialSystem")
			.PopSystem("UISystem");
	}

	void Application::Run()
	{
		// init our world
		FrameInfo::Get().m_World = std::make_shared<EditorWorld>();

		// temp TODO: Remove
		FrameInfo::Get().m_World->OnPreActivate();
		FrameInfo::Get().m_World->OnActivate();
		FrameInfo::Get().m_World->OnDeactivate();
	}
}