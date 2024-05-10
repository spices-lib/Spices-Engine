workspace "SpiecsEngine"
	architecture "x64"
	startproject "SpiecsGame"

	configurations
	{
		"Debug",
		"Release",
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "lib.lua"

include "SpiecsEngine"
include "SpiecsGame"
include "SpiecsTest"

group "Dependencies"
	include "SpiecsEngine/vendor/GLFW/"
	include "SpiecsEngine/vendor/imgui"
	include "SpiecsEngine/vendor/yaml-cpp"
	include "SpiecsEngine/vendor/implot"
	include "SpiecsTest/vendor/googletest"
group ""