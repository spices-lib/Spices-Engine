IncludeDir = {}
IncludeDir["GLFW"]                        = "%{wks.location}/SpicesEngine/vendor/GLFW/include"
IncludeDir["VulkanSDK"]                   = "C:/VulkanSDK/1.3.275.0/Include"
IncludeDir["stb_image"]                   = "%{wks.location}/SpicesEngine/vendor/stb_image"
IncludeDir["glm"]                         = "%{wks.location}/SpicesEngine/vendor/glm"
IncludeDir["ImGui"]                       = "%{wks.location}/SpicesEngine/vendor/imgui"
IncludeDir["entt"]                        = "%{wks.location}/SpicesEngine/vendor/entt/include"
IncludeDir["tinyobjloader"]               = "%{wks.location}/SpicesEngine/vendor/tinyobjloader"
IncludeDir["yaml_cpp"]                    = "%{wks.location}/SpicesEngine/vendor/yaml-cpp/include"
IncludeDir["rapidyaml"]                   = "%{wks.location}/SpicesEngine/vendor/rapidyaml"
IncludeDir["ImPlot"]                      = "%{wks.location}/SpicesEngine/vendor/implot"
IncludeDir["NvPerf"]                      = "%{wks.location}/SpicesEngine/vendor/NvPerf/include"
IncludeDir["NvPerfUtility"]               = "%{wks.location}/SpicesEngine/vendor/NvPerfUtility/include"
IncludeDir["spdlog"]                      = "%{wks.location}/SpicesEngine/vendor/spdlog/include"
IncludeDir["ImGuizmo"]                    = "%{wks.location}/SpicesEngine/vendor/ImGuizmo"
IncludeDir["tracy"]                       = "%{wks.location}/SpicesEngine/vendor/tracy/public"
IncludeDir["IconFontCppHeaders"]          = "%{wks.location}/SpicesEngine/vendor/IconFontCppHeaders"
IncludeDir["HoudiniEngine"]               = "%{wks.location}/SpicesEngine/vendor/HoudiniEngine"
IncludeDir["googletest"]                  = "%{wks.location}/SpicesTest/vendor/googletest/googletest/include"
IncludeDir["googlemock"]                  = "%{wks.location}/SpicesTest/vendor/googletest/googlemock/include"

LibraryDir = {}
LibraryDir["VulkanSDK"]                   = "C:/VulkanSDK/1.3.275.0/Lib"
LibraryDir["NvPerfUtility"]               = "%{wks.location}/SpicesEngine/vendor/NvPerfUtility/lib"
LibraryDir["HoudiniEnginedll"]            = "C:/Program Files/Side Effects Software/Houdini 20.0.751/bin"
LibraryDir["HoudiniEnginelib"]            = "C:/Program Files/Side Effects Software/Houdini 20.0.751/custom/houdini/dsolib"

Library = {}
Library["VulkanSDK"]                      = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["NvPerfUtility"]                  = "%{LibraryDir.NvPerfUtility}/nvperf_grfx_host.dll"
Library["HoudiniEnginelib"]               = "%{LibraryDir.HoudiniEnginelib}/libHAPIL.lib"