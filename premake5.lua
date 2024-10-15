-- @file workspace Premake.
-- @brief Defines all Solutions Building.
-- @author The Cherno & Spices.

workspace "SpicesEngine"
	architecture "x64"              -- Platform x64
	startproject "SpicesGame"       -- Start with SpicesGame

	-- Configuration: Debug and Release.
	configurations
	{
		"Debug",
		"Release",
	}

-- Building Output Folder.
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Libraries of this Solution.
include "lib.lua"

-- Project: SpicesEngine.
include "SpicesEngine"

-- Project: SpicesGame.
include "SpicesGame"

-- Project: SpicesTest.
include "SpicesTest"

-- Samples Project.
group "Samples"

group "Renderer"
	-- Samples Project WhittedRT.
	include "Samples/Renderer/WhittedRT"
group ""

group ""

-- Project Dependencies.
group "Dependencies"
	include "SpicesEngine/vendor/GLFW/"           -- Dependency GLFW.
	include "SpicesEngine/vendor/imgui"           -- Dependency imgui.
	include "SpicesEngine/vendor/yaml-cpp"        -- Dependency yaml-cpp.
	include "SpicesEngine/vendor/implot"          -- Dependency implot.
	include "SpicesEngine/vendor/GKlib"           -- Dependency GKlib.
	include "SpicesEngine/vendor/METIS"           -- Dependency METIS.
	include "SpicesEngine/vendor/KTX-Software"    -- Dependency ktx.
	include "SpicesEngine/vendor/shaderc"         -- Dependency shaderc.
	include "SpicesTest/vendor/googletest"        -- Dependency googletest.
group ""