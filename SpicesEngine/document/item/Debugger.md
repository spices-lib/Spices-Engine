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
* **Output file folder:** 
  ~~~
  ./xxprojectxx/saved/GPUCrashReport
  ~~~


# NVTX



# NsightPerf

# Tracy

# DebugUtils

# Instrumentor

# SubPass Statistics

# Nsight Graphics