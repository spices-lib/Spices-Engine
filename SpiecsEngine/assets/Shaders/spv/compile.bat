%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.MeshRenderer.Default.vert -o Shader.MeshRenderer.Default.vert.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.MeshRenderer.Default.frag -o Shader.MeshRenderer.Default.frag.spv

%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.SkyBoxRenderer.Default.vert -o Shader.SkyBoxRenderer.Default.vert.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.SkyBoxRenderer.Default.frag -o Shader.SkyBoxRenderer.Default.frag.spv

%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.SceneComposeRenderer.Default.vert -o Shader.SceneComposeRenderer.Default.vert.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.SceneComposeRenderer.Default.frag -o Shader.SceneComposeRenderer.Default.frag.spv

%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.WorldPickRenderer.Default.vert -o Shader.WorldPickRenderer.Default.vert.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.WorldPickRenderer.Default.frag -o Shader.WorldPickRenderer.Default.frag.spv

%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.SpriteRenderer.Default.vert -o Shader.SpriteRenderer.Default.vert.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.SpriteRenderer.Default.frag -o Shader.SpriteRenderer.Default.frag.spv

%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.WorldPickStage2Renderer.Default.vert -o Shader.WorldPickStage2Renderer.Default.vert.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.WorldPickStage2Renderer.Default.frag -o Shader.WorldPickStage2Renderer.Default.frag.spv

pause