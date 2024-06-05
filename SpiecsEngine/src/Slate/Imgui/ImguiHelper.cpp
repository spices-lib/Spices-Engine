/**
* @file ImguiGizmos.cpp.
* @brief The ImguiGizmos Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "ImguiHelper.h"
#include "Core/Library/FileLibrary.h"

// This file Only Can include once.
#include <imgui.cpp>

namespace Spiecs {

    void ImGuiH::SetStyle()
    {
        SPIECS_PROFILE_ZONE;

        /**
        * @brief Set Dark Style.
        */
        ImGui::StyleColorsDark();

        /**
        * @brief Set Style.
        */
        ImGuiStyle& style                   = ImGui::GetStyle();
        style.WindowRounding                = 0.0f;
        style.WindowBorderSize              = 0.0f;
        style.WindowPadding                 = ImVec2(0.0f, 0.0f);
        style.ColorButtonPosition           = ImGuiDir_Left;
        style.FrameRounding                 = 4.0f;
        style.FrameBorderSize               = 0.0f;
        style.FramePadding                  = ImVec2(2.0f, 2.0f);
        style.GrabRounding                  = 4.0f;
        style.IndentSpacing                 = 12.0f;
        style.ItemSpacing                   = ImVec2(2.0f, 2.0f);
        style.ScrollbarSize                 = 4.0f;
        style.SeparatorTextAlign            = ImVec2(0.0f, 0.5f);
        style.SeparatorTextPadding          = ImVec2(0.0f, 3.0f);
        style.SeparatorTextBorderSize       = 1.0f;
        style.Colors[ImGuiCol_WindowBg]     = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
        style.Colors[ImGuiCol_MenuBarBg]    = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
        style.Colors[ImGuiCol_ScrollbarBg]  = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
        style.Colors[ImGuiCol_PopupBg]      = ImVec4(0.135f, 0.135f, 0.135f, 1.0f);
        style.Colors[ImGuiCol_Border]       = ImVec4(0.4f, 0.4f, 0.4f, 0.5f);
        style.Colors[ImGuiCol_FrameBg]      = ImVec4(0.05f, 0.05f, 0.05f, 0.5f);

        /**
        * @brief Set Botton Style.
        */
        style.Colors[ImGuiCol_Button]       = style.Colors[ImGuiCol_WindowBg];
        style.Colors[ImGuiCol_ButtonActive] = style.Colors[ImGuiCol_FrameBg];
        style.Colors[ImGuiCol_ButtonHovered]= ImVec4(0.3f, 0.3f, 0.3f, 1.0f);

        /**
        * @brief Set Normal Style.
        */
        ImVec4                normal_color(0.465f, 0.465f, 0.525f, 1.0f);
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
        ImVec4                active_color(0.365f, 0.365f, 0.425f, 1.0f);
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
        ImVec4                hovered_color(0.565f, 0.565f, 0.625f, 1.0f);
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
        SPIECS_PROFILE_ZONE;

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
        static const ImWchar icons_ranges[] = { ICON_MIN_MD, ICON_MAX_16_MD, 0 };
        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        icons_config.GlyphMinAdvanceX = iconFontSize;
        icons_config.GlyphOffset.y = 3.0f;

        std::stringstream ss;
        ss << SPIECS_ENGINE_ASSETS_PATH << "Fonts/" << FONT_ICON_FILE_NAME_MD;
        if (FileLibrary::FileLibrary_Exists(ss.str().c_str()))
        {
            io.Fonts->AddFontFromFileTTF(ss.str().c_str(), iconFontSize, &icons_config, icons_ranges);
        }
    }

    void ImGuiH::MainDockSpace(Side side, float alpha)
    {
        SPIECS_PROFILE_ZONE;

        // Keeping the unique ID of the dock space
        ImGuiID dockspaceID = ImGui::GetID("DockSpace");

        // The dock need a dummy window covering the entire viewport.
        ImGuiViewport* viewport = ImGui::GetMainViewport();
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

    float ImGuiH::GetDPIScale()
    {
        SPIECS_PROFILE_ZONE;

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