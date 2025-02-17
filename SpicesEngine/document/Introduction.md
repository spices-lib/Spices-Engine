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
* **ScreenShots:**
  * GltfPack: BistroExterior, Renderer: RayTracingRenderer <br/>
    <img alt="BistroExterior" height="1087" src="BistroExterior.png" width="1800"/>
  * Virtual Geometry<br/>
    <img alt="Virtual Geometry" height="1087" src="Virtual Geometry.png" width="1800"/>

# How to Build
***
## Visual Studio 2022
1. Clone the solution with this code in cmd to the folder in your computer.<br/>
   ~~~
   git clone --recursive https://github.com/spices-lib/Spices-Engine.git
   ~~~
2. Locating to the root folder, and run the script **Startup.bat**, it will read the premake
3. Run **SpicesEngine.sln** with Visual Studio 2022.
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
   * [GLFW @ 9bed794](https://github.com/TheCherno/glfw/tree/9bed794ab7c1b961aaca259403695bbd3870d3b3)
   * [VulkanSDK 1.3.296](https://vulkan.lunarg.com/)
   * [imgui @ bdb8cdf](https://github.com/spices-lib/imgui/tree/bdb8cdf061e55ab159fbebf8285d44b0b0b73af2)
   * [yaml-cpp @ ac5a1ca](https://github.com/spices-lib/yaml-cpp/tree/ac5a1ca552659ea1737db862f4037226dc11b7d4)
   * [implot @ b6fc717](https://github.com/spices-lib/implot/tree/b6fc71745b8471e2fda6f17dd40586145269971a)
   * NvAftermath
   * NvPerf
   * NvPerfUtility
   * HoudiniEngine 7.0
   * [shaderc @ a0aafb9](https://github.com/spices-lib/Spices-Engine/tree/Master/SpicesEngine/vendor)
   * [IconFontCppHeaders @f30b1e7](https://github.com/juliettef/IconFontCppHeaders/tree/f30b1e73b2d71eb331d77619c3f1de34199afc38)
   * [ImGuizmo @ba662b1](https://github.com/CedricGuillemet/ImGuizmo/tree/ba662b119d64f9ab700bb2cd7b2781f9044f5565)
   * [glm @ee7e155](https://github.com/spices-lib/glm/tree/ee7e155649041fd83920fdd045d28d0d5310a70d)
   * [spdlog @d276069](https://github.com/gabime/spdlog/tree/d276069a6e916b1e1fd45885b15b72bd8ee000a7)
   * [tracy @521e371](https://github.com/wolfpld/tracy/tree/521e371c60e37b47ffb8b0330d310af7061e0af8)
   * [VulkanMemoryAllocator @871913d](https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator/tree/871913da6a4b132b567d7b65c509600363c0041e)
   * [meshoptimizer @30fe9fe](https://github.com/zeux/meshoptimizer/tree/30fe9fed38f963fb8bb9a255e8eb27fafc5b4746)
   * [taskflow @9823d00](https://github.com/taskflow/taskflow/tree/9823d00e119b2205943de737b9ca5f93c5cc26bd)
   * [nlohmann @ 1825117](https://github.com/nlohmann/json/tree/1825117e630636f5c7c71a852855d5249cf2b7ff)
   * rapidyaml
   * [GKlib @ 14898ff](https://github.com/spices-lib/GKlib/tree/14898ff4d035090f4cae90b217e1b122e67e2031)
   * [KTX-Software @ 921e798](https://github.com/spices-lib/KTX-Software/tree/921e798c84ffa2df6fbdece30518618d2eb194b7)
   * [METIS @ 8630318](https://github.com/spices-lib/METIS/tree/8630318d27eb4aef370b33a806107164dcd20908)
   * [NVTX @ 09e0d23](https://github.com/NVIDIA/NVTX/tree/09e0d23a61ae86cc381eef85d012afc3a2e6eeea)
   * entt
   * stb_image
   * tinyobjloader
   * [wepoll @ 0598a79](https://github.com/piscisaureus/wepoll/tree/0598a791bf9cbbf480793d778930fc635b044980)

## Features
1. **[Premake Build](item/Premake Build.md)**
2. **[MemoryPool](item/Memory.md)**
3. **[Reflection](item/Reflection.md)**
4. **[MultipleThreading](item/MultipleThreading.md)**
5. **[Debugger](item/Debugger.md)**
6. **Houdini LiveLink**
7. **[Vulkan Render Backend](item/Vulkan Render Backend.md)**
8. **[Rasterization](item/Rasterization.md)**
9. **[Ray Tracing](item/Ray Tracing.md)**
10. **[Virtual Geometry](item/Virtual Geometry.md)**
11. **[DeviceGeneratedCommands](item/DeviceGeneratedCommands.md)**
12. **[Physically Based Rendering(PBR)](item/Physically Based Rendering.md)**
13. **Entity Component System(ECS)**
14. **[Log](item/Log.md)**
15. **[Unit Test](item/Unit Test.md)**
16. **[CI CD](item/CI CD.md)**
17. **[User Interface](item/User Interface.md)**

## Todo List ( 44 / 209 )
| Index |      State       | Description 
|:-----:|:----------------:|:---:
|   1   |     Finished     | Vulkan context wrapper
|   2   |     Finished     | OBJ mesh load
|   3   |     Finished     | BasePass Renderer
|   4   |     Finished     | ECS
|   5   |     Finished     | DescriptorSet Combine Design
|   6   |     Finished     | Vulkan Memory Allocator
|   7   |     Finished     | Yaml Material Loader
|   8   |     Finished     | MeshPack Loader
|   9   |     Finished     | Texture Array
|  10   |     Finished     | TimeStamp
|  11   |     Finished     | Event Dispatcher
|  12   |     Finished     | Camera Controller
|  13   |     Finished     | Resize
|  14   |     Finished     | Input Attachments
|  15   |     Finished     | subPass
|  16   |     Finished     | Docking UI
|  17   |     Finished     | Implot
|  18   |     Finished     | console log
|  19   |     Finished     | Deferred Rendering
|  20   |     Finished     | GBuffer Visualizer
|  21   |     Finished     | Entity Picker
|  22   |     Finished     | Sobel outline
|  23   |     Finished     | Sprite Renderer
|  24   |     Finished     | google test
|  25   |     Finished     | linked_unordered_map
|  26   |     Finished     | Renderer architecture refactoring
|  27   |     Finished     | RendererPassBuilder
|  28   |     Finished     | DescriptorSetBuilder
|  29   |     Finished     | PipelineBuilder
|  30   |     Finished     | RenderBehaveBuilder
|  31   |     Finished     | RenderPassBuilder
|  32   |     Finished     | DGCLayoutBuilder
|  33   |     Finished     | RenderGraphBuilder
|  34   |     Finished     | SceneCompose Renderer
|  35   |     Finished     | Split material descriptor set
|  36   |     Finished     | gizmo
|  37   |     Finished     | tracy for game analysis
|  38   |     Finished     | log to files
|  39   |     Finished     | main menu slate
|  40   |     Finished     | stage slate
|  41   |     Finished     | info bar slate
|  42   |     Finished     | operator slate
|  43   |     Finished     | material panel slate
|  44   |     Finished     | property slate
|  45   |     Finished     | tracy memory
|  46   |     Finished     | switch tracy to in need mode
|  47   |     Finished     | font icon
|  48   |     Finished     | debugutils for vulkan objects
|  49   |     Finished     | pbr material
|  50   |     Finished     | geometry shader
|  51   |     Finished     | renderer resource create
|  52   |     Finished     | light matrix
|  53   |     Finished     | reverse y axis
|  54   |     Finished     | buffer device address
|  55   |     Finished     | RayTracing pipeline
|  56   |     Finished     | blas build
|  57   |     Finished     | tlas build
|  58   |     Finished     | ray tracing shader
|  59   |     Finished     | RayTracingRenderBehaveBuilder
|  60   |     Finished     | Shader Binding Table
|  61   |     Finished     | BindLess Texture
|  62   |     Finished     | VulkanMemoryAllocator
|  63   |     Finished     | RayTracing Scratch Buffer specific vma flag
|  64   |     Finished     | RayTracing Renderer
|  65   |     Finished     | RayTracing Compose Renderer
|  66   |                  | Bi-Directional Path Tracing
|  67   |                  | PBRT
|  68   |     Finished     | Storage Image
|  69   |     Finished     | Renderer specific mesh added event
|  70   |     Finished     | Shader include files
|  71   |     Finished     | Head file reuse both in shader and cpp
|  72   |     Finished     | BindLess Buffer access in shader
|  73   |     Finished     | Storage Buffer
|  74   |     Finished     | Discontinuous DescriptorSet index
|  75   |     Finished     | Shader Debug log
|  76   |     Finished     | Component property slate draw
|  77   |     Finished     | USD Composer Slate style
|  78   |                  | USD
|  79   |                  | Houdini LiveLink
|  80   |     Finished     | World mark handle script
|  81   |     Finished     | Compute Shader
|  82   |     Finished     | Mesh Shader pipeline
|  83   |     Finished     | Build MeshPack meshlet
|  84   |     Finished     | Task Shader normal & viewFrustum cull
|  85   |                  | MultipleThreading
|  86   |     Finished     | ThreadPool
|  87   |     Finished     | RHI MultipleThreading
|  88   |     Finished     | Secondary CommandBuffer
|  89   |     Finished     | Device Generated Commands
|  90   |     Finished     | Abstract Vk Function pointer
|  91   |     Finished     | Device Generated Commands
|  92   |     Finished     | DGC pipeline
|  93   |     Finished     | Compute Shader async queue
|  94   |     Finished     | meshoptimizer
|  95   |     Finished     | reverse depth buffer
|  96   |     Finished     | shader subgroup
|  97   |     Finished     | Mesh Shader optimize
|  98   |     Finished     | dgc shader group
|  99   |     Finished     | pipeline barrier
|  100  |     Finished     | Memory Barrier in dgc
|  101  |     Finished     | subPass self dependency
|  102  |     Finished     | subPass dependency
|  103  |     Finished     | delegate
|  104  |     Finished     | entity added to world through Create Slate
|  105  |     Finished     | DAG container
|  106  |     Finished     | Houdini style mesh description
|  107  |     Finished     | Build MeshPack lod
|  108  |     Finished     | Draw full viewport triangle
|  109  |                  | Dynamic lod switch in mesh shader
|  110  |                  | virtual geometry
|  111  |     Finished     | virtual geometry visualizer
|  112  |     Finished     | kdTree container
|  113  |     Finished     | statics py script
|  114  |     Finished     | adjust vertex input layout to position only
|  115  |     Finished     | meshlet lod
|  116  |     Finished     | Runtime Shader Compile
|  117  |     Finished     | Aftermath SDK
|  118  |     Finished     | Nvperf SDK
|  119  |     Finished     | nvtx SDK
|  120  |     Finished     | fragment barycentric
|  121  |     Finished     | GPU Profiler HUD
|  122  |     Finished     | Timeline Semaphore
|  123  |     Finished     | Video Memory budget HUD
|  124  |     Finished     | Viewport Grid Renderer
|  125  |                  | Viewport Grid fuse
|  126  |     Finished     | Viewport Grid Slate
|  127  |     Finished     | Digital draw in shader
|  128  |     Finished     | KTX texture compress
|  129  |     Finished     | MeshOptimizer
|  130  |     Finished     | Image host operation
|  131  |                  | Reflection
|  132  |     Finished     | variable traits
|  133  |     Finished     | function traits
|  134  |                  | class traits
|  135  |                  | Serialization
|  136  |                  | Assets System
|  137  |                  | Content Browser
|  138  |                  | Particle System
|  139  |     Finished     | MemoryPool
|  140  |     Finished     | ObjectPool
|  141  |     Finished     | ThreadCache
|  142  |     Finished     | CentralCache
|  143  |     Finished     | PageCache
|  144  |     Finished     | add tracy MemoryPoolAllocator marker
|  145  |     Finished     | MemoryPool entry
|  146  |     Finished     | freelist container
|  147  |     Finished     | radix trie container
|  148  |     Finished     | span container
|  149  |                  | Gltf extension
|  150  |     Finished     | Gltf mesh & texture load
|  151  |                  | PostProcessing Renderer
|  152  |     Finished     | blur
|  153  |     Finished     | bloom
|  154  |                  | TAA
|  155  |     Finished     | Tonemapping
|  156  |     Finished     | PostProcessing Slate
|  157  |     Finished     | SubPass Statistics
|  158  |     Finished     | timestamp querier
|  159  |     Finished     | pipeline statistics querier
|  160  |     Finished     | Behave state list container
|  161  |     Finished     | timestamp Slate
|  162  |     Finished     | pipeline statistics Slate
|  163  |     Finished     | Full Screen Mode
|  164  |     Finished     | Viewport Toggle
|  165  |     Finished     | Slate Layout save/load
|  166  |     Finished     | RayTracing shadow
|  167  |                  | Rasterization Shadow
|  168  |                  | Unlimited Lights
|  169  |     Finished     | thread safe container
|  170  |                  | LockFreeContainer
|  171  |     Finished     | renderer resource cache
|  172  |     Finished     | async entity added to world
|  173  |     Finished     | async raytracing acceleration structure build
|  174  |     Finished     | async BasePass dgc instance build
|  175  |     Finished     | async BasePass dgc instance rebuild
|  176  |                  | skybox fuse(mipmap)
|  177  |                  | GPU Work Graph
|  178  |                  | Occlusion query
|  179  |                  | Compute Shader cull
|  180  |                  | indirect draw
|  181  |                  | cvar
|  182  |     Finished     | mesh pack instance
|  183  |                  | DAG RDG
|  184  |                  | Emscripten Compile
|  185  |                  | framebuffer optimize
|  186  |                  | buffer compress
|  187  |                  | Nvperf Continus
|  188  |     Finished     | Nvperf OneShot
|  189  |                  | Tracy GPU
|  190  |     Finished     | query pool
|  191  |                  | dgc compute shader cull
|  192  |     Finished     | viewport resize suspend in debug mode
|  193  |     Finished     | resize over event cause resource in use error
|  194  |                  | sparse resource
|  195  |                  | tessllation shader
|  196  |                  | terrain
|  197  |                  | virtual texture map
|  198  |     Finished     | timeline semaphore wait idle
|  199  |                  | libclang AST
|  200  |                  | Vulkan Layer Configuration
|  201  |                  | grfconstruct
|  202  |                  | subgroup max pooling
|  203  |                  | hdr output
|  204  |                  | Garbage Collection
|  205  |                  | Custom slate pipeline shader
|  206  |                  | Network
|  207  |                  | Root Entity
|  208  |                  | Stage Tree
|  209  |                  | Stage Drag Drop