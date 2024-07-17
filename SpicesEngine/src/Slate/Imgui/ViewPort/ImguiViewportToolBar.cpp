#include "Pchheader.h"
#include "ImguiViewportToolBar.h"

#include <imgui_internal.h>

namespace Spices {

	ImguiViewportToolBar::ImguiViewportToolBar(
		const std::string& panelName,
		FrameInfo& frameInfo,
		ImguiSlate* owner
	)
		: ImguiSlate(panelName, frameInfo)
		, m_Owner(owner)
	{}

	void ImguiViewportToolBar::OnRender()
	{
		SPICES_PROFILE_ZONE;

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.149f, 0.16f, 0.164f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.156f, 0.302f, 0.353f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.156f, 0.302f, 0.353f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.149f, 0.16f, 0.168f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);
		ImGui::PushStyleColor(ImGuiCol_PopupBg, ImGui::GetStyle().Colors[ImGuiCol_WindowBg]);

		ImGuiWindowFlags window_flags =
			ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoFocusOnAppearing |
			ImGuiWindowFlags_NoNav |
			ImGuiWindowFlags_NoMove;

		ImVec2 StartPos = ImVec2(m_Owner->GetPanelPos().x + 5.0f, m_Owner->GetPanelPos().y + 5.0f);
		ImGui::SetNextWindowPos(StartPos);

		bool open = true;
		Begin("Left ToolBar", 0.0f, window_flags);

		ImVec2 ItemSize = ImGuiH::GetLineItemSize() * 1.5f;
		ItemSize.x *= 4.0f;
		float offest = ImGui::GetStyle().WindowPadding.y;

		if (ImGui::Button(ICON_MD_TUNE, ImGuiH::GetLineItemSize() * 1.5f))
		{
			m_OptionMenuOn = !m_OptionMenuOn;
			m_RenderMenuOn = false;
			m_ViewMenuOn = false;
			m_CameraMenuOn = false;
			m_LightMenuOn = false;
			m_WayPointOn = false;
		}
		ImGui::SameLine();


		static std::string renderingMode[5] = {
			"Rasterization",
			"RTX-Real-Time",
			"RTX-Interactive(Path Tracing)",
			"RTX-Accurate(lray)",
			"Pixar-Storm"
		};

		static int rendererSelected = 0;

		{
			SPICES_PROFILE_ZONEN("ImguiViewportToolBar::RendererMode");

			if (ImGui::Button(ICON_TEXT(ICON_MD_LIGHTBULB, RTX - Interactive), ItemSize))
			{
				m_OptionMenuOn     = false;
				m_RenderMenuOn     = !m_RenderMenuOn;
				m_ViewMenuOn       = false;
				m_CameraMenuOn     = false;
				m_LightMenuOn      = false;
				m_WayPointOn       = false;
			}

			ImGui::SameLine();
		}

