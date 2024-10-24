-- @file SpicesEngine Premake.
-- @brief Defines details of the Engine Solution Building.
-- @author The Cherno & Spices.

project "SpicesEngine"
	kind "StaticLib"          -- Use Lib as Dependency.
	language "C++"            -- Use C++.
	cppdialect "C++17"        -- Use C++17.
	staticruntime "On"        -- Use Runtime Linrary: MTD.

	-- Building Output Folder.
	targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")

	-- Building Object Folder.
	objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "Pchheader.h"          -- PreCompiler Header File.
	pchsource "src/Pchheader.cpp"    -- PreCompiler Cpp File.

	-- The Solution Files.
	files
	{
		-- Engine Source Files.
		"src/**.h",
		"src/**.hpp",
		"src/**.cpp",

		-- Engine Shader Headers.
		"assets/Shaders/src/Header/**.h",

		-- Library: std_image Files.
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		
		-- Library: glm Files.
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",

		-- Library: tinyobjloader Files.
		"vendor/tinyobjloader/**.cpp",
		"vendor/tinyobjloader/**.h",

		-- Library: ImGuizmo Files.
		"vendor/ImGuizmo/ImGuizmo.cpp",
		"vendor/ImGuizmo/ImGuizmo.h",

		-- Library: tracy Files.
		"vendor/tracy/public/TracyClient.cpp",
		"vendor/tracy/public/tracy/Tracy.hpp",

		-- Library: IconFontCppHeaders Files.
		"vendor/IconFontCppHeaders/**.h",

		-- Library: taskflow Files.
		"vendor/taskflow/taskflow/taskflow.hpp",

		-- Library: meshoptimizer Files.
		"vendor/meshoptimizer/src/**.h",
		"vendor/meshoptimizer/src/**.cpp",
	}

	-- Macros Definitions
	defines
	{
		-- Define Engine Assets Folder.
		'SPICES_ENGINE_ASSETS_PATH=std::string("' .. path.translate(os.getcwd(), "/") .. '/assets/")',

		-- Define Engine Logs Folder.
		'SPICES_CONSOLElOGFILE_PATH=std::string("saved/ConsoleLog/")',

		-- Define Nsight Perf Config Folder.
		'SPICES_PERFCONFIG_PATH=std::string("' .. path.translate(os.getcwd(), "/") .. '/vendor/NvPerfUtility/HudConfigurations/pub/")',

		-- Define Engine GPUCrashReport Folder.
		'SPICES_GPUCRASHREPORT_PATH=std::string("saved/GPUCrashReport/")',

		-- Define Engine GPUProfilerReport Folder.
		'SPICES_GPUPROFILEREPORT_PATH=std::string("saved/GPUProfilerReport/")',

		-- Define Engine GPUProfilerOneShot Folder.
		'SPICES_GPUPROFILEONESHOT_PATH=std::string("saved/GPUProfilerOneShot/")',

		-- Define Engine Extent Processes Folder.
		'SPICES_EXTENT_PROCESS_PATH=std::string("' .. path.translate(os.getcwd(), "/") .. '/../vendor/")',

		-- Define Engine Use Vulkan API for Rendering, though we may support multipile Rendering API.
		"RENDERAPI_VULKAN"
	}

	-- The Solution Additional Include Folder.
	includedirs
	{
		"src",                                                -- Engine Source Folder.
		"%{IncludeDir.GLFW}",                                 -- Library: GLFW Source Folder.
		"%{IncludeDir.VulkanSDK}",                            -- Library: VulkanSDK Source Folder.
		"%{IncludeDir.stb_image}",                            -- Library: stb_image Source Folder.
		"%{IncludeDir.glm}",                                  -- Library: glm Source Folder.
		"%{IncludeDir.ImGui}",                                -- Library: ImGui Source Folder.
		"%{IncludeDir.entt}",                                 -- Library: entt Source Folder.
		"%{IncludeDir.tinyobjloader}",                        -- Library: tinyobjloader Source Folder.
		"%{IncludeDir.yaml_cpp}",                             -- Library: yaml_cpp Source Folder.
		"%{IncludeDir.rapidyaml}",                            -- Library: rapidyaml Source Folder.
		"%{IncludeDir.ImPlot}",                               -- Library: ImPlot Source Folder.
		"%{IncludeDir.NvAftermath}",                          -- Library: NvAftermath Source Folder.
		"%{IncludeDir.NvPerf}",                               -- Library: NvPerf Source Folder.
		"%{IncludeDir.NvPerfUtility}",                        -- Library: NvPerfUtility Source Folder.
		"%{IncludeDir.NVTX}",                                 -- Library: NVTX Source Folder.
		"%{IncludeDir.spdlog}",                               -- Library: spdlog Source Folder.
		"%{IncludeDir.ImGuizmo}",                             -- Library: ImGuizmo Source Folder.
		"%{IncludeDir.tracy}",                                -- Library: tracy Source Folder.
		"%{IncludeDir.IconFontCppHeaders}",                   -- Library: IconFontCppHeaders Source Folder.
		"%{IncludeDir.HoudiniEngine}",                        -- Library: HoudiniEngine Dependency Folder.
		"%{IncludeDir.taskflow}",                             -- Library: taskflow Source Folder.
		"%{IncludeDir.meshoptimizer}",                        -- Library: meshoptimizer Source Folder.
		"%{IncludeDir.VulkanMemoryAllocator}",                -- Library: VulkanMemoryAllocator Header Folder.
		"%{IncludeDir.METIS}/include",                        -- Library: METIS Header Folder.
		"%{IncludeDir.ktx}/include",                          -- Library: ktx Header Folder.
		"%{IncludeDir.shaderc}",                              -- Library: shaderc Folder.
		"%{IncludeDir.shaderc}/libshaderc/include",           -- Library: libshaderc Folder.
		"%{IncludeDir.shaderc}/libshaderc_util/include",      -- Library: libshaderc_util Folder.
		"%{IncludeDir.glslang}",                              -- Library: glslang Folder.
	}

	-- The Solution Dependency
	links
	{
		"GLFW",                               -- Dependency: GLFW
		"%{Library.VulkanSDK}",               -- Dependency: VulkanSDK
		"imgui",                              -- Dependency: imgui
		"yaml-cpp",                           -- Dependency: yaml-cpp
		"implot",                             -- Dependency: implot
		"%{Library.NvAftermath}",             -- Dependency: NvAftermath
		"%{Library.NvPerf}",                  -- Dependency: NvPerf
		"%{Library.HoudiniEnginelib}",        -- Dependency: HoudiniEngine
		"METIS",                              -- Dependency: METIS
		"ktx",                                -- Dependency: ktx
		"shaderc",                            -- Dependency: shaderc
	}

	-- Library: std_image is included this solution, do not use PreCompiler Header.
	filter "files:vendor/stb_image/**.cpp"
	flags { "NoPCH" }
	
	-- Library: tinyobjloader is included this solution, do not use PreCompiler Header.
	filter "files:vendor/tinyobjloader/**.cpp"
	flags { "NoPCH" }
	
	-- Library: ImGuizmo is included this solution, do not use PreCompiler Header.
	filter "files:vendor/ImGuizmo/**.cpp"
	flags { "NoPCH" }
	
	-- Library: tracy is included this solution, do not use PreCompiler Header.
	filter "files:vendor/tracy/public/**.cpp"
	flags { "NoPCH" }

	-- Library: meshoptimizer is included this solution, do not use PreCompiler Header.
	filter "files:vendor/meshoptimizer/src/**.cpp"
	flags { "NoPCH" }

	-- Platform: Windows
	filter "system:windows"
		systemversion   "latest"              -- Use Lastest WindowSDK

											  -- tracy use __FILE__ in constexpr, but MSVC do not support it by default.
											  -- There are two way to handle with that:
											  -- 1.Use a complex format of __LINE__:
											  --   #define _DBJ_CONCATENATE_(a, b) a ## b
											  --   #define _DBJ_CONCATENATE(a, b)  _DBJ_CONCATENATE_(a, b)
                                              --   #define CONSTEXPR_LINE long(_DBJ_CONCATENATE(__LINE__,U)) 
		editAndContinue "Off"				  -- 2.Use DebugInfoFormat: Zi (Program Database).

		-- Windows Specific Solution Macro Definitions.
		defines
		{}

		-- Windows Specific Solution Dependency.
		links
		{}

	-- Configuration: Debug
	filter "configurations:Debug"

		-- Debug Specific Solution Macro Definitions.
		defines 
		{
			"SPICES_DEBUG",                  -- Debug Symbol.
			"TRACY_ENABLE",                  -- tracy Feature Enable.
			"TRACY_ON_DEMAND",               -- Used if want profile on demand.
		}

		links
		{
			"%{Library.shaderc_debug}",                 -- Dependency: shaderc_debug
			"%{Library.shaderc_utils_debug}",           -- Dependency: shaderc_utils_debug
		}

		runtime "Debug"
		symbols "On"

	-- Configuration: Release.
	filter "configurations:Release"

		-- Release Specific Solution Macro Definitions.
		defines 
		{
			"SPICES_RELEASE",                -- Release Symbol.
			"TRACY_ENABLE",                  -- tracy Feature Enable.
			"TRACY_ON_DEMAND",               -- Used if want profile on demand.
		}

		links
		{
			"%{Library.shaderc_release}",                 -- Dependency: shaderc_release
			"%{Library.shaderc_utils_release}",           -- Dependency: shaderc_utils_release
		}

		runtime "Release"
		optimize "On"