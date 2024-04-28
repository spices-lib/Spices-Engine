IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/SpiecsEngine/vendor/GLFW/include"
IncludeDir["VulkanSDK"] = "C:/VulkanSDK/1.3.275.0/Include"
IncludeDir["stb_image"] = "%{wks.location}/SpiecsEngine/vendor/stb_image"
IncludeDir["glm"] = "%{wks.location}/SpiecsEngine/vendor/glm"
IncludeDir["ImGui"] = "%{wks.location}/SpiecsEngine/vendor/imgui"
IncludeDir["entt"] = "%{wks.location}/SpiecsEngine/vendor/entt/include"
IncludeDir["tinyobjloader"] = "%{wks.location}/SpiecsEngine/vendor/tinyobjloader"
IncludeDir["yaml_cpp"] = "%{wks.location}/SpiecsEngine/vendor/yaml-cpp/include"
IncludeDir["rapidyaml"] = "%{wks.location}/SpiecsEngine/vendor/rapidyaml"
IncludeDir["ImPlot"] = "%{wks.location}/SpiecsEngine/vendor/implot"

LibraryDir = {}
LibraryDir["VulkanSDK"] = "C:/VulkanSDK/1.3.275.0/Lib"

Library = {}
Library["VulkanSDK"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"