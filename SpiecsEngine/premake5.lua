project "SpiecsEngine"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"

	targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pchheader.h"
	pchsource "src/pchheader.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",

		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
	}

	defines
	{
		'SPIECS_ENGINE_ASSETS_PATH=std::string("%{wks.location}/SpiecsEngine/assets/")'
	}

	includedirs
	{
		"src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.VulkanSDK}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
	}

	links
	{
		"GLFW",
		"%{Library.VulkanSDK}",
		"imgui"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
		}

		links
		{
		}

	filter "configurations:Debug"
		defines "FL_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "FL_RELEASE"
		runtime "Release"
		optimize "On"