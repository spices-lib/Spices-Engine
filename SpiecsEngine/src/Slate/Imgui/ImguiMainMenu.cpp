#include "Pchheader.h"
#include "ImguiMainMenu.h"

namespace Spiecs {

	void ImguiMainMenu::OnRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin(m_PanelName.c_str());
		ImGui::End();
		ImGui::PopStyleVar();
	}
}