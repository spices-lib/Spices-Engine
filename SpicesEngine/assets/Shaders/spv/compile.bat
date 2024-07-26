%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.BasePassRenderer.Mesh.Default.vert  -o Shader.BasePassRenderer.Mesh.Default.vert.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.BasePassRenderer.Mesh.Default.frag  -o Shader.BasePassRenderer.Mesh.Default.frag.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.BasePassRenderer.Mesh.Default.rchit -o Shader.BasePassRenderer.Mesh.Default.rchit.spv       --target-env=vulkan1.3
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.BasePassRenderer.Mesh.CornelBox.frag -o Shader.BasePassRenderer.Mesh.CornelBox.frag.spv       --target-env=vulkan1.3
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.BasePassRenderer.Mesh.CornelBox.rchit -o Shader.BasePassRenderer.Mesh.CornelBox.rchit.spv       --target-env=vulkan1.3
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.BasePassRenderer.Mesh.interior_stair_wl3ieamdw.rchit -o Shader.BasePassRenderer.Mesh.interior_stair_wl3ieamdw.rchit.spv       --target-env=vulkan1.3
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.BasePassRenderer.Mesh.interior_stair_wl3ieamdw.frag -o Shader.BasePassRenderer.Mesh.interior_stair_wl3ieamdw.frag.spv       --target-env=vulkan1.3
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.BasePassRenderer.Mesh.Sphere.frag -o Shader.BasePassRenderer.Mesh.Sphere.frag.spv       --target-env=vulkan1.3

%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.BasePassRenderer.SkyBox.Default.vert -o Shader.BasePassRenderer.SkyBox.Default.vert.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.BasePassRenderer.SkyBox.Default.frag -o Shader.BasePassRenderer.SkyBox.Default.frag.spv

%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.ShadowRenderer.DirectionalLightShadow.Default.vert -o Shader.ShadowRenderer.DirectionalLightShadow.Default.vert.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.ShadowRenderer.DirectionalLightShadow.Default.geom -o Shader.ShadowRenderer.DirectionalLightShadow.Default.geom.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.ShadowRenderer.DirectionalLightShadow.Default.frag -o Shader.ShadowRenderer.DirectionalLightShadow.Default.frag.spv

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

%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.TestRenderer.Test.Default.vert -o Shader.TestRenderer.Test.Default.vert.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.TestRenderer.Test.Default.geom -o Shader.TestRenderer.Test.Default.geom.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.TestRenderer.Test.Default.frag -o Shader.TestRenderer.Test.Default.frag.spv

%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.RayTracingRenderer.RayTracing.Default.rgen         -o Shader.RayTracingRenderer.RayTracing.Default.rgen.spv         --target-env=vulkan1.3
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.RayTracingRenderer.RayTracing.Default.rmiss        -o Shader.RayTracingRenderer.RayTracing.Default.rmiss.spv        --target-env=vulkan1.3
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.RayTracingRenderer.RayTracing.DefaultShadow.rmiss  -o Shader.RayTracingRenderer.RayTracing.DefaultShadow.rmiss.spv  --target-env=vulkan1.3

%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.RayTracingComposeRenderer.RayTracingCompose.Default.vert -o Shader.RayTracingComposeRenderer.RayTracingCompose.Default.vert.spv
%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.RayTracingComposeRenderer.RayTracingCompose.Default.frag -o Shader.RayTracingComposeRenderer.RayTracingCompose.Default.frag.spv

%VULKAN_SDK%/Bin/glslc.exe ../src/Shader.ParticleRenderer.Particle.Default.comp -o Shader.ParticleRenderer.Particle.Default.comp.spv

pause