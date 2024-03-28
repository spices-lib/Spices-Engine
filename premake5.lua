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