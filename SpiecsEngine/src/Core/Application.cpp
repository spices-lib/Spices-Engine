#include "pchheader.h"
#include "Application.h"
#include "Render/FrameInfo.h"
#include "Systems/SystemManager.h"
#include "Systems/RenderSystem.h"
#include "Systems/UISystem.h"
#include "Systems/NativeScriptSystem.h"
#include "Render/Vulkan/VulkanRenderBackend.h"


namespace Spiecs {

	Application::Application()
	{
		// Init All Systems
		SystemManager::Get()
			.PushSystem<NativeScriptSystem>()
			.PushSystem<RenderSystem>()
			.PushSystem<UISystem>();
	}

	Application::~Application()
	{
		FrameInfo::Get().m_World = nullptr;

		SystemManager::Get()
			.PopSystem("UISystem")
			.PopSystem("RenderSystem")
			.PopSystem("NativeScriptSystem");
	}

	void Application::Run()
	{
		// init our world
		FrameInfo::Get().m_World = std::make_shared<EditorWorld>();

		// temp TODO: Remove
		FrameInfo::Get().m_World->OnPreActivate();


		TimeStep ts;

		while (!glfwWindowShouldClose(VulkanRenderBackend::GetState().m_Windows))
		{
			glfwPollEvents();

			ts.Flush();
			FrameInfo::Get().m_World->OnActivate(ts);
		}

		VulkanRenderBackend::WaitIdle();

		FrameInfo::Get().m_World->OnDeactivate();
	}
}