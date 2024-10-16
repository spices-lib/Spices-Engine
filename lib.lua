-- @file Includes Premake.
-- @brief Defines all Solutions Includes.
-- @author The Cherno & Spices.

-- Includes
IncludeDir                                = {}
IncludeDir["GLFW"]                        = "%{wks.location}/SpicesEngine/vendor/GLFW/include"                                -- Include: GLFW
IncludeDir["VulkanSDK"]                   = "C:/VulkanSDK/1.3.275.0/Include"                                                  -- Include: VulkanSDK
IncludeDir["stb_image"]                   = "%{wks.location}/SpicesEngine/vendor/stb_image"                                   -- Include: stb_image
IncludeDir["glm"]                         = "%{wks.location}/SpicesEngine/vendor/glm"                                         -- Include: glm
IncludeDir["ImGui"]                       = "%{wks.location}/SpicesEngine/vendor/imgui"                                       -- Include: ImGui
IncludeDir["entt"]                        = "%{wks.location}/SpicesEngine/vendor/entt/include"                                -- Include: entt
IncludeDir["tinyobjloader"]               = "%{wks.location}/SpicesEngine/vendor/tinyobjloader"                               -- Include: tinyobjloader
IncludeDir["yaml_cpp"]                    = "%{wks.location}/SpicesEngine/vendor/yaml-cpp/include"                            -- Include: yaml_cpp
IncludeDir["rapidyaml"]                   = "%{wks.location}/SpicesEngine/vendor/rapidyaml"                                   -- Include: rapidyaml
IncludeDir["ImPlot"]                      = "%{wks.location}/SpicesEngine/vendor/implot"                                      -- Include: ImPlot
IncludeDir["NvAftermath"]                 = "%{wks.location}/SpicesEngine/vendor/NvAftermath/include"                         -- Include: NvAftermath version 2024.2.0.24200
IncludeDir["NvPerf"]                      = "%{wks.location}/SpicesEngine/vendor/NvPerf/include"                              -- Include: NvPerf version 2024.1
IncludeDir["NvPerfUtility"]               = "%{wks.location}/SpicesEngine/vendor/NvPerfUtility/include"                       -- Include: NvPerfUtility version 2024.1
IncludeDir["NVTX"]                        = "%{wks.location}/SpicesEngine/vendor/NVTX/c/include"                              -- Include: NVTX
IncludeDir["spdlog"]                      = "%{wks.location}/SpicesEngine/vendor/spdlog/include"                              -- Include: spdlog
IncludeDir["ImGuizmo"]                    = "%{wks.location}/SpicesEngine/vendor/ImGuizmo"                                    -- Include: ImGuizmo
IncludeDir["tracy"]                       = "%{wks.location}/SpicesEngine/vendor/tracy/public"                                -- Include: tracy
IncludeDir["IconFontCppHeaders"]          = "%{wks.location}/SpicesEngine/vendor/IconFontCppHeaders"                          -- Include: IconFontCppHeaders
IncludeDir["HoudiniEngine"]               = "%{wks.location}/SpicesEngine/vendor/HoudiniEngine"                               -- Include: HoudiniEngine
IncludeDir["taskflow"]                    = "%{wks.location}/SpicesEngine/vendor/taskflow"                                    -- Include: taskflow
IncludeDir["meshoptimizer"]               = "%{wks.location}/SpicesEngine/vendor/meshoptimizer"                               -- Include: meshoptimizer
IncludeDir["VulkanMemoryAllocator"]       = "%{wks.location}/SpicesEngine/vendor/VulkanMemoryAllocator/include"               -- Include: VulkanMemoryAllocator
IncludeDir["GKlib"]                       = "%{wks.location}/SpicesEngine/vendor/GKlib"                                       -- Include: GKlib
IncludeDir["METIS"]                       = "%{wks.location}/SpicesEngine/vendor/METIS"                                       -- Include: METIS
IncludeDir["ktx"]                         = "%{wks.location}/SpicesEngine/vendor/KTX-Software"                                -- Include: ktx
IncludeDir["shaderc"]                     = "%{wks.location}/SpicesEngine/vendor/shaderc"                                     -- Include: shaderc
IncludeDir["glslang"]                     = "%{IncludeDir.VulkanSDK}/glslang"                                                 -- Include: glslang
IncludeDir["googletest"]                  = "%{wks.location}/SpicesTest/vendor/googletest/googletest/include"                 -- Include: googletest
IncludeDir["googlemock"]                  = "%{wks.location}/SpicesTest/vendor/googletest/googlemock/include"                 -- Include: googlemock

-- Library Directries Folder
LibraryDir                                = {}
LibraryDir["VulkanSDK"]                   = "C:/VulkanSDK/1.3.275.0/Lib"                                                      -- Library Directries: VulkanSDK Folder
LibraryDir["VulkanSDKDLL"]                = "%{LibraryDir.VulkanSDK}/../Bin"                                                  -- Library Directries: VulkanSDK dll Folder
LibraryDir["NvAftermath"]                 = "%{wks.location}/SpicesEngine/vendor/NvAftermath/lib/x64"                         -- Library Directries: NvAftermath Folder
LibraryDir["NvPerf"]                      = "%{wks.location}/SpicesEngine/vendor/NvPerf/lib"                                  -- Library Directries: NvPerf Folder
LibraryDir["HoudiniEnginedll"]            = "C:/Program Files/Side Effects Software/Houdini 20.5.278/bin"                     -- Library Directries: HoudiniEnginedll Folder
LibraryDir["HoudiniEnginelib"]            = "C:/Program Files/Side Effects Software/Houdini 20.5.278/custom/houdini/dsolib"   -- Library Directries: HoudiniEnginelib Folder

-- Library Directries
Library                                   = {}
Library["VulkanSDK"]                      = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"                                            -- Library Directries: VulkanSDK
Library["NvAftermath"]                    = "%{LibraryDir.NvAftermath}/GFSDK_Aftermath_Lib.x64.lib"                           -- Library Directries: NvAftermath
Library["NvPerf"]                         = "%{LibraryDir.NvPerf}/nvperf_grfx_host.lib"                                       -- Library Directries: NvPerf
Library["HoudiniEnginelib"]               = "%{LibraryDir.HoudiniEnginelib}/libHAPIL.lib"                                     -- Library Directries: HoudiniEnginelib

Library["shaderc_debug"]                  = "%{LibraryDir.VulkanSDK}/shaderc_sharedd.lib"                                     -- Library Directries: shaderc_debug
Library["shaderc_utils_debug"]            = "%{LibraryDir.VulkanSDK}/shaderc_utild.lib"                                       -- Library Directries: shaderc_utils_debug

Library["shaderc_release"]                = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"                                      -- Library Directries: shaderc_release
Library["shaderc_utils_release"]          = "%{LibraryDir.VulkanSDK}/shaderc_util.lib"                                        -- Library Directries: shaderc_utils_release