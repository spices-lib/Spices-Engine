-- @file SpicesTest Premake.
-- @brief Defines details of the UintTest Solution Building.
-- @author The Cherno & Spices.

project "SpicesTest"
	kind "ConsoleApp"           -- Use exe.
	language "C++"			    -- Use C++.
	cppdialect "C++17"		    -- Use C++17.
	staticruntime "On"		    -- Use Runtime Linrary: MTD.

	-- Building Output Folder.
	targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")

	-- Building Object Folder.
	objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	-- The Solution Files.
	files
	{
		-- Game Source Files.
		"src/**.h",
		"src/**.cpp",
	}

	-- Macros Definitions
	defines
	{
		-- Define Engine Use Vulkan API for Rendering, though we may support multipile Rendering API.
		"RENDERAPI_VULKAN"
	}

	-- The Solution Additional Include Folder.
	includedirs
	{
		"%{wks.location}/SpicesEngine/src",    -- Engine Source Folder.
		"%{IncludeDir.googletest}",			   -- Library: googletest Source Folder.
		"%{IncludeDir.googlemock}",			   -- Library: googlemock Source Folder.
		"src",                                 -- Engine Source Folder.
		"%{IncludeDir.GLFW}",                  -- Library: GLFW Source Folder.
		"%{IncludeDir.VulkanSDK}",             -- Library: VulkanSDK Source Folder.
		"%{IncludeDir.stb_image}",             -- Library: stb_image Source Folder.
		"%{IncludeDir.glm}",                   -- Library: glm Source Folder.
		"%{IncludeDir.ImGui}",                 -- Library: ImGui Source Folder.
		"%{IncludeDir.entt}",                  -- Library: entt Source Folder.
		"%{IncludeDir.tinyobjloader}",         -- Library: tinyobjloader Source Folder.
		"%{IncludeDir.yaml_cpp}",              -- Library: yaml_cpp Source Folder.
		"%{IncludeDir.rapidyaml}",             -- Library: rapidyaml Source Folder.
		"%{IncludeDir.ImPlot}",                -- Library: ImPlot Source Folder.
		"%{IncludeDir.NvAftermath}",           -- Library: NvAftermath Source Folder.
		"%{IncludeDir.NvPerf}",                -- Library: NvPerf Source Folder.
		"%{IncludeDir.NvPerfUtility}",         -- Library: NvPerfUtility Source Folder.
		"%{IncludeDir.NVTX}",                  -- Library: NVTX Source Folder.
		"%{IncludeDir.spdlog}",                -- Library: spdlog Source Folder.
		"%{IncludeDir.ImGuizmo}",              -- Library: ImGuizmo Source Folder.
		"%{IncludeDir.tracy}",                 -- Library: tracy Source Folder.
		"%{IncludeDir.IconFontCppHeaders}",    -- Library: IconFontCppHeaders Source Folder.
		"%{IncludeDir.HoudiniEngine}",         -- Library: HoudiniEngine Dependency Folder.
		"%{IncludeDir.taskflow}",              -- Library: taskflow Source Folder.
		"%{IncludeDir.meshoptimizer}",         -- Library: meshoptimizer Source Folder.
		"%{IncludeDir.VulkanMemoryAllocator}", -- Library: VulkanMemoryAllocator Header Folder.
		"%{IncludeDir.METIS}/include",         -- Library: METIS Header Folder.
		"%{IncludeDir.ktx}/include",           -- Library: ktx Header Folder.
	}

	-- In Visual Studio, it only works when generated a new solution, remember update solution will not works.
    -- In Rider, it will not work, needs to add environment variables manually in project configurations setting.
	debugenvs 
	{
		-- Houdini dll Path.
		-- NvAftermath dll Path.
		-- NvPerf dll Path.
		"PATH=%{LibraryDir.HoudiniEnginedll};%{LibraryDir.NvAftermath};%{LibraryDir.NvPerf}", 
	}

	-- The Solution Dependency
	links
	{
		"SpicesEngine",                        -- Dependency: SpicesEngine
		"googlemock",                          -- Dependency: googlemock
	}

	-- Platform: Windows
	filter "system:windows"
		systemversion "latest"                 -- Use Lastest WindowSDK
		editAndContinue "Off"				   -- Use DebugInfoFormat: Zi (Program Database).
		
	-- Configuration: Debug
	filter "configurations:Debug"

		-- Debug Specific Solution Macro Definitions.
		defines
		{
			"SPICES_DEBUG",                    -- Debug Symbol.
			"TRACY_ENABLE"				       -- tracy Feature Enable.
		}

		runtime "Debug"
		symbols "On"
		
	-- Configuration: Release.
	filter "configurations:Release"

		-- Release Specific Solution Macro Definitions.
		defines
		{
			"SPICES_RELEASE",                  -- Release Symbol.
		}

		runtime "Release"
		optimize "On"
		