Introduction {#mainpage}
============
# Solution: Spices Engine
***
* **Start Date:**  March 27, 2024
* **Project Purpose:** The primary objective of the Spices Engine project 
is to validate and deepen my understanding and proficiency 
in several advanced fields of graphics programming and engine development, 
specifically Vulkan, engine development principles, C++, rendering pipelines, 
ray tracing, and shader development. Prior to writing the first line of code, 
I underwent a rigorous six-month period of study to ensure I had the necessary 
knowledge to successfully complete this project.
*  **Device:** NVIDIA GeForce RTX 4060

# How to Build
***
## Visual Studio 2022
1. Clone the solution with this code in cmd to the folder in your computer.<br/>
   ~~~
   git clone --recursive https://github.com/spices-lib/Spices-Engine.git
   ~~~
2. Locating to the root folder, and run the script **Startup.bat**, it will read the premake
scripts to build a visual studio project.
## JetBrains Rider
1. Clone the solution with this code in cmd to the folder in your computer.<br/>
   ~~~
   git clone --recursive https://github.com/spices-lib/Spices-Engine.git
   ~~~
2. Locating to the root folder, and run the script **Startup.bat**, it will read the premake
   scripts to build a visual studio project.
3. Run **SpicesEngine.sln** with JetBrains Rider.
4. Copy the environment variables in Visual Studio 2022 to Rider compile settings.
## Bat
1. Clone the solution with this code in cmd to the folder in your computer.<br/>
   ~~~
   git clone --recursive https://github.com/spices-lib/Spices-Engine.git
   ~~~
2. Locating to the root folder, and run the script **Startup.bat**, it will read the premake
   scripts to build a visual studio project.
3. Locating to the folder **./Scripts/**, and run the script **Build_Debug_x64.bat** or **Build_Release_x64.bat**.

# Project: SpicesEngine
***
*  Kind: **Static Library (lib)**
*  language: **C++17**
*  targetdir: **./bin/.../SpicesEngine/SpicesEngine.lib**
*  links:
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

## Features
1. **[MemoryPool](item/Memory.md)**
2. **Reflect**
3. **[MultipleThreading](item/MultipleThreading.md)**
4. **[Debugger](item/Debugger.md)**
5. **Houdini LiveLink**
6. **[Vulkan Render Backend](item/Vulkan Render Backend.md)**
7. **Path Tracing**
8. **Nanite**
9. **DeviceGeneratedCommands**
10. **Physically Based Rendering(PBR)**
11. **Entity Component System(ECS)**
12. **Log System**
13. **Git CI**
14. **User Interface**

## Todo List
1. **Bi-Directional Path Tracing**
2. **Houdini LiveLink**
3. **GPU Work Graph**
4. **Emscripten Compile**
5. **Garbage Collection**
6. **USD Support**