		if (ImGui::Button(ICON_MD_REMOVE_RED_EYE, ImGuiH::GetLineItemSize() * 1.5f))
		{
			m_OptionMenuOn = false;
			m_RenderMenuOn = false;
			m_ViewMenuOn = !m_ViewMenuOn;
			m_CameraMenuOn = false;
			m_LightMenuOn = false;
			m_WayPointOn = false;

		}
		ImGui::SameLine();
		if (ImGui::Button(ICON_TEXT(ICON_MD_VIDEO_CAMERA_FRONT, Perspective), ItemSize))
		{
			m_OptionMenuOn = false;
			m_RenderMenuOn = false;
			m_ViewMenuOn = false;
			m_CameraMenuOn = !m_CameraMenuOn;
			m_LightMenuOn = false;
			m_WayPointOn = false;
		}
		ImGui::SameLine();
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));
		if (ImGui::Button(ICON_MD_ARROW_RIGHT, ImVec2(ItemSize.y * 0.5f, ItemSize.y)))
		{
		}
		ImGui::PopStyleVar();
		End();

		ImVec2 NextPos = StartPos;
		NextPos.x += m_Owner->GetPanelSize().x - ItemSize.x - ItemSize.y - 10.0f;
		ImGui::SetNextWindowPos(NextPos);

		Begin("Right ToolBar", 0.0f, window_flags);
		if (ImGui::Button(ICON_TEXT(ICON_MD_SUNNY, Stage Lights), ItemSize))
		{
			m_OptionMenuOn = false;
			m_RenderMenuOn = false;
			m_ViewMenuOn = false;
			m_CameraMenuOn = false;
			m_LightMenuOn = !m_LightMenuOn;
			m_WayPointOn = false;

		}
		ImGui::SameLine();
		if (ImGui::Button(ICON_MD_LOCATION_ON, ImGuiH::GetLineItemSize() * 1.5f))
		{
			m_OptionMenuOn = false;
			m_RenderMenuOn = false;
			m_ViewMenuOn = false;
			m_CameraMenuOn = false;
			m_LightMenuOn = false;
			m_WayPointOn = !m_WayPointOn;

		}
		End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.0f, 4.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6.0f, 6.0f));
		ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);

		if (m_OptionMenuOn)
		{
			ImGui::SetNextWindowPos(StartPos + ImVec2(offest, ItemSize.y + 2.0f + offest));

			bool open;
			
			ImGui::Begin("##", &open, window_flags);
			ImGuiH::MainMenuTitleSeparator();

			if (ImGui::BeginMenu("Navigation"))
			{
				ImGuiH::MainMenuTitleSeparator();

				ImGui::BeginGroup();
				ImGui::MenuItem("Navigation Speed");
				ImGui::SameLine(200.0f);
				ImGui::PushItemWidth(100.0f);
				static float test = 0.0f;
				ImGui::DragFloat("##", &test);
				ImGui::PopItemWidth();
				ImGui::SameLine(300.0f);
				ImGuiH::DrawResetIcon(true);
				ImGui::EndGroup();

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

			ImGui::End();
		}
	
		if (m_RenderMenuOn)
		{
			ImGui::SetNextWindowPos(StartPos + ImVec2(ItemSize.y + offest, ItemSize.y + 2.0f + offest));

			bool open;
			ImGui::Begin("##", &open, window_flags);
			ImGuiH::MainMenuTitleSeparator();

			if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, RTX-Real-Time))) {}
			if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, RTX-Interactive(Path Tracing)))) {}
			if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, RTX-Accurate(lray)))) {}
			if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Pixar-Storm))) {}
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
			ImGui::SeparatorText("Rendering Settings");
			ImGui::PopStyleColor();
			if (ImGui::BeginMenu(ICON_TEXT(ICON_EMPTY, Load from Preset))) 
			{
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Draft))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Medium))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Final))) {}
				ImGui::Separator();
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Load Preset From File))) {}

				ImGui::EndMenu();
			}
			if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Save Current as Preset))) {}
			if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Reset to Defaults))) {}
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
			ImGui::SeparatorText("Rendering Mode");
			ImGui::PopStyleColor();
			if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Default))) {}
			if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Wireframe))) {}
			if (ImGui::BeginMenu(ICON_TEXT(ICON_EMPTY, Debug View))) 
			{
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Off))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, 3D Motion Vectors[WARNING: Flashing Colors]))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Barycentrics))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Beauty After Tonemap))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Beauty Before Tonemap))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Depth))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Instance ID))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Interpolated Normal))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Heat Map: Any Hit))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Heat Map: Intersection))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Heat Map: Timing))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, SDG: Cross Correspondence))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, SDG: Motion))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Semantic ID))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Tangent U))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Tangent V))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Texture Coordinates 0))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Texture Coordinates 1))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Triangle Normal))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Triangle Normal(OctEnc)))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Wireframe))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, PT Adaptive Sampling Error[WARNING: Flashing Colors]))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, PT Denoised Result))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, PT Noisy Result))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, PT AOV Background))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, PT AOV Diffuse Filter))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, PT AOV Direct illumation))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, PT AOV Global illumation))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, PT AOV Reflections))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, PT AOV Reflection Filter))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, PT AOV Refractions))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, PT AOV Refraction Filter))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, PT AOV Self-illumination))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, PT AOV Volumes))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, PT AOV World Normal))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, PT AOV World Postion))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, PT AOV Z-Depth))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, PT AOV Multimatte0))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, PT AOV Multimatte1))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, PT AOV Multimatte2))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, PT AOV Multimatte3))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, PT AOV Multimatte4))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, PT AOV Multimatte5))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, PT AOV Multimatte6))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, PT AOV Multimatte7))) {}

				ImGui::EndMenu();
			}
			ImGui::Separator();
			if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Camera Light))) {}
			if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Disable Materials(White Mode)))) {}
			ImGui::Separator();
			if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Preferences))) {}

			ImGui::End();
		}

		if (m_ViewMenuOn)
		{
			ImGui::SetNextWindowPos(StartPos + ImVec2(ItemSize.y + ItemSize.x + 2.0f * offest, ItemSize.y + 2.0f + offest));

			bool open;
			ImGui::Begin("##", &open, window_flags);
			ImGuiH::MainMenuTitleSeparator();

			if (ImGui::BeginMenu(ICON_TEXT(ICON_EMPTY, Heads Up Display)))
			{
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, FPS))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Device Memory))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Process Memory))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Resolution))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Progress))) {}
				ImGui::Separator();
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Camera Speed))) {}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu(ICON_TEXT(ICON_EMPTY, Show By Type)))
			{
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Camera))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Lights))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Skeletons))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Audio))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Waypoint))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Units))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Measurements))) {}
				ImGui::Separator();
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Meshes))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, NavMesh))) {}
				if (ImGui::BeginMenu(ICON_TEXT(ICON_EMPTY, Physics)))
				{
					if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Joints))) {}
					if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Simulation Settings))) {}
					if (ImGui::BeginMenu(ICON_TEXT(ICON_EMPTY, Colliders)))
					{
						if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, None))) {}
						if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Selected))) {}
						if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, All))) {}
						if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Simplify at Distance))) {}
						ImGui::Separator();
						if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Normals))) {}

						ImGui::EndMenu();
					}
					if (ImGui::BeginMenu(ICON_TEXT(ICON_EMPTY, Mesh Properties)))
					{
						if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, None))) {}
						if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Selected))) {}
						if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, All))) {}

						ImGui::EndMenu();
					}
					if (ImGui::BeginMenu(ICON_TEXT(ICON_EMPTY, Tendons)))
					{
						if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, None))) {}
						if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Selected))) {}
						if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, All))) {}

						ImGui::EndMenu();
					}
					if (ImGui::BeginMenu(ICON_TEXT(ICON_EMPTY, Deformable Body)))
					{
						if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, None))) {}
						if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Selected))) {}
						if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, All))) {}
						ImGui::Separator();
						if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Simulation))) {}
						if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Collision))) {}

						ImGui::EndMenu();
					}
					if (ImGui::BeginMenu(ICON_TEXT(ICON_EMPTY, Attachments)))
					{
						if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, None))) {}
						if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Selected))) {}
						if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, All))) {}
						ImGui::Separator();
						if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Hide Actor 0))) {}
						if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Hide Actor 1))) {}

						ImGui::EndMenu();
					}
					if (ImGui::BeginMenu(ICON_TEXT(ICON_EMPTY, Particles)))
					{
						if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, None))) {}
						if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Selected))) {}
						if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, All))) {}
						ImGui::Separator();
						if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Particles))) {}
						if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Flush Surface))) {}
						if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Cloth Particles))) {}
						if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Cloth Mesh))) {}
						ImGui::Separator();
						if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Diffuse Particles))) {}

						ImGui::EndMenu();
					}
					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu(ICON_TEXT(ICON_EMPTY, Show By Purpose)))
			{
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Guide))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Proxy))) {}
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Render))) {}

				ImGui::EndMenu();
			}
			ImGui::Separator();
			if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Selection Outline))) {}
			if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Axis))) {}
			if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Grid))) {}

			ImGui::End();
		}

		if (m_CameraMenuOn)
		{
			ImGui::SetNextWindowPos(StartPos + ImVec2(2.0f * ItemSize.y + ItemSize.x + 3.0f * offest, ItemSize.y + 2.0f + offest));

			bool open;
			ImGui::Begin("##", &open, window_flags);
			ImGuiH::MainMenuTitleSeparator();

			if (ImGui::BeginMenu(ICON_TEXT(ICON_EMPTY, Cameras)))
			{
				if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, No camera))) {}

				ImGui::EndMenu();
			}
			if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Perspective))) {}
			if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Top))) {}
			if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Front))) {}
			if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Right))) {}
			ImGui::Separator();
			if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Sequencer Camera Sync))) {}
			ImGui::Separator();
			if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Create from View))) {}

			ImGui::End();
		}

		if (m_LightMenuOn)
		{
			ImGui::SetNextWindowPos(NextPos + ImVec2(offest, ItemSize.y + 2.0f + offest));

			bool open;
			ImGui::Begin("##", &open, window_flags);
			ImGuiH::MainMenuTitleSeparator();

			if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Lights Off))) {}
			if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Camera Light))) {}
			if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Stage Lights))) {}
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
			ImGui::SeparatorText("Light Rigs");
			ImGui::PopStyleColor();
			if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Colored Lights))) {}
			if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Dafault))) {}
			if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Grey Studio))) {}
			if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Sunny Sky))) {}
			ImGui::Separator();
			if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Use auto light rig on startup))) {}
			ImGui::Separator();
			if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Add Current Light Rig to Stage))) {}
			ImGui::End();
		}

		ImGui::PopItemFlag();
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(7);
	}
}