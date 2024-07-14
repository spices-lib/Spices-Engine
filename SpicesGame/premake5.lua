project "SpicesGame"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"

	targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
	}

	defines
	{
		'SPICES_GAME_ASSETS_PATH=std::string("%{wks.location}/SpicesGame/assets/")',
		"RENDERAPI_VULKAN",
	}

	includedirs
	{
		"%{wks.location}/SpicesEngine/src",
		"src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.VulkanSDK}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.tinyobjloader}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.ImPlot}",                -- Library: ImPlot Source Folder.
		"%{IncludeDir.tracy}",
		"%{IncludeDir.IconFontCppHeaders}",    -- Library: IconFontCppHeaders Source Folder.
	}

	links
	{
		"SpicesEngine"
	}

	filter "system:windows"
		systemversion "latest"
		editAndContinue "Off"

	filter "configurations:Debug"
		defines
		{
			"SPICES_DEBUG",
			--"TRACY_ENABLE"
		}
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines
		{
			"SPICES_RELEASE",
		}
		runtime "Release"
		optimize "On"