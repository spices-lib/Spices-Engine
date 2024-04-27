#pragma once
#include "Core/Core.h"
#include "ImguiUtils.h"

namespace Spiecs {

	class ImguiViewport : public ImguiSlate
	{
	public:
		ImguiViewport(const std::string& panelName);
		virtual ~ImguiViewport() {};

		virtual void OnUpdate(TimeStep& ts) override {};
		virtual void OnRender() override;
		virtual void OnEvent(Event& event) override;

	private:
		bool OnWindowResized(WindowOnResizedEvent& event);

	private:
		ImTextureID m_ViewportID;
	};
}