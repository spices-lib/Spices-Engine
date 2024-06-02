#include "Pchheader.h"
#include "ImguiViewportToolBar.h"

namespace Spiecs {

	ImguiViewportToolBar::ImguiViewportToolBar(
		const std::string& panelName , 
		FrameInfo&         frameInfo , 
		ImguiSlate*        owner
	)
		: ImguiSlate(panelName, frameInfo)
		, m_Owner(owner)
	{
		LoadSlateIcon(m_IconID.optionIcon, "slate/viewporttoolbar/ViewPortToolbar.Options.png");
	}

	void ImguiViewportToolBar::OnRender()
	{
		ImGuiWindowFlags window_flags =
			ImGuiWindowFlags_NoDecoration        |
			ImGuiWindowFlags_NoDocking           |
			ImGuiWindowFlags_AlwaysAutoResize    |
			ImGuiWindowFlags_NoSavedSettings     |
			ImGuiWindowFlags_NoFocusOnAppearing  |
			ImGuiWindowFlags_NoNav               |
			ImGuiWindowFlags_NoMove;

		ImGui::SetNextWindowPos(ImVec2(m_Owner->GetPanelPos().x + 5.0f, m_Owner->GetPanelPos().y + 5.0f));
		
		bool open = true;
		Begin(m_PanelName.c_str(), 0.35f, window_flags);

		ImGui::ImageButton(m_IconID.optionIcon, ImVec2(m_LineHeight, m_LineHeight));


		End();
	}
}