%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.MeshRenderer.vert -o Shader.MeshRenderer.vert.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.MeshRenderer.frag -o Shader.MeshRenderer.frag.spv

%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.SkyBoxRenderer.vert -o Shader.SkyBoxRenderer.vert.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.SkyBoxRenderer.frag -o Shader.SkyBoxRenderer.frag.spv

pause