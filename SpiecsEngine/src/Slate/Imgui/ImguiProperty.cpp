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

        if (m_FrameInfo.m_PickEntityID.size() == 0)
        {
            End();

            return;
        }

        // @todo remove.
        {
            ImGui::PushItemWidth(100.0f);
            ImGui::Button(ICON_TEXT(ICON_MD_ADD, Add));
            ImGui::PopItemWidth();
            ImGui::SameLine();

            std::string ss;
            ss = *m_FrameInfo.m_PickEntityID.end();
            ImGui::PushItemWidth(m_PanelSize.x - 100.0f);
            char text[128] = "";
            if(ImGui::InputTextWithHint("##", ss.c_str(), text, 128)) {}
            ImGui::PopItemWidth();



            ImGui::PushItemWidth(100.0f);
            ImGui::Text("Prim Path");
            ImGui::PopItemWidth();
            ImGui::SameLine();

            ss = "/" + ss;
            ImGui::PushItemWidth(m_PanelSize.x - 100.0f);
            char textpath[128] = "";
            if (ImGui::InputTextWithHint("##", ss.c_str(), textpath, 128)) {}
            ImGui::PopItemWidth();

            ImGui::PushItemWidth(100.0f);
            ImGui::Text("Instanceable");
            ImGui::PopItemWidth();
            //ImGui::SameLine();

            bool isChecked;
            ImGui::Checkbox("##", &isChecked);
        }

		/**
		* @brief End render Property.
		*/
		End();
	}
}