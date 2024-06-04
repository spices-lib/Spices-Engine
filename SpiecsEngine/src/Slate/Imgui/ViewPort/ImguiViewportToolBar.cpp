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
	{}

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

		ImVec2 ItemSize = ImGuiH::GetLineItemSize() * 1.5f;
		ItemSize.x *= 3.0f;


		if (ImGui::Button(ICON_MD_SETTINGS_DISPLAY, ImGuiH::GetLineItemSize() * 1.5f))
		{
			ImGui::BeginMenu(ICON_MD_SETTINGS_DISPLAY);

			if (ImGui::BeginMenu("Navigation"))
			{
				if (ImGui::MenuItem("Navigation Speed")) {}
				if (ImGui::MenuItem("Navigation Speed Scalar")) {}
				if (ImGui::MenuItem("Lock Navigation Height")) {}
				if (ImGui::MenuItem("Gamepad Camera Control")) {}
				ImGui::Separator();
				if (ImGui::MenuItem("Object Centric Navigation")) {}
				if (ImGui::MenuItem("Double Click Sets Interest")) {}
				ImGui::Separator();
				if (ImGui::MenuItem("Inertia Mode")) {}
				if (ImGui::MenuItem("Camera Inertia")) {}
				ImGui::Separator();
				if (ImGui::MenuItem("Look Speed Horizontal")) {}
				if (ImGui::MenuItem("Look Speed Vertical")) {}
				if (ImGui::MenuItem("Look Speed Tumble Speed")) {}
				if (ImGui::MenuItem("Zoom Speed")) {}
				ImGui::Separator();
				if (ImGui::MenuItem("Animation clamp")) {}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Selection"))
			{
				if (ImGui::MenuItem("Selection Color")) {}
				if (ImGui::MenuItem("Selection Line Width")) {}
				if (ImGui::MenuItem("Bounding Box Color")) {}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Grid"))
			{
				if (ImGui::MenuItem("Grid Color")) {}
				if (ImGui::MenuItem("Grid Line Width")) {}
				if (ImGui::MenuItem("Grid Size")) {}
				if (ImGui::MenuItem("Grid Fade")) {}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Gizmos"))
			{
				if (ImGui::MenuItem("Gizmos Line Width")) {}
				if (ImGui::MenuItem("Gizmos Constant Scale Enabled")) {}
				if (ImGui::MenuItem("Gizmos Constant Scale")) {}
				if (ImGui::MenuItem("Gizmos Camera Scale")) {}
				if (ImGui::MenuItem("Gizmos Min FadeOut")) {}
				if (ImGui::MenuItem("Gizmos Max FadeOut")) {}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Viewport"))
			{
				if (ImGui::MenuItem("Render Resolution         Custom")) {}
				// todo here.
				if (ImGui::MenuItem("Custom Resolution")) {}
				if (ImGui::MenuItem("Render Scale")) {}
				if (ImGui::MenuItem("Fill Viewport")) {}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Viewport UI"))
			{
				if (ImGui::MenuItem("UI Background Opacity")) {}
				if (ImGui::MenuItem("UI Control Brightness")) {}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Manipulator Transform"))
			{
				if (ImGui::MenuItem("Transform Manipulator Scale")) {}
				if (ImGui::MenuItem("Enable Free Rotation")) {}
				if (ImGui::MenuItem("Free Rotation Type")) {}
				if (ImGui::MenuItem("Manipulator Intersection Thickness")) {}

				ImGui::EndMenu();
			}

			ImGui::Separator();
			ImGui::MenuItem("Reset To Defaults");
			ImGui::Separator();
			ImGui::MenuItem("Preferences");

			ImGui::EndMenu();
		}

		ImGui::SameLine();
		ImGui::Button(ICON_TEXT(ICON_MD_LIGHTBULB, Real - Time), ItemSize);
		ImGui::SameLine();
		ImGui::Button(ICON_MD_REMOVE_RED_EYE, ImGuiH::GetLineItemSize() * 1.5f);
		ImGui::SameLine();
		ImGui::Button(ICON_TEXT(ICON_MD_VIDEO_CAMERA_FRONT, Perspective), ItemSize);


		End();
	}
}