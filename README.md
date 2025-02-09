以下内容为未尚未整理到部署文档的临时部分。
临时存放使用,
SpicesEngine项目的详情在[此处](https://spices-engine.vercel.app/index.html)。

<a name="EdKx2"></a>
# **Project Introduction**
---
**Project Name:** Spices Engine<br />**Start Date**: March 27, 2024<br />**Project Purpose**:<br />The primary objective of the Spices Engine project is to validate and deepen my understanding and proficiency in several advanced fields of graphics programming and engine development, specifically Vulkan, engine development principles, C++, rendering pipelines, ray tracing, and shader development. Prior to writing the first line of code, I underwent a rigorous six-month period of study to ensure I had the necessary knowledge to successfully complete this project.<br />**Project Features and Implementation**:
1. **Premake Auto Build**
2. **Vulkan Render backend**
3. **Rasterization Pipeline**
4. **Ray Tracing Pipeline**
5. **Mesh Pipeline**
6. **DGC**
7. **Physically Based Rendering (PBR) **
8. **Entity Component System (ECS)**
9. **Log System**
10. **Documentation**
11. **Rendering Pipeline Analysis**
12. **Performance Analysis**
13. **Unit Test**
14. **Git Version Control**
15. **User Interface(Imgui)**

**Project Todo:**
1. **Bi-Directional Path Tracing**
2. **Houdini LiveLink**
3. **Work Graph**

**Device:**
1. **NVIDIA GeForce RTX 4060**

**Reference:**
1. [**https://github.com/TheCherno/Hazel**](https://github.com/TheCherno/Hazel)
2. [**https://github.com/travisvroman/kohi**](https://github.com/travisvroman/kohi)
3. [**https://github.com/blurrypiano/littleVulkanEngine**](https://github.com/blurrypiano/littleVulkanEngine)
4. [**https://github.com/EasyJellySniper/Unheard-Engine**](https://github.com/EasyJellySniper/Unheard-Engine)
5. [**https://github.com/KhronosGroup/Vulkan-Samples/tree/main**](https://github.com/KhronosGroup/Vulkan-Samples/tree/main)
6. [**https://github.com/SaschaWillems/Vulkan**](https://github.com/SaschaWillems/Vulkan)
7. [**https://github.com/Zielon/PBRVulkan**](https://github.com/Zielon/PBRVulkan)
8. [**https://github.com/nvpro-samples/vk_mini_path_tracer**](https://github.com/nvpro-samples/vk_mini_path_tracer)
9. [**https://github.com/nvpro-samples/vk_raytracing_tutorial_KHR**](https://github.com/nvpro-samples/vk_raytracing_tutorial_KHR)
10. [**https://docs.vulkan.org/tutorial/latest/00_Introduction.html**](https://docs.vulkan.org/tutorial/latest/00_Introduction.html)
11. [**https://github.com/KhronosGroup/Vulkan-Guide**](https://github.com/KhronosGroup/Vulkan-Guide)
12. [**https://github.com/KhronosGroup/Vulkan-Samples**](https://github.com/KhronosGroup/Vulkan-Samples)
13. [**https://github.com/SaschaWillems/Vulkan-glTF-PBR**](https://github.com/SaschaWillems/Vulkan-glTF-PBR)
14. [**https://zeux.io/2020/02/27/writing-an-efficient-vulkan-renderer/**](https://zeux.io/2020/02/27/writing-an-efficient-vulkan-renderer/)
15. [**https://github.com/GPUOpen-LibrariesAndSDKs/glTFSample**](https://github.com/GPUOpen-LibrariesAndSDKs/glTFSample)
16. [**https://www.saschawillems.de/**](https://www.saschawillems.de/)
17. [**https://taidaesal.github.io/vulkano_tutorial/**](https://taidaesal.github.io/vulkano_tutorial/)
18. [**https://www.gsn-lib.org/docs/nodes/raytracing.php**](https://www.gsn-lib.org/docs/nodes/raytracing.php)
19. [**https://puluo.top/optix_01/**](https://puluo.top/optix_01/)
20. [**https://raytracing.github.io/books/RayTracingInOneWeekend.html#diffusematerials/truelambertianreflection**](https://raytracing.github.io/books/RayTracingInOneWeekend.html#diffusematerials/truelambertianreflection)
21. [**https://www.willusher.io/graphics/2019/11/20/the-sbt-three-ways/**](https://www.willusher.io/graphics/2019/11/20/the-sbt-three-ways/)
22. [**https://www.pbr-book.org/4ed/contents**](https://www.pbr-book.org/4ed/contents)
23. [**https://github.com/nvpro-samples/gl_vk_meshlet_cadscene**](https://github.com/nvpro-samples/gl_vk_meshlet_cadscene)
24. [**https://developer.nvidia.com/blog/new-vulkan-device-generated-commands/**](https://developer.nvidia.com/blog/new-vulkan-device-generated-commands/)
<a name="ym5nT"></a>
# Vulkan Render backend
---
Spices Engine use vulkan as render backend
<a name="dP7LC"></a>
# Rasterization Pipeline
---
- **Scene screenshot**
![SwapChainImage.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721471601026-0add88cf-38b0-45ac-b818-d70de73ee049.png#averageHue=%23475924&clientId=u7a1e0cda-1053-4&from=drop&id=u90ca8fc9&originHeight=1300&originWidth=2200&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=412682&status=done&style=none&taskId=ud47f99ad-8d47-4540-8925-e7416419c49&title=)
- **Nsight screenshot**
![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721471634889-f4f303b5-c875-47d5-a7c5-e8b9c546cdae.png#averageHue=%23d68741&clientId=u7a1e0cda-1053-4&from=paste&height=295&id=u3dcdbb0e&originHeight=369&originWidth=2181&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=48638&status=done&style=none&taskId=ua414e5b7-a791-408e-825b-3d82550b86b&title=&width=1744.8)<br />![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721471798707-dffed65d-3c01-4a1b-9e3a-975b398fbc17.png#averageHue=%23343433&clientId=u7a1e0cda-1053-4&from=paste&height=699&id=u92d68e44&originHeight=874&originWidth=1476&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=94193&status=done&style=none&taskId=ud68c5d18-323f-421c-b719-f69acc46549&title=&width=1180.8)
- **c++ renderer code sample**
file in:  SpicesEngine/src/Render/Renderer/SpecificRenderer/BasePassRenderer.cpp<br />![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721471999802-48a549c7-b2bb-4881-86e8-dd524f13218f.png#averageHue=%23282727&clientId=u7a1e0cda-1053-4&from=paste&height=909&id=UUpM8&originHeight=1136&originWidth=1185&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=248199&status=done&style=none&taskId=ub87edc5f-9554-4ee0-b471-f793050480c&title=&width=948)<br />![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721472055403-6b4f3c0e-627a-4571-8e22-deef725b402a.png#averageHue=%23272626&clientId=u7a1e0cda-1053-4&from=paste&height=975&id=UIgDv&originHeight=1219&originWidth=1204&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=181096&status=done&style=none&taskId=u05993ca7-190d-4902-8bdd-4f6592223ba&title=&width=963.2)
- **shader code sample**
file in: SpicesEngine/assets/Shaders/src/Shader.BasePassRenderer.Mesh.Default.vert<br />![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721472223309-77d85a29-7367-4d41-8f99-87a441568d14.png#averageHue=%23262626&clientId=u7a1e0cda-1053-4&from=paste&height=1026&id=bJAyO&originHeight=1282&originWidth=1027&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=125907&status=done&style=none&taskId=u89f3b53d-ae3d-4186-a915-8cb3d2813e6&title=&width=821.6)
<a name="ZO6Yu"></a>
# Ray Tracing Pipeline
---
- **Scene screenshot**
![SwapChainImage__2_.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721472619750-cda5ffc6-09d0-46a4-aa00-00b15330fdeb.png#averageHue=%232f464c&clientId=u7a1e0cda-1053-4&from=drop&id=ubeb88d1d&originHeight=1369&originWidth=2560&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=1869161&status=done&style=none&taskId=u8818103c-7971-4a98-a0de-dfa4a6481be&title=)
- **Nsight screenshot**
![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721472700722-c421211b-4fde-4335-8b66-9542fda1ffb4.png#averageHue=%23373636&clientId=u7a1e0cda-1053-4&from=paste&height=1037&id=u232e39ad&originHeight=1296&originWidth=1972&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=168392&status=done&style=none&taskId=u8e0cae6a-a26e-476c-aabc-c873c7d477e&title=&width=1577.6)
- Acceleration Structures
![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721472846986-bf28b6fa-35e8-4d8e-9d46-4ec3cf341425.png#averageHue=%23447d88&clientId=u7a1e0cda-1053-4&from=paste&height=1028&id=u93b8f91d&originHeight=1285&originWidth=2056&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=567772&status=done&style=none&taskId=u614c5b0a-8c50-41ca-a151-da81cceaa79&title=&width=1644.8)
- Shader Timeing Heatmap
![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721473072152-14ec6377-b286-4444-8075-02166f0b2d07.png#averageHue=%23001c7a&clientId=u7a1e0cda-1053-4&from=paste&height=774&id=ubf044657&originHeight=968&originWidth=2042&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=917712&status=done&style=none&taskId=u1a550f86-9735-459c-9c60-f9a7d518c35&title=&width=1633.6)
- **c++ renderer code sample**
file in:  SpicesEngine/src/Render/Renderer/SpecificRenderer/RayTracingRenderer.cpp<br />![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721473200196-6ad4e69e-6566-48c4-a2a4-ba597b6d6c02.png#averageHue=%23282726&clientId=u7a1e0cda-1053-4&from=paste&height=274&id=O0YzU&originHeight=342&originWidth=1609&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=120175&status=done&style=none&taskId=ub6b7c628-3d20-4880-bf20-8a6fcfbfc7c&title=&width=1287.2)<br />![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721473221155-f8820f5d-dc24-4f6c-9c5f-09e03fc8673f.png#averageHue=%23272626&clientId=u7a1e0cda-1053-4&from=paste&height=727&id=Azsc2&originHeight=909&originWidth=1260&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=137881&status=done&style=none&taskId=u8068c5d2-c39a-4b9e-95d1-31ea38209fa&title=&width=1008)
- **shader code sample**
file in: SpicesEngine/assets/Shaders/src/Header/ShaderClosestHit.glsl<br />![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721473325987-94759c37-ae33-4c27-b694-3a897fd60d3b.png#averageHue=%23282727&clientId=u7a1e0cda-1053-4&from=paste&height=967&id=J6eBz&originHeight=1064&originWidth=1020&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=126405&status=done&style=none&taskId=u14de1198-ed52-4036-b24b-823cd225c7b&title=&width=927.2727071746326)
<a name="h1aSm"></a>
# Mesh Pipeline
---
- **Scene screenshot**
![屏幕截图 2024-08-25 202619.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1724589224154-44965a34-6784-4dc0-9417-0a0620b0bfe5.png#averageHue=%23409a67&clientId=ud381dff5-f5e5-4&from=drop&id=uaea2cdba&originHeight=1324&originWidth=2201&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=470380&status=done&style=none&taskId=u309e40cb-393b-411b-be5a-1b9cada3a17&title=)
- **Mesh Shaders**
file in:  SpicesEngine/assets/Shaders/src/Header/Shader.BasePassRenderer.Mesh.Default.mesh<br />![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1724589343315-083f2454-8b97-4363-a939-6efaefd10048.png#averageHue=%23272626&clientId=ud381dff5-f5e5-4&from=paste&height=851&id=u290418d8&originHeight=1277&originWidth=1746&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=181859&status=done&style=none&taskId=ua0dd2c44-dda6-4db1-a836-ea486674a82&title=&width=1164)<br />![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1724589377298-648de39e-53ff-473c-8651-437aa0f65afe.png#averageHue=%23262626&clientId=ud381dff5-f5e5-4&from=paste&height=797&id=udb87c3e7&originHeight=1196&originWidth=1739&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=158600&status=done&style=none&taskId=ud87811eb-8fd6-4619-aa28-68e0bbf9881&title=&width=1159.3333333333333)
- **Frustum Cull**
![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1724589771276-b543cdca-05d1-4e21-a9db-fe6675bc6516.png#averageHue=%23272626&clientId=ud381dff5-f5e5-4&from=paste&height=563&id=u66b08cbb&originHeight=844&originWidth=1664&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=130043&status=done&style=none&taskId=u554709a0-4bcb-4763-bd3c-b3b35e30bc5&title=&width=1109.3333333333333)
- **Normal Cull**
![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1724589785437-9aa56825-5183-4ec9-8295-3e3cf1437d52.png#averageHue=%23272626&clientId=ud381dff5-f5e5-4&from=paste&height=295&id=u89e26029&originHeight=443&originWidth=1402&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=60155&status=done&style=none&taskId=uddf62893-9266-44bc-8422-0962a84be52&title=&width=934.6666666666666)
- **Task**
file in:  SpicesEngine/assets/Shaders/src/Header/Shader.BasePassRenderer.Mesh.Default.task<br />![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1724589825394-537aa98c-61fa-4f08-a96d-6046386e3e7a.png#averageHue=%23272727&clientId=ud381dff5-f5e5-4&from=paste&height=723&id=u67ca4981&originHeight=1084&originWidth=1749&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=122546&status=done&style=none&taskId=u7d2c7b36-8cbd-413d-a581-2a0e9ec78c5&title=&width=1166)
<a name="SR74e"></a>
# DGC
---
- **Support 100W DrawCall（3 Points Per Mesh） with 40 FPS in 4060 or 30W DrawCall(600 Points Per Mesh)**
file in:  SpicesEngine/src/Render/Renderer/SpecificRenderer/BasePassRenderer.cpp<br />![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1724589939026-38229d51-0193-444d-84d2-83bfe943765e.png#averageHue=%23272727&clientId=ud381dff5-f5e5-4&from=paste&height=357&id=ub9b021e3&originHeight=535&originWidth=1537&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=101466&status=done&style=none&taskId=ue8b2792c-af95-4f4f-84e1-ec98b7736fe&title=&width=1024.6666666666667)
<a name="oUmkY"></a>
# Physically Based Rendering (PBR) 
---
- **BRDF diffuse**
![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721473825047-6fdca373-fb26-4784-993e-1b211bcb2825.png#averageHue=%2353657d&clientId=u7a1e0cda-1053-4&from=paste&height=805&id=u7e610bbe&originHeight=1006&originWidth=1994&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=3347087&status=done&style=none&taskId=u76baa548-1ec8-47a1-b75b-62e71df6e5f&title=&width=1595.2)
- **BRDF specular - Normal distribution + Geometric shadowing term + Fresnel**
![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721474171005-c3b28cb0-8f2a-4716-8f19-4083e66358a8.png#averageHue=%23030300&clientId=u7a1e0cda-1053-4&from=paste&height=807&id=u1321c94c&originHeight=1009&originWidth=1986&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=760964&status=done&style=none&taskId=u09a6eaca-550a-454a-b76e-06b9770e679&title=&width=1588.8)
- **BRDF diffuse + specular**
![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721474313751-c45949c5-0711-473f-b945-78c5df42acb6.png#averageHue=%234d6984&clientId=u7a1e0cda-1053-4&from=paste&height=808&id=ua3cb8af2&originHeight=1010&originWidth=1995&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=3878116&status=done&style=none&taskId=u74090082-8037-45f9-93b0-56c862e7d10&title=&width=1596)
- **PBR Shader code sample**
file in:  SpicesEngine/assets/Shaders/src/Header/ShaderFunctionLibrary.glsl<br />![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721474431169-0c8a1f41-69d8-40c0-9119-f66345609643.png#averageHue=%23272626&clientId=u7a1e0cda-1053-4&from=paste&height=798&id=qVVB2&originHeight=997&originWidth=1219&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=117018&status=done&style=none&taskId=ucbe3bb33-11d1-4a4c-a4f3-cd69ac44beb&title=&width=975.2)
- **Material sample**
file in:  SpicesEngine/assets/Materials/src/Material.BasePassRenderer.Mesh.Default.material<br />![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721474579331-d98a15c3-23da-4e84-bb6b-8caa6745f400.png#averageHue=%23272727&clientId=u7a1e0cda-1053-4&from=paste&height=907&id=JmAf7&originHeight=1134&originWidth=1926&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=280073&status=done&style=none&taskId=ufe655883-f247-4c5b-a243-f41fdaae7bc&title=&width=1540.8)
<a name="PgrCY"></a>
# Entity Component System (ECS)
---
Spices Engine use ECS as the manage solution of game world objects
<a name="xyVpM"></a>
# Unit Test
---
All unit tests are located at folder **SpicesTest/src** as a project<br />![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721552740552-12100a74-7ca1-4543-9d5f-204de0653af3.png#averageHue=%232c2c2b&clientId=uf73232b3-ecc6-4&from=paste&height=486&id=ufbeea045&originHeight=535&originWidth=1272&originalType=binary&ratio=1.100000023841858&rotation=0&showTitle=false&size=61183&status=done&style=none&taskId=u2210c2a7-43f1-427d-a5b1-1dca5def422&title=&width=1156.36361130013)
<a name="JoaS6"></a>
# User Interface(Imgui)
---
User Interface is designed based on Nvidia USD Composer style<br />MainMenu<br />![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721553266885-9b37500d-c02e-4a3e-9ee6-a4ebde9b1a75.png#averageHue=%236b6f79&clientId=uf73232b3-ecc6-4&from=paste&height=521&id=ufe49f289&originHeight=573&originWidth=749&originalType=binary&ratio=1.100000023841858&rotation=0&showTitle=false&size=305373&status=done&style=none&taskId=u5b80bb96-1d78-42f5-9888-e3f50e91f13&title=&width=680.9090761507841)<br />stage<br />![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721553031419-042db42d-29ed-4dfe-a8bc-4370ed10d5fe.png#averageHue=%23515151&clientId=uf73232b3-ecc6-4&from=paste&height=509&id=u67f79c83&originHeight=560&originWidth=522&originalType=binary&ratio=1.100000023841858&rotation=0&showTitle=false&size=73108&status=done&style=none&taskId=u64abf96e-7ac9-41c3-96f9-021f3099b5a&title=&width=474.545444259959)<br />Property<br />![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721553169122-b8a144ba-7b7b-4b9b-a24b-965fff8fe751.png#averageHue=%231bdf1b&clientId=uf73232b3-ecc6-4&from=paste&height=678&id=u920f6fd6&originHeight=746&originWidth=523&originalType=binary&ratio=1.100000023841858&rotation=0&showTitle=false&size=60076&status=done&style=none&taskId=uc496f10d-76ea-46ba-9022-4e69c9a1e79&title=&width=475.45453514934593)<br />Material Panel<br />![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721553195253-f9bc1a52-a7d0-4eca-971c-a943ea436e43.png#averageHue=%23323433&clientId=uf73232b3-ecc6-4&from=paste&height=508&id=u23bc9d47&originHeight=559&originWidth=527&originalType=binary&ratio=1.100000023841858&rotation=0&showTitle=false&size=47297&status=done&style=none&taskId=u2b8a3afe-4f05-4628-9984-ed53101c889&title=&width=479.09089870689354)<br />Console<br />![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721553215980-2ba352e6-db3a-4980-89e6-4cfc184a0285.png#averageHue=%23212224&clientId=uf73232b3-ecc6-4&from=paste&height=305&id=u3ca1342e&originHeight=336&originWidth=2003&originalType=binary&ratio=1.100000023841858&rotation=0&showTitle=false&size=86911&status=done&style=none&taskId=u1ffacb54-f17e-4bb9-b41e-c4a042bd924&title=&width=1820.9090514419502)<br />Viewport<br />![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721553239664-4be35d01-9317-45e9-8c78-d131d45db375.png#averageHue=%23667388&clientId=uf73232b3-ecc6-4&from=paste&height=878&id=u97504a6d&originHeight=966&originWidth=1991&originalType=binary&ratio=1.100000023841858&rotation=0&showTitle=false&size=2596285&status=done&style=none&taskId=uaf757af9-ed32-46b3-9202-0b5254e2353&title=&width=1809.9999607693073)<br />Gizmo<br />![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721553322404-bb4d700f-03d7-462c-bafc-8dcd34e7852b.png#averageHue=%233b506b&clientId=uf73232b3-ecc6-4&from=paste&height=760&id=uadbb4435&originHeight=836&originWidth=1080&originalType=binary&ratio=1.100000023841858&rotation=0&showTitle=false&size=1472584&status=done&style=none&taskId=uf300c367-b953-46ac-a7e8-ec6281b91d0&title=&width=981.8181605378463)
