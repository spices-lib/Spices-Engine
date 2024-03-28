workspace "SpiecsEngine"
	architecture "x64"
	startproject "SpiecsEngine"

	configurations
	{
		"Debug",
		"Release",
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "lib.lua"

include "SpiecsEngine"

group "Dependencies"
	include "SpiecsEngine/vendor/GLFW/"
	include "SpiecsEngine/vendor/imgui"
group ""