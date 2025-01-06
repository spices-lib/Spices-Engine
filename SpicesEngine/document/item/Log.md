Log
============
* **Log Level**
  * Trace
  * Info
  * Warn
  * Error
  * Critical
* **Log files folder:**
~~~
xxprojectxx/saved/ConsoleLog
~~~
* **Output to console**<br/>
  <img alt="Console" height="785" src="Console.png" width="1800"/>
* **Output to console ui**<br/>
  <img alt="UIConsole" height="407" src="UIConsole.png" width="1800"/>
* **Output to local files**<br/>
  <img alt="LogFiles" height="621" src="LogFiles.png" width="1800"/>

# Game Log
***
* **Use Macro enable spdlog collection**<br/>
  <img alt="LogMacro" height="685" src="LogMacro.png" width="1800"/>

# Render Log
***
* **Use Macro enable spdlog collection**<br/>
  <img alt="VkMacro" height="205" src="VkMacro.png" width="1800"/>

# Shader Log
***
* **Enable Shader log Feature**
  ~~~
  ./SpicesEngine/src/Render/Vulkan/VulkanInstance.cpp
  ~~~
  <img alt="ShaderLogFeature" height="240" src="ShaderLogFeature.png" width="1800"/>
* **Use function in shader enable spdlog collection**
  ~~~
  debugPrintfEXT("Hello: %d", num);
  ~~~