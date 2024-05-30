/**
* @file ImguiGizmos.cpp.
* @brief The ImguiGizmos Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "ImguiHelper.h"
#include "Core/Library/FileLibrary.h"

namespace Spiecs {

    void ImGuiH::SetStyle()
    {
        ZoneScoped;

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
        style.ColorButtonPosition           = ImGuiDir_Left;
        style.FrameRounding                 = 4.0f;
        style.FrameBorderSize               = 0.0f;
        style.FramePadding                  = ImVec2(2.0f, 2.0f);
        style.GrabRounding                  = 4.0f;
        style.IndentSpacing                 = 12.0f;
        style.ItemSpacing                   = ImVec2(2.0f, 2.0f);
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
        ZoneScoped;

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
    }

    float ImGuiH::GetDPIScale()
    {
        ZoneScoped;

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