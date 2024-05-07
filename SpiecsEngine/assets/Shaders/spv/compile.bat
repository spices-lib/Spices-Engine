%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.MeshRenderer.vert -o Shader.MeshRenderer.vert.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.MeshRenderer.frag -o Shader.MeshRenderer.frag.spv

%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.SkyBoxRenderer.vert -o Shader.SkyBoxRenderer.vert.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.SkyBoxRenderer.frag -o Shader.SkyBoxRenderer.frag.spv

%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.SceneComposeRenderer.vert -o Shader.SceneComposeRenderer.vert.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.SceneComposeRenderer.frag -o Shader.SceneComposeRenderer.frag.spv

%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.WorldPickRenderer.vert -o Shader.WorldPickRenderer.vert.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.WorldPickRenderer.frag -o Shader.WorldPickRenderer.frag.spv

pause