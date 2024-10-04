/**
* @file ImguiGizmos.cpp.
* @brief The ImguiGizmos Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiHelper.h"
#include "Core/Library/FileLibrary.h"

// This file Only Can include once.
#include <imgui.cpp>
#include <imgui_internal.h>
#include <imgui_widgets.cpp>

namespace Spices {

    void ImGuiH::SetStyle()
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Set Dark Style.
        */
        ImGui::StyleColorsDark();

        /**
        * @brief Set Style.
        */
        ImGuiStyle& style                         = ImGui::GetStyle();
        style.WindowRounding                      = 0.0f;
        style.WindowBorderSize                    = 0.0f;
        style.WindowPadding                       = ImVec2(3.0f, 3.0f);
        style.ColorButtonPosition                 = ImGuiDir_Left;
        style.FrameRounding                       = 4.0f;
        style.FrameBorderSize                     = 0.0f;
        style.FramePadding                        = ImVec2(2.0f, 2.0f);
        style.GrabRounding                        = 4.0f;
        style.IndentSpacing                       = 12.0f;
        style.ItemSpacing                         = ImVec2(2.0f, 2.0f);
        style.ScrollbarSize                       = 4.0f;
        style.SeparatorTextAlign                  = ImVec2(0.0f, 0.5f);
        style.SeparatorTextPadding                = ImVec2(0.0f, 0.0f);
        style.SeparatorTextBorderSize             = 1.0f;
        style.GrabMinSize                         = 2.0f;
        style.Colors[ImGuiCol_WindowBg]           = ImVec4(0.27f, 0.27f, 0.27f, 1.0f);
        style.Colors[ImGuiCol_MenuBarBg]          = style.Colors[ImGuiCol_WindowBg];
        style.Colors[ImGuiCol_ScrollbarBg]        = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
        style.Colors[ImGuiCol_PopupBg]            = ImVec4(0.135f, 0.135f, 0.135f, 1.0f);
        style.Colors[ImGuiCol_Border]             = ImVec4(0.141f, 0.129f, 0.121f, 1.0f);
        style.Colors[ImGuiCol_FrameBg]            = ImVec4(0.125f, 0.129f, 0.137f, 1.0f);
        style.Colors[ImGuiCol_PopupBg]            = ImVec4(0.22f, 0.231f, 0.239f, 1.0f);
        style.Colors[ImGuiCol_TabUnfocused]       = style.Colors[ImGuiCol_WindowBg];
        style.Colors[ImGuiCol_TabActive]          = style.Colors[ImGuiCol_WindowBg];
        style.Colors[ImGuiCol_TabUnfocusedActive] = style.Colors[ImGuiCol_WindowBg];
        style.Colors[ImGuiCol_TabHovered]         = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);

        /**
        * @brief Set Botton Style.
        */
        style.Colors[ImGuiCol_Button]       = style.Colors[ImGuiCol_WindowBg];
        style.Colors[ImGuiCol_ButtonActive] = style.Colors[ImGuiCol_FrameBg];
        style.Colors[ImGuiCol_ButtonHovered]= ImVec4(0.3f, 0.3f, 0.3f, 1.0f);

        /**
        * @brief Set Normal Style.
        */
        constexpr ImVec4                normal_color(0.465f, 0.465f, 0.525f, 1.0f);
        std::vector<ImGuiCol> to_change_nrm;
        to_change_nrm.push_back(ImGuiCol_Header);
        to_change_nrm.push_back(ImGuiCol_SliderGrab);
        to_change_nrm.push_back(ImGuiCol_CheckMark);
        to_change_nrm.push_back(ImGuiCol_ResizeGrip);
        to_change_nrm.push_back(ImGuiCol_TextSelectedBg);
        to_change_nrm.push_back(ImGuiCol_Separator);
        to_change_nrm.push_back(ImGuiCol_FrameBgActive);
        for (auto c : to_change_nrm)
        {
            style.Colors[c] = normal_color;
        }

        /**
        * @brief Set Active Style.
        */
        constexpr ImVec4                active_color(0.365f, 0.365f, 0.425f, 1.0f);
        std::vector<ImGuiCol> to_change_act;
        to_change_act.push_back(ImGuiCol_HeaderActive);
        to_change_act.push_back(ImGuiCol_SliderGrabActive);
        to_change_act.push_back(ImGuiCol_ResizeGripActive);
        to_change_act.push_back(ImGuiCol_SeparatorActive);
        for (auto c : to_change_act)
        {
            style.Colors[c] = active_color;
        }

        /**
        * @brief Set Hovered Style.
        */
        constexpr ImVec4                hovered_color(0.565f, 0.565f, 0.625f, 1.0f);
        std::vector<ImGuiCol> to_change_hover;
        to_change_hover.push_back(ImGuiCol_HeaderHovered);
        to_change_hover.push_back(ImGuiCol_FrameBgHovered);
        to_change_hover.push_back(ImGuiCol_ResizeGripHovered);
        to_change_hover.push_back(ImGuiCol_SeparatorHovered);
        for (auto c : to_change_hover)
        {
            style.Colors[c] = hovered_color;
        }

        style.Colors[ImGuiCol_TitleBgActive]      = ImVec4(0.465f, 0.465f, 0.465f, 1.0f);
        style.Colors[ImGuiCol_TitleBg]            = ImVec4(0.125f, 0.125f, 0.125f, 1.0f);
        style.Colors[ImGuiCol_Tab]                = ImVec4(0.05f, 0.05f, 0.05f, 0.5f);
        style.Colors[ImGuiCol_TabHovered]         = ImVec4(0.465f, 0.495f, 0.525f, 1.0f);
        style.Colors[ImGuiCol_TabActive]          = ImVec4(0.282f, 0.290f, 0.302f, 1.0f);
        style.Colors[ImGuiCol_ModalWindowDimBg]   = ImVec4(0.465f, 0.465f, 0.465f, 0.350f);

        //Colors_ext[ImGuiColExt_Warning] = ImVec4 (1.0f, 0.43f, 0.35f, 1.0f);

        ImGui::SetColorEditOptions(ImGuiColorEditFlags_Float | ImGuiColorEditFlags_PickerHueWheel);
    }

    void ImGuiH::SetFonts(FontMode fontmode)
    {
        SPICES_PROFILE_ZONE;

        ImGuiIO& io = ImGui::GetIO();
        const float high_dpi_scale = GetDPIScale();

        /**
        * @brief Set Nicer fonts.
        */
        ImFont* font = nullptr;
        if (fontmode == FONT_MONOSPACED_SCALED)
        {
            if (font == nullptr)
            {
                const std::string p = R"(C:/Windows/Fonts/consola.ttf)";
                if (FileLibrary::FileLibrary_Exists(p.c_str()))
                {
                    font = io.Fonts->AddFontFromFileTTF(p.c_str(), 12.0f * high_dpi_scale);
                }
            }
        }
        else if (fontmode == FONT_PROPORTIONAL_SCALED)
        {
            const char* vk_path = getenv("VK_SDK_PATH");
            if (vk_path)
            {
                const std::string p = std::string(vk_path) + R"(/Samples/Layer-Samples/data/FreeSans.ttf)";
                if (FileLibrary::FileLibrary_Exists(p.c_str()))
                    font = io.Fonts->AddFontFromFileTTF(p.c_str(), 16.0f * high_dpi_scale);
            }
            if (font == nullptr)
            {
                const std::string p = R"(C:/Windows/Fonts/segoeui.ttf)";
                if (FileLibrary::FileLibrary_Exists(p.c_str()))
                    font = io.Fonts->AddFontFromFileTTF(p.c_str(), 16.0f * high_dpi_scale);
            }
        }

        if (font == nullptr)
        {
            ImFontConfig font_config = ImFontConfig();
            font_config.SizePixels = 13.0f * ((fontmode == FONT_FIXED) ? 1 : high_dpi_scale);  // 13 is the default font size
            io.Fonts->AddFontDefault(&font_config);
        }
        
        float iconFontSize = 15.0f * high_dpi_scale;

        // merge in icons from Font Awesome
        static constexpr ImWchar icons_ranges[] = { ICON_MIN_MD, ICON_MAX_16_MD, 0 };
        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        icons_config.GlyphMinAdvanceX = iconFontSize;
        icons_config.GlyphOffset.y = 3.0f;

        std::stringstream ss;
        ss << SPICES_ENGINE_ASSETS_PATH << "Fonts/" << FONT_ICON_FILE_NAME_MD;
        if (FileLibrary::FileLibrary_Exists(ss.str().c_str()))
        {
            io.Fonts->AddFontFromFileTTF(ss.str().c_str(), iconFontSize, &icons_config, icons_ranges);
        }
    }

    void ImGuiH::MainDockSpace(Side side, float alpha)
    {
        SPICES_PROFILE_ZONE;

        // Keeping the unique ID of the dock space
        ImGuiID dockspaceID = ImGui::GetID("DockSpace");

        // The dock need a dummy window covering the entire viewport.
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        // All flags to dummy window
        ImGuiWindowFlags host_window_flags = 0;
        host_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
        host_window_flags |= ImGuiWindowFlags_NoMove; //| ImGuiWindowFlags_NoDocking;
        host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        host_window_flags |= ImGuiWindowFlags_NoBackground;
        // Starting dummy window
        char label[32];
        ImFormatString(label, IM_ARRAYSIZE(label), "DockSpaceViewport_%08X", viewport->ID);
        ImGui::Begin(label, NULL, host_window_flags);

        // The central node is transparent, so that when UI is draw after, the image is visible
        // Auto Hide Bar, no title of the panel
        // Center is not dockable, that is for the scene
        ImGuiDockNodeFlags dockspaceFlags =
            ImGuiDockNodeFlags_PassthruCentralNode;
            //ImGuiDockNodeFlags_AutoHideTabBar;
            //ImGuiDockNodeFlags_NoDockingInCentralNode;

        // Building the splitting of the dock space is done only once
        if (!ImGui::DockBuilderGetNode(dockspaceID))
        {

            ImGui::DockBuilderRemoveNode(dockspaceID);
            ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_DockSpace);

            ImGuiID dock_main_id = dockspaceID;

            // Slitting all 4 directions
            ImGuiID id_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.2f, nullptr, &dock_main_id);
            ImGui::DockBuilderDockWindow(side == Side::Left ? "Docking Space" : "Dock_left", id_left);

            ImGuiID id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.2f, nullptr, &dock_main_id);
            ImGui::DockBuilderDockWindow(side == Side::Right ? "Docking Space" : "Dock_right", id_right);

            ImGuiID id_up = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.2f, nullptr, &dock_main_id);
            ImGui::DockBuilderDockWindow(side == Side::Up ? "Docking Space" : "Dock_up", id_up);

            ImGuiID id_down = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.2f, nullptr, &dock_main_id);
            ImGui::DockBuilderDockWindow(side == Side::Down ? "Docking Space" : "Dock_down", id_down);

            ImGui::DockBuilderDockWindow(side == Side::Scene ? "Docking Space" : "Scene", dock_main_id);  // Center

            ImGui::DockBuilderFinish(dock_main_id);
        }

        // Setting the panel to blend with alpha
        ImVec4 col = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(col.x, col.y, col.z, alpha));

        ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspaceFlags);
        ImGui::PopStyleColor();
        ImGui::End();
    }

    ImVec2 ImGuiH::GetLineItemSize()
    {
        float x = ImGui::GetFont()->FontSize * ImGui::GetIO().FontGlobalScale + ImGui::GetStyle().FramePadding.y * 2.0f;
        return ImVec2(x, x);
    }

    bool ImGuiH::DrawResetIcon(const bool& isMove)
    {
        bool IsReset = false;

        if (isMove)
        {
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyle().Colors[ImGuiCol_Button]);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.31f, 0.49f, 0.62f, 1.0f));
            IsReset = ImGui::Button(ICON_MD_SQUARE, ImGuiH::GetLineItemSize());
            ImGui::PopStyleColor(2);
        }
        else
        {
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyle().Colors[ImGuiCol_Button]);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyle().Colors[ImGuiCol_Button]);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
            //ImGui::Button(ICON_MD_CROP_SQUARE, ImGuiH::GetLineItemSize());
            ImGui::SetWindowFontScale(0.65f);
            ImGui::Button(ICON_MD_STOP, ImGuiH::GetLineItemSize());
            ImGui::SetWindowFontScale(1.0f);
            ImGui::PopStyleColor(3);
        }

        return IsReset;
    }

    void ImGuiH::Checkbox(bool* isChecked)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));
        if (ImGui::Button(*isChecked ? ICON_MD_CHECK_BOX : ICON_MD_SQUARE, ImGuiH::GetLineItemSize()))
        {
            *isChecked = !*isChecked;
        }
        ImGui::PopStyleColor();
    }

    void ImGuiH::MainMenuTitleSeparator()
    {
        ImGui::SetWindowFontScale(0.5f);
        ImGui::SeparatorText("##");
        ImGui::SetWindowFontScale(1.0f);
    }

    void ImGuiH::DrawPropertyItem(const std::string& itemName, float columeWidth, std::function<void()> func)
    {
        SPICES_PROFILE_ZONEN("DrawPropertyItem");

        ImGui::PushID(itemName.c_str());
        ImGui::Columns(2, 0, false);

        ImGui::SetColumnWidth(0, columeWidth);
        ImGui::Text(itemName.c_str());
        ImGui::NextColumn();

        func();

        ImGui::Columns(1);
        ImGui::PopID();
    }

    void ImGuiH::DrawTreeTitle(const std::string& treeName, std::function<void()> optionFunc, std::function<void()> treeFunc)
    {
        SPICES_PROFILE_ZONE;

        constexpr ImGuiTreeNodeFlags treeNodeFlags = 
				ImGuiTreeNodeFlags_DefaultOpen      | 
				ImGuiTreeNodeFlags_AllowItemOverlap | 
				ImGuiTreeNodeFlags_Framed           | 
				ImGuiTreeNodeFlags_FramePadding     ;

        ImGui::PushStyleColor(ImGuiCol_Header            , ImVec4(0.196f, 0.204f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered     , ImVec4(0.164f, 0.18f, 0.184f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_HeaderActive      , ImVec4(0.164f, 0.18f, 0.184f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_WindowBg          , ImVec4(0.196f, 0.204f, 0.2f, 1.0f));

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding   , ImVec2(2.0f, 6.0f));
        const bool open = ImGui::TreeNodeEx(treeName.c_str(), treeNodeFlags, treeName.c_str());
        ImGui::SameLine(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
        ImGui::PushStyleColor(ImGuiCol_Button           , ImVec4(0.16f, 0.16f, 0.16f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered    , ImVec4(0.608f, 0.608f, 0.608f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive     , ImVec4(0.396f, 0.439f, 0.168f, 1.0f));
        if (ImGui::Button(ICON_MD_FILTER_VINTAGE, ImGuiH::GetLineItemSize()))
        {
            ImGui::OpenPopup(treeName.c_str());
        }
        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar();
        if (ImGui::BeginPopup(treeName.c_str()))
        {
            if (optionFunc)
            {
                optionFunc();
            }
            ImGui::EndPopup();
        }
        if (open)
        {
            if (treeFunc)
            {
                treeFunc();
            }
            ImGui::TreePop();
        }

        ImGui::PopStyleColor(4);
    }

    bool ImGuiH::S_DragScalarN(const char* label, ImGuiDataType data_type, void* p_data, int components, float v_speed, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags)
    {
        using namespace ImGui;

        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;
        
        ImGuiContext& g = *GImGui;
        bool value_changed = false;
        BeginGroup();
        PushID(label);
        PushMultiItemsWidths(components, CalcItemWidth());
        size_t type_size = GDataTypeInfo[data_type].Size;
        for (int i = 0; i < components; i++)
        {
            PushID(i);
            if (i > 0)
                SameLine(0, g.Style.ItemInnerSpacing.x);
            const void* cp_min = (float*)p_min + i;
            const void* cp_mmx = (float*)p_max + i;
            value_changed |= DragScalar("", data_type, p_data, v_speed, cp_min, cp_mmx, format, flags);
            PopID();
            PopItemWidth();
            p_data = (void*)((char*)p_data + type_size);
        }
        PopID();
        
        const char* label_end = FindRenderedTextEnd(label);
        if (label != label_end)
        {
            SameLine(0, g.Style.ItemInnerSpacing.x);
            TextEx(label, label_end);
        }
        
        EndGroup();
        return value_changed;
    }

    float ImGuiH::GetDPIScale()
    {
        SPICES_PROFILE_ZONE;

        // Cached DPI scale, so that this doesn't change after the first time code calls getDPIScale.
        // A negative value indicates that the value hasn't been computed yet.
        static float cached_dpi_scale = -1.0f;

        if (cached_dpi_scale < 0.0f)
        {
            // Compute the product of the monitor DPI scale and any DPI scale
            // set in the NVPRO_DPI_SCALE variable.
            cached_dpi_scale = 1.0f;

            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            if (monitor != nullptr)
            {
                float y_scale;
                glfwGetMonitorContentScale(monitor, &cached_dpi_scale, &y_scale);
            }
            // Otherwise, GLFW isn't initialized yet, but might be in the future.
            // (Note that this code assumes all samples use GLFW.)

            // Multiply by the value of the NVPRO_DPI_SCALE environment variable.
            const char* dpi_env = getenv("NVPRO_DPI_SCALE");
            if (dpi_env)
            {
                const float parsed_dpi_env = strtof(dpi_env, nullptr);
                if (parsed_dpi_env != 0.0f)
                {
                    cached_dpi_scale *= parsed_dpi_env;
                }
            }

            cached_dpi_scale = (cached_dpi_scale > 0.0f ? cached_dpi_scale : 1.0f);
        }

        return cached_dpi_scale;
    }
}

namespace ImGui {

    void CenteredText(const char* label, const ImVec2& size_arg)
    {
        ImGuiWindow* window = GetCurrentWindow();

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = CalcTextSize(label, NULL, true);

        ImVec2 pos = window->DC.CursorPos;
        ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

        const ImVec2 pos2 = ImVec2((pos.x + size.x), (pos.y + size.y));
        const ImRect bb(pos, pos2);

        ItemSize(size, style.FramePadding.y);

        const ImVec2 pos_min = ImVec2((bb.Min.x + style.FramePadding.x), (bb.Min.y + style.FramePadding.y));
        const ImVec2 pos_max = ImVec2((bb.Max.x - style.FramePadding.x), (bb.Max.y - style.FramePadding.y));

        RenderTextClipped(pos_min, pos_max, label, NULL, &label_size, style.ButtonTextAlign, &bb);
    }
}