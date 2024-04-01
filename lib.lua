IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/SpiecsEngine/vendor/GLFW/include"
IncludeDir["VulkanSDK"] = "C:/VulkanSDK/1.3.275.0/Include"
IncludeDir["stb_image"] = "%{wks.location}/SpiecsEngine/vendor/stb_image"
IncludeDir["glm"] = "%{wks.location}/SpiecsEngine/vendor/glm"
IncludeDir["ImGui"] = "%{wks.location}/SpiecsEngine/vendor/imgui"
IncludeDir["entt"] = "%{wks.location}/SpiecsEngine/vendor/entt/include"

LibraryDir = {}
LibraryDir["VulkanSDK"] = "C:/VulkanSDK/1.3.275.0/Lib"

Library = {}
Library["VulkanSDK"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"