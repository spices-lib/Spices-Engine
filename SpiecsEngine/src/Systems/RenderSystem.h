#pragma once
#include "Core/Core.h"
#include "SystemManager.h"
#include "Render/FrameInfo.h"
#include "Render/RenderFrontend.h"
#include "Render/Renderer/Renderer.h"
#include <unordered_map>
#include <memory>

namespace Spiecs {

	class RenderSystem : public System
	{
	public:
		RenderSystem(const std::string& systemName) : System(systemName) {};
		virtual ~RenderSystem() override {};

		RenderSystem(const RenderSystem&) = delete;
		RenderSystem& operator=(const RenderSystem&) = delete;

		virtual void OnSystemInitialize() override;
		virtual void OnSystemShutDown() override;
		virtual void OnSystemUpdate() override;

	private:
		std::unique_ptr<RenderFrontend> m_RenderFrontend;

		std::unordered_map<std::string, std::unique_ptr<Renderer>> m_Renderers;
	};
}