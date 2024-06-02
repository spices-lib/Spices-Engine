/**
* @file Application.cpp.
* @brief The Application Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "Application.h"
#include "Render/FrameInfo.h"
#include "Render/Vulkan/VulkanRenderBackend.h"

// System Header.
#include "Systems/SystemManager.h"
#include "Systems/RenderSystem.h"
#include "Systems/NativeScriptSystem.h"
#include "Systems/ResourceSystem.h"
#include "Systems/SlateSystem.h"

namespace Spiecs {

	Application::Application()
	{
		SPIECS_PROFILE_ZONE;
		
		/**
		* @brief Init Log Class.
		*/
		Log::Init();

		/**
		* @brief Init all Systems.
		* @attention SystemManager Class did not Constructor, it returns Null.
		* @todo Fixing it.
		*/
		SystemManager().Get()
		.PushSystem<NativeScriptSystem>()
		.PushSystem<RenderSystem>()
		.PushSystem<ResourceSystem>()
		.PushSystem<SlateSystem>();
	}

	Application::~Application()
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Destroy our Specific World.
		*/
		FrameInfo::Get().m_World = nullptr;

		/**
		* @brief Destroy all Systems.
		*/
		SystemManager::Get()
			.PopSystem("SlateSystem")
			.PopSystem("ResourceSystem")
			.PopSystem("RenderSystem")
			.PopSystem("NativeScriptSystem");


		/**
		* @brief Shutdown Log Class.
		*/
		Log::ShutDown();
	}

	void Application::Run()
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Specify the current World, which created from Game.
		* @todo Mult World Support.
		*/
		FrameInfo::Get().m_World = CreateWorld();

		/**
		* @brief World OnPreActivate.
		* @todo Remove.
		*/
		FrameInfo::Get().m_World->OnPreActivate();

		/**
		* @brief Init Golbal TimeStep Class.
		*/
		TimeStep ts;

		/**
		* @brief Golbal While Loop.
		* @todo Multithreading.
		*/
		while (!glfwWindowShouldClose(VulkanRenderBackend::GetState().m_Windows))
		{
			SPIECS_PROFILE_ZONE;

			/**
			* @brief Wait for glfw events.
			*/
			glfwPollEvents();

			/**
			* @brief Update TimeStep.
			*/
			ts.Flush();
			
			/**
			* @brief Activete Our Specific World.
			*/
			FrameInfo::Get().m_World->OnActivate(ts);

			SPIECS_PROFILE_FRAME;
		}

		/**
		* @brief Vulkan Device Idle.
		*/
		VulkanRenderBackend::WaitIdle();

		/**
		* @brief Deactivate Our Specific World.
		*/
		FrameInfo::Get().m_World->OnDeactivate();
	}
}