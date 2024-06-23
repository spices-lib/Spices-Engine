project "SpiecsTest"
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
		"RENDERAPI_VULKAN"
	}

	includedirs
	{
		"src",
		"%{wks.location}/SpiecsEngine/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.VulkanSDK}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.tinyobjloader}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.googletest}",
		"%{IncludeDir.googlemock}",
		"%{IncludeDir.tracy}",
		"%{IncludeDir.IconFontCppHeaders}",    -- Library: IconFontCppHeaders Source Folder.
		"%{IncludeDir.yaml_cpp}",              -- Library: yaml_cpp Source Folder.
		"%{IncludeDir.rapidyaml}",             -- Library: rapidyaml Source Folder.
		"%{IncludeDir.ImPlot}",                -- Library: ImPlot Source Folder.
		"%{IncludeDir.NvPerf}",                -- Library: NvPerf Source Folder.
		"%{IncludeDir.NvPerfUtility}",         -- Library: NvPerfUtility Source Folder.
		"%{IncludeDir.ImGuizmo}",              -- Library: ImGuizmo Source Folder.
	}

	links
	{
		"SpiecsEngine",
		"googlemock"
	}

	filter "system:windows"
		systemversion "latest"
		editAndContinue "Off"
		

	filter "configurations:Debug"
		defines
		{
			"SPIECS_DEBUG",
			--"TRACY_ENABLE"
		}
		runtime "Debug"
		symbols "On"
		

	filter "configurations:Release"
		defines
		{
			"SPIECS_RELEASE",
		}
		runtime "Release"
		optimize "On"
		