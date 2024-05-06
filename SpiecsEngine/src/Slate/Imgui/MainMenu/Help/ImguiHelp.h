#pragma once
#include "Core/Core.h"
#include "Slate/Imgui/ImguiUtils.h"

namespace Spiecs {

	class ImguiAbout;

	class ImguiHelp : public ImguiSlate
	{
	public:
		ImguiHelp(const std::string& panelName, FrameInfo& frameInfo);
		virtual ~ImguiHelp() {};

		virtual void OnUpdate(TimeStep& ts) override {};
		virtual void OnRender() override;
		virtual void OnEvent(Event& event) override {};

	private:
		std::shared_ptr<ImguiAbout> m_About;
	};
}