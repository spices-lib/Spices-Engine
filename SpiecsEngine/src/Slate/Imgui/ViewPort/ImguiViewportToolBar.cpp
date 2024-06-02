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
		LoadSlateIcon(m_IconID.optionIcon,    "slate/viewporttoolbar/ViewPortToolbar.Viewer.png");
		LoadSlateIcon(m_IconID.renderingIcon, "slate/viewporttoolbar/ViewPortToolbar.Rendering.png");
		LoadSlateIcon(m_IconID.viewerIcon,    "slate/viewporttoolbar/ViewPortToolbar.Options.png");
		LoadSlateIcon(m_IconID.cameraIcon,    "slate/viewporttoolbar/ViewPortToolbar.Camera.png");
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
		ImGui::SameLine(m_LineHeight * 1.5);
		ImGui::ImageButton(m_IconID.renderingIcon, ImVec2(m_LineHeight, m_LineHeight));
		ImGui::SameLine(m_LineHeight * 3.0);
		ImGui::ImageButton(m_IconID.viewerIcon, ImVec2(m_LineHeight, m_LineHeight));
		ImGui::SameLine(m_LineHeight * 4.5);
		ImGui::ImageButton(m_IconID.cameraIcon, ImVec2(m_LineHeight, m_LineHeight));

		End();
	}
}