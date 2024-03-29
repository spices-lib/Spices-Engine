#include "pchheader.h"
#include "RenderSystem.h"

#include "Render/Renderer/MeshRenderer.h"

namespace Spiecs {

	void RenderSystem::OnSystemInitialize()
	{
		m_RenderFrontend = std::make_unique<RenderFrontend>();

		m_Renderers

		SPIECS_LOG("RenderSystem: Init Succeed");
	}

	void RenderSystem::OnSystemShutDown()
	{

	}

	void RenderSystem::OnSystemUpdate()
	{
		FrameInfo frameInfo;
		auto currentTime = std::chrono::high_resolution_clock::now();

		while (!m_RenderFrontend->isWindowClosed())
		{
			glfwPollEvents();

			static auto beginTime = std::chrono::high_resolution_clock::now();
			auto nowTime = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(nowTime - currentTime).count();
			float gameTime = std::chrono::duration<float, std::chrono::seconds::period>(nowTime - beginTime).count();
			currentTime = nowTime;

			m_RenderFrontend->beginFrame(frameInfo);

			m_RenderFrontend->DrawTest(frameInfo);

			m_RenderFrontend->endFrame(frameInfo);

			frameInfo.m_FrameIndex = (frameInfo.m_FrameIndex + 1) % MaxFrameInFlight;
		}

		m_RenderFrontend->WaitIdle();
	}
}