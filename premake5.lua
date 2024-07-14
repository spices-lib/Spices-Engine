workspace "SpicesEngine"
	architecture "x64"
	startproject "SpicesGame"

	configurations
	{
		"Debug",
		"Release",
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "lib.lua"

include "SpicesEngine"
include "SpicesGame"
include "SpicesTest"

group "Dependencies"
	include "SpicesEngine/vendor/GLFW/"
	include "SpicesEngine/vendor/imgui"
	include "SpicesEngine/vendor/yaml-cpp"
	include "SpicesEngine/vendor/implot"
	include "SpicesTest/vendor/googletest"
group ""