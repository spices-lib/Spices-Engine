project "SpiecsEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"

	targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "Pchheader.h"
	pchsource "src/Pchheader.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",

		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",

		"vendor/tinyobjloader/**.cpp",
		"vendor/tinyobjloader/**.h",

		"vendor/ImGuizmo/ImGuizmo.cpp",
		"vendor/ImGuizmo/ImGuizmo.h"
	}

	defines
	{
		'SPIECS_ENGINE_ASSETS_PATH=std::string("%{wks.location}/SpiecsEngine/assets/")',
		'SPIECS_lOG_PATH=std::string("%{wks.location}/bin/Log/Log.log")',
		"RENDERAPI_VULKAN"
	}

	includedirs
	{
		"src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.VulkanSDK}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.tinyobjloader}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.rapidyaml}",
		"%{IncludeDir.ImPlot}",
		"%{IncludeDir.NvPerf}",
		"%{IncludeDir.NvPerfUtility}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.ImGuizmo}",
	}

	links
	{
		"GLFW",
		"%{Library.VulkanSDK}",
		"imgui",
		"yaml-cpp",
		"implot",
		"%{Library.NvPerfUtility}",
	}

	filter "files:vendor/stb_image/**.cpp"
	flags { "NoPCH" }

	filter "files:vendor/tinyobjloader/**.cpp"
	flags { "NoPCH" }

	filter "files:vendor/ImGuizmo/**.cpp"
	flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

		defines
		{
		}

		links
		{
		}

	filter "configurations:Debug"
		defines "SPIECS_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "SPIECS_RELEASE"
		runtime "Release"
		optimize "On"