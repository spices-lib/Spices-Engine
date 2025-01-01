Building Structure
============
# Build Tool
***
* Tool Locating in
  ~~~
  ./vendor/premake/premake5.exe
  ~~~
# Build Scripts
***
* Generate Solution: 
  ~~~
  ./Startup.bat
  ~~~
* Compile with Debug: 
  ~~~
  ./scripts/Build_Debug_x64.bat
  ~~~
* Compile with Release:
  ~~~
  ./scripts/Build_Release_x64.bat
  ~~~

# Solution Build Config
***
* Solution Name: **SpicesEngine**
* Config Locating in
  ~~~
  ./premake5.lua
  ~~~
* architecture: **x64**
* startproject: **SpicesGame**
* configurations: **Debug**, **Release**
* Projects:
  * **SpicesEngine**
  * **SpicesGame**
  * **SpicesTest**
* Dependencies: 
  * **GLFW**
  * **imgui**
  * **yaml-cpp**
  * **implot**
  * **GKlib**
  * **METIS**
  * **KTX-Software**
  * **shaderc**
  * **googletest**

# Project Build Config
***
## SpicesEngine
* Project Name: **SpicesEngine**
* Config Locating in
  ~~~
  ./SpicesEngine/premake5.lua
  ~~~
* kind: **StaticLib**
* language: **C++**
* cppdialect: **C++17**
* staticruntime: **On**
* flags: **MultiProcessorCompile**
* pchheader:
  ~~~
  Pchheader.h
  ~~~
* pchsource:
  ~~~
  ./SpicesEngine/src/Pchsource.cpp
  ~~~
* links: 
  * GLFW
  * VulkanSDK
  * imgui
  * yaml-cpp
  * implot
  * NvAftermath
  * NvPerf
  * HoudiniEnginelib
  * METIS
  * ktx
  * shaderc
* systemversion: **latest**
* editAndContinue: **Off**
## SpicesGame
* Project Name: **SpicesGame**
* Config Locating in 
  ~~~
  ./SpicesGame/premake5.lua
  ~~~
* kind: **ConsoleApp**
* language: **C++**
* cppdialect: **C++17**
* staticruntime: **On**
* flags: **MultiProcessorCompile**
* debugenvs: 
  ~~~
  PATH=LibraryDir.HoudiniEnginedll;LibraryDir.NvAftermath;LibraryDir.NvPerf;LibraryDir.VulkanSDKDLL
  ~~~
* links: 
  * SpicesEngine
## SpicesTest
* Project Name: **SpicesTest**
* Config Locating in
  ~~~
  ./SpicesTest/premake5.lua
  ~~~
* kind: **ConsoleApp**
* language: **C++**
* cppdialect: **C++17**
* staticruntime: **On**
* flags: **MultiProcessorCompile**
* debugenvs:
  ~~~
  PATH=LibraryDir.HoudiniEnginedll;LibraryDir.NvAftermath;LibraryDir.NvPerf;LibraryDir.VulkanSDKDLL
  ~~~
* links: 
  * SpicesEngine
  * googlemock

## Dependency Build Config (Use imgui as a sample)
* Project Name: **imgui**
* Config Locating in
  ~~~
  ./SpicesEngine/vendor/imgui/premake5.lua
  ~~~
* kind: **StaticLib**
* language: **C++**
* cppdialect: **C++17**
* staticruntime: **On**

## Libraries
* Locating in ./lib.lua
* IncludeDirectories:
  * GLFW
  * VulkanSDK
  * stb_image
  * glm
  * ImGui
  * entt
  * tinyobjloader
  * yaml_cpp
  * rapidyaml
  * ImPlot
  * NvAftermath
  * NvPerf
  * NvPerfUtility
  * NVTX
  * spdlog
  * ImGuizmo
  * tracy
  * IconFontCppHeaders
  * HoudiniEngine
  * taskflow
  * meshoptimizer
  * VulkanMemoryAllocator
  * GKlib
  * METIS
  * ktx
  * shaderc
  * glslang
  * nlohmann
  * googletest
  * googlemock
* Libraries:
  * VulkanSDK
  * NvAftermath
  * NvPerf
  * HoudiniEnginelib
  * shaderc_debug
  * shaderc_utils_debug
  * shaderc_release
  * shaderc_utils_release

# How to expand
***
## Add New Project
1. Make a empty folder.
2. Prepare a premake5.lua file and make you own project config.
3. include this premake5.lua file in the Solution Build Config.

## Add New Dependency
1. clone the dependency to the vendor folder.
2. Prepare a premake5.lua file and make dependency config.
3. include this premake5.lua file in the Solution Build Config.
4. links the dependency in the Project Build Config.

## Add New Library
1. clone the library to the vendor folder.
2. include the library in the lib.lua file.
3. add to the includedirs in the Project Build Config.

# TODO
***
1. Compile from source.
2. Add emscripten support.
3. Expand the vsTool.