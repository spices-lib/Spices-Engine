%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.MeshRenderer.Default.vert -o Shader.MeshRenderer.Default.vert.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.MeshRenderer.Default.frag -o Shader.MeshRenderer.Default.frag.spv

%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.SkyBoxRenderer.Default.vert -o Shader.SkyBoxRenderer.Default.vert.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.SkyBoxRenderer.Default.frag -o Shader.SkyBoxRenderer.Default.frag.spv

%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.SceneComposeRenderer.vert -o Shader.SceneComposeRenderer.vert.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.SceneComposeRenderer.frag -o Shader.SceneComposeRenderer.frag.spv

%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.WorldPickRenderer.vert -o Shader.WorldPickRenderer.vert.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.WorldPickRenderer.frag -o Shader.WorldPickRenderer.frag.spv

%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.SpriteRenderer.Default.vert -o Shader.SpriteRenderer.Default.vert.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.SpriteRenderer.Default.frag -o Shader.SpriteRenderer.Default.frag.spv

%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.WorldPickStage2Renderer.vert -o Shader.WorldPickStage2Renderer.vert.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.WorldPickStage2Renderer.frag -o Shader.WorldPickStage2Renderer.frag.spv

pause