Debugger
============
# Introduce
***
* There are many types of debuggers in the SpiceEngine solution, covering various aspects of CPU, GPU, Memory, and test. 
The following is their distribution.
* CPU costs: **Tracy**, **Instrumentor**.
* GPU costs: **NsightPerf**, **SubPass Statistics**, **Nsight Graphics**.
* GPU resources state: **DebugUtils**, **Nsight Graphics**.
* GPU shader crash check: **Aftermath**.
* Program memory usage: **Tracy**.
* GPU video memory usage: **Nsight Graphics**.
* Program Threads state: **Tracy**.

# Aftermath
***
* **Usage:** Aftermath is a GPU crash checker. When Device lost happened, it will generate a crash report contains some possible reasons about the crash.
It helps to analyse the process of the crash.
* **Advantage:** When the cause of the crash is related to the shader, it can accurately locate the location where the error occurred.
* **Disadvantage:** When the cause of the crash is related to multiple threads render, It can hardly provide any valuable analytical reports.
* **Entry:** Aftermath will automatically start when device lost.
* **Output file folder:** 
  ~~~
  xxprojectxx/saved/GPUCrashReport
  ~~~
* **How to visualize report**
  * Open the dump file with Nsight Graphics
    ~~~
    xxprojectxx/saved/GPUCrashReport/xxx_xxx/xxx.nv-hpudmp
    ~~~
  * ScreenShot:<br/>
    <img alt="Aftermath" src="Aftermath.png"/>

# NsightPerf
***
## Continuous
***
* **TODO:** finish it.

## GpuProfilerHUD
***
* **Usage:** GpuProfilerHUD is a GPU performance analysis tool. It filtered out several of the most important metrics 
in the GPU rendering process and rendered them individually into a visualization chart.
* **Advantage:** This tool is integrated internally within the engine, without the need for third-party software, and covers the most important GPU performance evaluation metrics.
* **Disadvantage:** It is mainly used for rapid analysis and lacks extensive and detailed analysis of data.
* **Entry:** 
  ~~~
  Window -> Utilities -> Statistics -> GPU Runtime Profiler HUD
  ~~~
* **HUD ScreenShots:**<br/>
  <img alt="GpuProfilerHUD01" src="GpuProfilerHUD01.png"/>
  <img alt="GpuProfilerHUD02" src="GpuProfilerHUD02.png"/>
  <img alt="GpuProfilerHUD03" src="GpuProfilerHUD03.png"/>
  <br/>
* **Config:**
  * In used config: 
  ~~~
  ./SpicesEngine/vendor/NvPerfUtility/HudConfigurations/pub/xxx/SpicesEngineDefault.yaml
  ~~~
  * GpuProfilerHUD items can be configured in the following ways:
    1. Open configure file above.
    2. add new profiler metrics in both configurations and panels.
    3. save config file and run the script: 
      ~~~
      ./vendor/NvPerfUtility/HudConfigurations/hud_configurations_generator.bat
      ~~~
    4. restart the engine.
* **Config ScreenShots:**<br/>
  <img alt="GpuProfilerHUD01" src="GpuProfilerConfig01.png"/><br/>
***
  <img alt="GpuProfilerHUD02" src="GpuProfilerConfig02.png"/><br/>

## OneshotCollection
***
* **Usage:** OneshotCollection is a GPU performance analysis tool. It collects performance indicators of concern and outputs visualization files.
* **Advantage:** It supports collecting average performance metrics on multiple frames and offline visualization reports.
* **Disadvantage:**  It is mainly used for rapid analysis and lacks extensive and detailed analysis of data.
* **Entry:**
  ~~~
  Window -> Utilities -> Debugger -> GPU One-Shot Collection
  ~~~
* **Timeline Viewer ScreenShots:**<br/>
  <img alt="OneshotCollection" src="OneshotCollection.png"/>
  <br/>
* **Config:**
  * see GpuProfilerHUD
* **How to visualize report**
  * Start Timeline Viewer in the folder:
  ~~~
  ./vendor/TimelineViewer/TimelineViewer.exe
  ~~~
  * Select the report file in the folder:
  ~~~
  xxprojectxx/saved/GPUProfilerOneShot/xxxxx_xxxxx/counterData.bin
  ~~~
  * Select the config file in the folder:
  ~~~
  ./SpicesEngine/vendor/NvPerfUtility/HudConfigurations/pub/xxx/SpicesEngineDefault.yaml
  ~~~
  * Start Processing

