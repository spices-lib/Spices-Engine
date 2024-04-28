#include "Pchheader.h"
#include "ImguiMainMenu.h"

namespace Spiecs {

	void ImguiMainMenu::OnRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		Begin();
		End();
		ImGui::PopStyleVar();
	}
}