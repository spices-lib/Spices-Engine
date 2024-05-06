#pragma once
#include "Core/Core.h"
#include "Slate/Imgui/ImguiUtils.h"

namespace Spiecs {

	class ImguiVisualizer;

	class ImguiWindow : public ImguiSlate
	{
	public:
		ImguiWindow(const std::string& panelName, FrameInfo& frameInfo);
		virtual ~ImguiWindow() {};

		virtual void OnUpdate(TimeStep& ts) override {};
		virtual void OnRender() override;
		virtual void OnEvent(Event& event) override {};

	private:
		std::shared_ptr<ImguiVisualizer> m_Visualizer;
	};
}