#include "pchheader.h"
#include "RenderSystem.h"
#include "Render/Resources/Mesh.h"

namespace Spiecs {

	void RenderSystem::OnSystemInitialize()
	{
		m_RenderFrontend = std::make_unique<RenderFrontend>();

		SPIECS_LOG("RenderSystem: Init Succeed");
	}

	void RenderSystem::OnSystemShutDown()
	{

	}

	void RenderSystem::OnSystemUpdate()
	{
		auto currentTime = std::chrono::high_resolution_clock::now();

		while (!m_RenderFrontend->isWindowClosed())
		{
			glfwPollEvents();

			static auto beginTime = std::chrono::high_resolution_clock::now();
			auto nowTime = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(nowTime - currentTime).count();
			float gameTime = std::chrono::duration<float, std::chrono::seconds::period>(nowTime - beginTime).count();
			currentTime = nowTime;

			//std::cout << "FPS: " << 1000 / frameTime << std::endl;

			m_RenderFrontend->beginFrame(FrameInfo::Get());

			m_RenderFrontend->DrawTest(FrameInfo::Get());

			m_RenderFrontend->endFrame(FrameInfo::Get());

			FrameInfo::Get().m_FrameIndex = (FrameInfo::Get().m_FrameIndex + 1) % MaxFrameInFlight;
		}

		m_RenderFrontend->WaitIdle();
	}
}