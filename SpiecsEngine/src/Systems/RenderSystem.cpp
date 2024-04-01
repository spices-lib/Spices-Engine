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
		// init FrameInfo 
		FrameInfo frameInfo;
		auto currentTime = std::chrono::high_resolution_clock::now();

		// init meshes 
		// TODO: Move to ECS
		std::shared_ptr<FilePack> pack = std::make_shared<FilePack>(GetState(), "111");
		pack->OnCreatePack();
		std::shared_ptr<Mesh> mesh = Mesh::Builder().AddPack(pack).Build();
		frameInfo.m_Meshes.push_back(mesh);


		while (!m_RenderFrontend->isWindowClosed())
		{
			glfwPollEvents();

			static auto beginTime = std::chrono::high_resolution_clock::now();
			auto nowTime = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(nowTime - currentTime).count();
			float gameTime = std::chrono::duration<float, std::chrono::seconds::period>(nowTime - beginTime).count();
			currentTime = nowTime;

			//std::cout << "FPS: " << 1000 / frameTime << std::endl;

			m_RenderFrontend->beginFrame(frameInfo);

			m_RenderFrontend->DrawTest(frameInfo);

			m_RenderFrontend->endFrame(frameInfo);

			frameInfo.m_FrameIndex = (frameInfo.m_FrameIndex + 1) % MaxFrameInFlight;
		}

		m_RenderFrontend->WaitIdle();
	}
}