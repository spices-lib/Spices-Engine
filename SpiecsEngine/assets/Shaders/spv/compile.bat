%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.BasePassRenderer.Mesh.Default.vert -o Shader.BasePassRenderer.Mesh.Default.vert.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.BasePassRenderer.Mesh.Default.frag -o Shader.BasePassRenderer.Mesh.Default.frag.spv

%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.BasePassRenderer.SkyBox.Default.vert -o Shader.BasePassRenderer.SkyBox.Default.vert.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.BasePassRenderer.SkyBox.Default.frag -o Shader.BasePassRenderer.SkyBox.Default.frag.spv

%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.SceneComposeRenderer.SceneCompose.Default.vert -o Shader.SceneComposeRenderer.SceneCompose.Default.vert.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.SceneComposeRenderer.SceneCompose.Default.frag -o Shader.SceneComposeRenderer.SceneCompose.Default.frag.spv

%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.WorldPickRenderer.WorldPick.Default.vert -o Shader.WorldPickRenderer.WorldPick.Default.vert.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.WorldPickRenderer.WorldPick.Default.frag -o Shader.WorldPickRenderer.WorldPick.Default.frag.spv

%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.SpriteRenderer.Sprite.Default.vert -o Shader.SpriteRenderer.Sprite.Default.vert.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.SpriteRenderer.Sprite.Default.frag -o Shader.SpriteRenderer.Sprite.Default.frag.spv

%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.WorldPickStage2Renderer.WorldPickStage2.Default.vert -o Shader.WorldPickStage2Renderer.WorldPickStage2.Default.vert.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.WorldPickStage2Renderer.WorldPickStage2.Default.frag -o Shader.WorldPickStage2Renderer.WorldPickStage2.Default.frag.spv

%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.MeshNormalVisualizer.vert -o Shader.MeshNormalVisualizer.vert.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.MeshNormalVisualizer.geom -o Shader.MeshNormalVisualizer.geom.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.MeshNormalVisualizer.frag -o Shader.MeshNormalVisualizer.frag.spv

pause