## ReportGenerator
***
* **Usage:** ReportGenerator is a GPU performance analysis tool. It collects almost all relevant performance analysis metrics during GPU rendering and outputs HTML performance analysis reports.
* **Advantage:** It contains almost all rendering performance metrics, very detailed.
* **Disadvantage:** The generated report covers very detailed indicators, making it almost difficult to understand each one.
* **Entry:**
  ~~~
  Window -> Utilities -> Debugger -> GPU Report Generate
  ~~~
* **Report ScreenShots:**<br/>
  <img alt="ReportGenerator01" src="ReportGenerator01.png"/><br/>
***
  <img alt="ReportGenerator02" src="ReportGenerator02.png"/><br/>
***
  <img alt="ReportGenerator03" src="ReportGenerator03.png"/><br/>
* **Output folder:**
  ~~~
  xxprojectxx\saved\GPUProfilerReport
  ~~~
  
# Tracy
***
* **Usage:** Tracy is widely used in this solution for CPU time counting, memory pool allocation counting, and thread work analysis.
* **Advantage:** Real time, efficient, and widely covered, it is the primary entry point for performance analysis.
* **Disadvantage:** Cannot generate offline reports.
* **Entry:**
  ~~~
  Window -> Utilities -> Debugger -> CPU Profiler
  ~~~
* **ScreenShots:**
  * **CPU Cost:**<br/>
    <img alt="Tracy CPU" src="Tracy CPU.png"/><br/>
  * **Thread works:**<br/>
    <img alt="Tracy Thread" src="Tracy Thread.png"/><br/>
  * **Memory Allocation:**<br/>
    <img alt="Tracy Memory" src="Tracy Memory.png"/><br/>

# DebugUtils
***
* **Usage:** DebugUtils helps to mark vulkan resource which can be collected by frame capture software.
* **Advantage:** Support custom tagging of vulkan resources
* **Disadvantage:** Only can be used inside frame capture software.
* **How to use**
  1. Mark vulkan resource with MACRO:
     <img alt="DebugUtilsMacros" src="DebugUtilsMacros.png"/><br/>
  2. Capture frame and see resources.

# Instrumentor
***
* **Usage:** Instrumentor is used in this solution for Unit Test CPU time counting and thread work analysis.
* **Advantage:** Can generate offline reports.
* **Disadvantage:** Does not support on-demand analysis.
* **How to use**
  1. Run SpicesTest project.
  2. Open the file with chrome website:
    ~~~
    ./SpicesTest/SpiceslProfile-SpicesTest.json
    chrome://tracing
    ~~~
* **ScreenShot:**<br/>
  <img alt="Instrumentor" src="Instrumentor.png"/><br/>

# SubPass Statistics
***
* **Usage:** Statistics is used to analyze the time consumption and pipeline status of subPass of GPU rendering.
* **Advantage:** Easy to use.
* **Disadvantage:** Can only analyze a single frame.
* **Entry:**
  ~~~
  Window -> Utilities -> Statistics -> Renderer Profiler HUD
  ~~~
* **ScreenShot:**<br/>
  <img alt="Statistics01" src="Statistics01.png"/>
  <img alt="Statistics02" src="Statistics02.png"/>
  <img alt="Statistics03" src="Statistics03.png"/>

# Video Memory HUD
***
* **Usage:** It is used to display video memory heaps usage.
* **Advantage:** Easy to use.
* **Disadvantage:** Not supporting displaying the memory usage of a single resource.
* **Entry:**
  ~~~
  Window -> Utilities -> Statistics -> Video Memory Runtime HUD
  ~~~
* **ScreenShot:**<br/>
  <img alt="Video Memory HUD" src="Video Memory HUD.png"/>

