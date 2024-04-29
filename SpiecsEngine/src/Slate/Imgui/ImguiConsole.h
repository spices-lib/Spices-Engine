#pragma once
#include "Core/Core.h"
#include "ImguiUtils.h"

namespace Spiecs {

	class Console;

	struct ConsoleIconID
	{
		// Left Icon
		ImTextureID clearConsoleIcon;
		ImTextureID openLogFileIcon;
		ImTextureID openLogFolderIcon;
		ImTextureID enableCommandFieldIcon;

		// Right Icon
		ImTextureID select;
		ImTextureID verbose;
		ImTextureID info;
		ImTextureID warning;
		ImTextureID error;
	};

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
		void LoadConsoleIcon(ImTextureID& id, const std::string& iconFile);

	private:

		std::shared_ptr<Console> m_Console;
		ImGuiTextFilter m_Filter;

		ConsoleIconID m_ConsoleIconID;
		int m_Level = 0;
	};
}