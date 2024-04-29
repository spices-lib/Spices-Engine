#pragma once
#include "Core/Core.h"
#include "ImguiUtils.h"

namespace Spiecs {

	class Console;

	class ImguiConsole : public ImguiSlate
	{
	public:
		ImguiConsole(const std::string& panelName, std::shared_ptr<Console> console);
		virtual ~ImguiConsole() {};

		virtual void OnUpdate(TimeStep& ts) override {};
		virtual void OnRender() override;
		virtual void OnEvent(Event& event) override {};

		static int TextEditCallbackStub(ImGuiInputTextCallbackData* data);

	private:

		std::shared_ptr<Console> m_Console;
		ImGuiTextFilter m_Filter;
	};
}