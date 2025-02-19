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
        * @brief Search String.
        */
        static std::string searchString;
        static bool isEnableSearch = false;

        /**
        * @brief Begin render Search Input Text.
        */
        {
            SPICES_PROFILE_ZONEN("ImguiProperty::Search");

            ImGui::Spacing();
            ImGui::PushItemWidth(m_PanelSize.x - ImGuiH::GetLineItemSize().x * 2.0f - ImGui::GetStyle().WindowPadding.x);
            static char search[256] = {};
            if (ImGui::InputTextWithHint("##", ICON_TEXT(ICON_MD_SEARCH, Search), search, 128))
            {
                searchString = std::string(search);
                if (searchString.size() == 0) isEnableSearch = false;
                else isEnableSearch = true;
            }
            ImGui::PopItemWidth();

            ImGui::SameLine(m_PanelSize.x - ImGuiH::GetLineItemSize().x * 2.0f);
            ImGui::Button(ICON_MD_FILTER_ALT, ImGuiH::GetLineItemSize());
            ImGui::SameLine(m_PanelSize.x - ImGuiH::GetLineItemSize().x * 1.0f);
            ImGui::Button(ICON_MD_REORDER, ImGuiH::GetLineItemSize());
            ImGui::Spacing();
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
                if (ImGui::InputTextWithHint("##", ICON_TEXT_ROW(" ", ss.c_str()), text, 128)) {}
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
                if (ImGui::InputTextWithHint("##", ICON_TEXT_ROW(" ", ss.str().c_str()), textpath, 128)) {}
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

            entt::entity item = static_cast<entt::entity>(*m_FrameInfo.m_PickEntityID.end_k());
            Entity entity(item, m_FrameInfo.m_World.get());

            DrawComponent<TransformComponent>       ("Transform", entity);
            DrawComponent<CameraComponent>          ("Camera", entity);
            DrawComponent<DirectionalLightComponent>("DirectionalLight", entity);
            DrawComponent<PointLightComponent>      ("PointLight", entity);
            DrawComponent<MeshComponent>            ("Mesh", entity);
            DrawComponent<NativeScriptComponent>    ("NativeScript", entity);
            DrawComponent<SkyBoxComponent>          ("SkyBox", entity);
            DrawComponent<SpriteComponent>          ("Sprite", entity);
            DrawComponent<LandscapeComponent>       ("Landscape", entity);
            DrawComponent<ParticleComponent>        ("Particle", entity);
            DrawComponent<EntityComponent>          ("Attachment", entity);
            DrawComponent<TagComponent>             ("Tag", entity);
            DrawComponent<UUIDComponent>            ("UUID", entity);

            if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight))
            {
                if (ImGui::BeginMenu("Add Component"))
                {
                    if (ImGui::MenuItem("CameraComponent"))
                    {
                        AsyncTask(ThreadPoolEnum::Game, [=]() { Entity(item, m_FrameInfo.m_World.get()).AddComponent<CameraComponent>(); });
                    }
                    if (ImGui::MenuItem("DirectionalLightComponent"))
                    {
                        AsyncTask(ThreadPoolEnum::Game, [=]() { Entity(item, m_FrameInfo.m_World.get()).AddComponent<DirectionalLightComponent>(); });
                    }
                    if (ImGui::MenuItem("LandscapeComponent"))
                    {
                        AsyncTask(ThreadPoolEnum::Game, [=]() { Entity(item, m_FrameInfo.m_World.get()).AddComponent<LandscapeComponent>(); });
                    }
                    if (ImGui::MenuItem("MeshComponent"))
                    {
                        AsyncTask(ThreadPoolEnum::Game, [=]() { Entity(item, m_FrameInfo.m_World.get()).AddComponent<MeshComponent>(); });
                    }
                    if (ImGui::MenuItem("NativeScriptComponent"))
                    {
                        AsyncTask(ThreadPoolEnum::Game, [=]() { Entity(item, m_FrameInfo.m_World.get()).AddComponent<NativeScriptComponent>(); });
                    }
                    if (ImGui::MenuItem("ParticleComponent"))
                    {
                        AsyncTask(ThreadPoolEnum::Game, [=]() { Entity(item, m_FrameInfo.m_World.get()).AddComponent<ParticleComponent>(); });
                    }
                    if (ImGui::MenuItem("PointLightComponent"))
                    {
                        AsyncTask(ThreadPoolEnum::Game, [=]() { Entity(item, m_FrameInfo.m_World.get()).AddComponent<PointLightComponent>(); });
                    }
                    if (ImGui::MenuItem("SkyBoxComponent"))
                    {
                        AsyncTask(ThreadPoolEnum::Game, [=]() { Entity(item, m_FrameInfo.m_World.get()).AddComponent<SkyBoxComponent>(); });
                    }
                    if (ImGui::MenuItem("SpriteComponent"))
                    {
                        AsyncTask(ThreadPoolEnum::Game, [=]() { Entity(item, m_FrameInfo.m_World.get()).AddComponent<SpriteComponent>(); });
                    }
                    ImGui::MenuItem("WidgetComponent");

                    ImGui::EndMenu();
                }
                ImGui::EndPopup();
            }
        }

		/**
		* @brief End render Property.
		*/
        ImGui::PopStyleVar();
		End();
	}
}