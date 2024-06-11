/**
* @file ImguiProperty.cpp.
* @brief The ImguiProperty Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "ImguiProperty.h"

namespace Spiecs {

	void ImguiProperty::OnRender()
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Begin render Property.
		*/
		Begin();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 4.0f));

        /**
        * @brief Begin render Search Input Text.
        */
        {
            SPIECS_PROFILE_ZONEN("ImguiProperty::Search");

            ImGui::Separator();
            ImGui::PushItemWidth(m_PanelSize.x);
            char search[128] = "";
            if (ImGui::InputTextWithHint("##", "Search", search, 128)) {}
            ImGui::PopItemWidth();
            ImGui::Separator();
        }

        /**
        * @brief End if not a select entity.
        */
        if (m_FrameInfo.m_PickEntityID.size() == 0)
        {
            SPIECS_PROFILE_ZONEN("ImguiProperty::End");

            ImGui::PopStyleVar();
            End();

            return;
        }

        /**
        * @brief Render Datails.
        */
        {
            SPIECS_PROFILE_ZONEN("ImguiProperty::Datails");

            {
                ImGui::PushID("ImguiProperty::Add");

                ImGui::Columns(2);
                ImGui::SetColumnWidth(0, ImGuiH::GetLineItemSize().x * 4.0f);
                ImGui::Button(ICON_TEXT(ICON_MD_ADD, Add));
                ImGui::NextColumn();

                std::string ss;
                ss = *m_FrameInfo.m_PickEntityID.end();
                ImGui::PushItemWidth(m_PanelSize.x - ImGuiH::GetLineItemSize().x * 4.0f);
                char text[128] = "";
                if (ImGui::InputTextWithHint("##", ss.c_str(), text, 128)) {}
                ImGui::PopItemWidth();

                ImGui::Columns(1);
                ImGui::PopID();
            }

            {
                ImGui::PushID("ImguiProperty::Prim Path");
                ImGui::Columns(2);
                ImGui::SetColumnWidth(0, ImGuiH::GetLineItemSize().x * 4.0f);
                ImGui::Text("Prim Path");
                ImGui::NextColumn();

                std::string ss;
                ss = *m_FrameInfo.m_PickEntityID.end();
                ss = "/" + ss;
                ImGui::PushItemWidth(m_PanelSize.x - ImGuiH::GetLineItemSize().x * 4.0f);
                char textpath[128] = "";
                if (ImGui::InputTextWithHint("##", ss.c_str(), textpath, 128)) {}
                ImGui::PopItemWidth();

                ImGui::Columns(1);
                ImGui::PopID();
            }

            {
                ImGui::PushID("ImguiProperty::Instanceable");
                ImGui::Columns(2);
                ImGui::SetColumnWidth(0, ImGuiH::GetLineItemSize().x * 4.0f);
                ImGui::Text("Instanceable");
                ImGui::NextColumn();

                static bool isChecked;
                ImGui::Checkbox("##", &isChecked);

                ImGui::Columns(1);
                ImGui::PopID();
            }
        }

        /**
        * @brief Render All Component.
        */
        {
            SPIECS_PROFILE_ZONEN("ImguiProperty::Components");

            Entity entity((entt::entity)*m_FrameInfo.m_PickEntityID.endk(), m_FrameInfo.m_World.get());

            DrawComponent<TransformComponent>("Transform", entity);
            DrawComponent<CameraComponent>("Camera", entity);
            DrawComponent<DirectionalLightComponent>("DirectionalLight", entity);
            DrawComponent<PointLightComponent>("PointLight", entity);
            DrawComponent<MeshComponent>("Mesh", entity);
            DrawComponent<NativeScriptComponent>("NativeScript", entity);
            DrawComponent<SkyBoxComponent>("SkyBox", entity);
            DrawComponent<SpriteComponent>("Sprite", entity);
            DrawComponent<TagComponent>("Tag", entity);
            DrawComponent<UUIDComponent>("UUID", entity);
        }

		/**
		* @brief End render Property.
		*/
        ImGui::PopStyleVar();
		End();
	}
}