# Nsight Graphics
***
## Frame Debugger
***
* **Usage:** Frame Debugger is used to capturing frame and analyse rendering process.
* **Advantage:** Commonly used, easy to use, and debuggable.
* **Disadvantage:** Not supporting detailed analysis of pipelines.
* **Launch:**
  * Application Executable:
    ~~~
    ./bin/xxx/xxProjectxx/xxx.exe
    ~~~
  * Working Directory:
    ~~~
    xxProjectxx
    ~~~
  * Environment: PATH=LibraryDir.HoudiniEnginedll;LibraryDir.NvAftermath;LibraryDir.NvPerf;LibraryDir.VulkanSDKDLL
* **Vulkan:**
  * Device Address C++ Support: **No**
  * Enable Coherent Buffer Collection: **No**
* **Troubleshooting:**
  * Collect Hardware Performance Metrics: **No**
  * Block on First Incompatibility: **Disable**
* **Screen Shots:**
  * Scrubber<br/>
    <img alt="Scrubber" src="Scrubber.png"/>
***
  * Inspector<br/>
    <img alt="Inspector" height="901" src="Inspector.png" width="1800"/>
***
  * Events<br/>
    <img alt="Events" src="Events.png"/>
***
  * API Statistics<br/>
    <img alt="API Statistics" src="API Statistics.png"/>
***
  * Resources<br/>
    <img alt="Resources" height="540" src="Resources.png" width="1800"/>
***
  * DescriptorSets<br/>
    <img alt="DescriptorSets" src="DescriptorSets.png"/>
***
  * DeviceMemory<br/>
    <img alt="DeviceMemory" height="597" src="DeviceMemory.png" width="1800"/>
***
  * Objects<br/>
    <img alt="Objects" height="677" src="Objects.png" width="1800"/>
***
  * Shader<br/>
    <img alt="Shader" src="Shader.png"/>
***

## C++ Capture
***
* **Usage:** 
* **Advantage:** 
* **Disadvantage:** 
* **Launch:**
  * Application Executable:
    ~~~
    ./bin/xxx/xxProjectxx/xxx.exe
    ~~~
  * Working Directory:
    ~~~
    xxProjectxx
    ~~~
  * Environment: PATH=LibraryDir.HoudiniEnginedll;LibraryDir.NvAftermath;LibraryDir.NvPerf;LibraryDir.VulkanSDKDLL
* **Vulkan:**
  * Device Address C++ Support: **No**
  * Enable Coherent Buffer Collection: **No**
* **Troubleshooting:**
  * Block on First Incompatibility: **Disable**
* **Generate C++ Project**
  1. Generate C++ Capture
  2. Build
  3. Open IDE
  4. Run

## GPU Trace Profiler
***
* **Usage:** 
* **Advantage:** 
* **Disadvantage:** 
* **Launch:**
  * Application Executable:
    ~~~
    ./bin/xxx/xxProjectxx/xxx.exe
    ~~~
  * Working Directory:
    ~~~
    xxProjectxx
    ~~~
  * Environment: PATH=LibraryDir.HoudiniEnginedll;LibraryDir.NvAftermath;LibraryDir.NvPerf;LibraryDir.VulkanSDKDLL
  * Automatically Connect: **NO**
* **Ada(NVIDIA GeForce RTX 4060)**
  * Multi-Pass Metrics: **YES**
  * Real-Time Shader Profiler: **YES**
  * Capture Screenshot: **NO**
* **Attach**
  1. Select Process
  2. Attach GPU Trace
  3. Generate GPU Trace Capture
* **Screen Shots:**
  * Timeline<br/>
    <img alt="Timeline" src="Timeline.png"/>
***
  * Metrics<br/>
    <img alt="Metrics" src="Metrics.png"/>
***
  * HotSpots<br/>
    <img alt="HotSpots" src="HotSpots.png"/>
***
  * ShaderPipelines<br/>
    <img alt="ShaderPipelines" src="ShaderPipelines.png"/>
***
  * Summary<br/>
    <img alt="Summary01" src="Summary01.png"/>
    <img alt="Summary02" src="Summary02.png"/>
    <img alt="Summary03" src="Summary03.png"/>
***
  * ShaderSource<br/>
    <img alt="ShaderSource" src="ShaderSource.png"/>
***
  * TraceAnalysis<br/>
    <img alt="TraceAnalysis" src="TraceAnalysis.png"/>
***

# NVTX
***
* **Usage:** NVTX helps mark the process of rendering.
* **TODO:** finish it.