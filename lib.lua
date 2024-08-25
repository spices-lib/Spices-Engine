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
IncludeDir["NvPerf"]                      = "%{wks.location}/SpicesEngine/vendor/NvPerf/include"                              -- Include: NvPerf
IncludeDir["NvPerfUtility"]               = "%{wks.location}/SpicesEngine/vendor/NvPerfUtility/include"                       -- Include: NvPerfUtility
IncludeDir["spdlog"]                      = "%{wks.location}/SpicesEngine/vendor/spdlog/include"                              -- Include: spdlog
IncludeDir["ImGuizmo"]                    = "%{wks.location}/SpicesEngine/vendor/ImGuizmo"                                    -- Include: ImGuizmo
IncludeDir["tracy"]                       = "%{wks.location}/SpicesEngine/vendor/tracy/public"                                -- Include: tracy
IncludeDir["IconFontCppHeaders"]          = "%{wks.location}/SpicesEngine/vendor/IconFontCppHeaders"                          -- Include: IconFontCppHeaders
IncludeDir["HoudiniEngine"]               = "%{wks.location}/SpicesEngine/vendor/HoudiniEngine"                               -- Include: HoudiniEngine
IncludeDir["taskflow"]                    = "%{wks.location}/SpicesEngine/vendor/taskflow"                                    -- Include: taskflow
IncludeDir["meshoptimizer"]               = "%{wks.location}/SpicesEngine/vendor/meshoptimizer"                               -- Include: meshoptimizer
IncludeDir["VulkanMemoryAllocator"]       = "%{wks.location}/SpicesEngine/vendor/VulkanMemoryAllocator/include"               -- Include: VulkanMemoryAllocator
IncludeDir["googletest"]                  = "%{wks.location}/SpicesTest/vendor/googletest/googletest/include"                 -- Include: googletest
IncludeDir["googlemock"]                  = "%{wks.location}/SpicesTest/vendor/googletest/googlemock/include"                 -- Include: googlemock

-- Library Directries Folder
LibraryDir                                = {}
LibraryDir["VulkanSDK"]                   = "C:/VulkanSDK/1.3.275.0/Lib"                                                      -- Library Directries: VulkanSDK Folder
LibraryDir["NvPerfUtility"]               = "%{wks.location}/SpicesEngine/vendor/NvPerfUtility/lib"                           -- Library Directries: NvPerfUtility Folder
LibraryDir["HoudiniEnginedll"]            = "C:/Program Files/Side Effects Software/Houdini 20.5.278/bin"                     -- Library Directries: HoudiniEnginedll Folder
LibraryDir["HoudiniEnginelib"]            = "C:/Program Files/Side Effects Software/Houdini 20.5.278/custom/houdini/dsolib"   -- Library Directries: HoudiniEnginelib Folder

-- Library Directries
Library                                   = {}
Library["VulkanSDK"]                      = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"                                            -- Library Directries: VulkanSDK
Library["NvPerfUtility"]                  = "%{LibraryDir.NvPerfUtility}/nvperf_grfx_host.dll"                                -- Library Directries: NvPerfUtility
Library["HoudiniEnginelib"]               = "%{LibraryDir.HoudiniEnginelib}/libHAPIL.lib"                                     -- Library Directries: HoudiniEnginelib