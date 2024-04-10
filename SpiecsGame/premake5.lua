project "SpiecsGame"
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
		'SPIECS_GAME_ASSETS_PATH=std::string("%{wks.location}/SpiecsGame/assets/")',
		"RENDERAPI_VULKAN"
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
	}

	links
	{
		"SpiecsEngine"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "SPIECS_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "SPIECS_RELEASE"
		runtime "Release"
		optimize "On"