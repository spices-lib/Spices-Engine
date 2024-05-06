#pragma once
#include "Core/Core.h"
#include "Slate/Imgui/ImguiUtils.h"

namespace Spiecs {

	class ImguiGBufferVisualizer;

	class ImguiVisualizer : public ImguiSlate
	{
	public:
		ImguiVisualizer(const std::string& panelName, FrameInfo& frameInfo);
		virtual ~ImguiVisualizer() {};

		virtual void OnUpdate(TimeStep& ts) override {};
		virtual void OnRender() override;
		virtual void OnEvent(Event& event) override {};

	private:
		std::shared_ptr<ImguiGBufferVisualizer> m_GBufferVisualizer;
	};
}