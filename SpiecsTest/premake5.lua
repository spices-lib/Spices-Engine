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
	}

	includedirs
	{
		"%{wks.location}/SpiecsEngine/src",
		"src",
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
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"