/**
* @file ImguiProperty.cpp.
* @brief The ImguiProperty Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiProperty.h"

namespace Spices {

	void ImguiProperty::OnRender()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Begin render Property.
		*/
		Begin();
	    
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 4.0f));

        /**
        * @brief Begin render Search Input Text.
        */
        {
            SPICES_PROFILE_ZONEN("ImguiProperty::Search");

		    ImGui::Spacing();
            ImGui::PushItemWidth(m_PanelSize.x);
            char search[128] = "";
            if (ImGui::InputTextWithHint("##", ICON_TEXT(ICON_MD_SEARCH, Search), search, 128)) {}
            ImGui::PopItemWidth();
        }

        /**
        * @brief End if not a select entity.
        */
        if (m_FrameInfo.m_PickEntityID.size() == 0)
        {
            SPICES_PROFILE_ZONEN("ImguiProperty::End");
            
            ImGui::PopStyleVar();
            End();

            return;
        }

        /**
        * @brief Render Datails.
        */
        {
            SPICES_PROFILE_ZONEN("ImguiProperty::Datails");

            {
                ImGui::PushID("ImguiProperty::Add");

                ImGui::Columns(2, 0, false);
                ImGui::SetColumnWidth(0, ImGuiH::GetLineItemSize().x * 4.0f);
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.16f, 0.16f, 0.16f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.619f, 0.619f, 0.619f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.408f, 0.451f, 0.18f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.341f, 0.486f, 0.353f, 1.0f));
                ImGui::Button(ICON_TEXT(ICON_MD_ADD, Add));
                ImGui::PopStyleColor(4);
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
                ImGui::Columns(2, 0, false);
                ImGui::SetColumnWidth(0, ImGuiH::GetLineItemSize().x * 4.0f);
                ImGui::Text("Prim Path");
                ImGui::NextColumn();

                std::stringstream ss;
                ss << "/" << *m_FrameInfo.m_PickEntityID.end();
                ImGui::PushItemWidth(m_PanelSize.x - ImGuiH::GetLineItemSize().x * 4.0f);
                char textpath[128] = "";
                if (ImGui::InputTextWithHint("##", ss.str().c_str(), textpath, 128)) {}
                ImGui::PopItemWidth();

                ImGui::Columns(1);
                ImGui::PopID();
            }

            {
                ImGui::PushID("ImguiProperty::Instanceable");
                ImGui::Columns(2, 0, false);
                ImGui::SetColumnWidth(0, ImGuiH::GetLineItemSize().x * 4.0f);
                ImGui::Text("Instanceable");
                ImGui::NextColumn();

                static bool isChecked;
                ImGuiH::Checkbox(&isChecked);
                ImGui::Columns(1);
                ImGui::PopID();
            }
        }

        /**
        * @brief Render All Component.
        */
        {
            SPICES_PROFILE_ZONEN("ImguiProperty::Components");

            Entity entity(static_cast<entt::entity>(*m_FrameInfo.m_PickEntityID.endk()), m_FrameInfo.m_World.get());

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