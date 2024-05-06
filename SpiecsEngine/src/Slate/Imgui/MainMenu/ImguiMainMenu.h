#pragma once
#include "Core/Core.h"
#include "../ImguiUtils.h"

namespace Spiecs {

	class ImguiWindow;
	class ImguiCreateEntity;
	class ImguiHelp;

	class ImguiMainMenu : public ImguiSlate
	{
	public:
		ImguiMainMenu(const std::string& panelName, FrameInfo& frameInfo);
		virtual ~ImguiMainMenu() {};

		virtual void OnUpdate(TimeStep& ts) override {};
		virtual void OnRender() override;
		virtual void OnEvent(Event& event) override {};

	private:
		std::shared_ptr<ImguiCreateEntity> m_Create;
		std::shared_ptr<ImguiWindow> m_Window;
		std::shared_ptr<ImguiHelp> m_Help;
	};

}