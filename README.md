# **Project Introduction**

---

**Project Name:** Spices Engine
**Start Date**: March 27, 2024
**Project Purpose**:
The primary objective of the Spices Engine project is to validate and deepen my understanding and proficiency in several advanced fields of graphics programming and engine development, specifically Vulkan, engine development principles, C++, rendering pipelines, ray tracing, and shader development. Prior to writing the first line of code, I underwent a rigorous six-month period of study to ensure I had the necessary knowledge to successfully complete this project.
**Project Features and Implementation**:

1. **Premake Auto Build**
2. **Vulkan Render backend**
3. **Rasterization Pipeline**
4. **Ray Tracing Pipeline**
5. **Physically Based Rendering (PBR) **
6. **Entity Component System (ECS)**
7. **Log System**
8. **Documentation**
9. **Rendering pipeline analysis**
10. **Unit Test**
11. **Git Version Control**
12. **User Interface(Imgui)**

**Project Todo:**

1. **Bi-Directional Path Tracing**
2. **Houdini LiveLink**

**Device:**

1. **NVIDIA GeForce RTX 4060**
# Project Looks Like that

---

![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721468942734-334a66bf-5482-4f72-8b4e-ff24bea2792a.png#averageHue=%232e5250&clientId=u3caa4349-6f5a-4&from=paste&height=927&id=u86621213&originHeight=1391&originWidth=2560&originalType=binary&ratio=1&rotation=0&showTitle=false&size=3576004&status=done&style=none&taskId=u43d13339-3209-4388-95a4-d9688efcf63&title=&width=1706.6666666666667)
# Git Desktop Clone

---

```bash
https://github.com/spices-lib/Spices-Engine.git
```

- **SDK dependency**
   - [Vulkan SDK 1.3.275.0](https://vulkan.lunarg.com/)
- **Submodule dependency**
   - [GLFW @ 9bed794](https://github.com/TheCherno/glfw/tree/9bed794ab7c1b961aaca259403695bbd3870d3b3)
   - [IconFontCppHeaders @ f30b1e7](https://github.com/juliettef/IconFontCppHeaders/tree/f30b1e73b2d71eb331d77619c3f1de34199afc38)
   - [ImGuizmo @ ba662b1](https://github.com/CedricGuillemet/ImGuizmo/tree/ba662b119d64f9ab700bb2cd7b2781f9044f5565)
   - [glm @ ee7e155](https://github.com/spices-lib/glm/tree/ee7e155649041fd83920fdd045d28d0d5310a70d)
   - [imgui @ 8d88308](https://github.com/spices-lib/imgui/tree/8d883081f68aea723aa4de7cc28c9511b55f9883)
   - [implot @ b6fc717](https://github.com/spices-lib/implot/tree/b6fc71745b8471e2fda6f17dd40586145269971a)
   - [spdlog @ d276069](https://github.com/gabime/spdlog/tree/d276069a6e916b1e1fd45885b15b72bd8ee000a7)
   - [tracy @ 521e371](https://github.com/wolfpld/tracy/tree/521e371c60e37b47ffb8b0330d310af7061e0af8)
   - [yaml-cpp @ ac5a1ca](https://github.com/spices-lib/yaml-cpp/tree/ac5a1ca552659ea1737db862f4037226dc11b7d4)
   - [googletest @ 6a6515b](https://github.com/spices-lib/googletest/tree/6a6515b8afbddcdc7bc6c1133256e153f39d6e03)
# Premake Auto Build

---

Double click  **Startup.bat**  and it will generate visual studio 2022 project by all **premake5.lua **files this project automatically.
These files determine the building of the project：

- **lib.lua**
- **premake5.lua**
- **SpicesEngine/premake5.lua**
- **SpicesGame/premake5.lua**
- **SpicesTest/premake5.lua**
- **SpicesEngine/vendor/GLFW/premake5.lua**
- **SpicesEngine/vendor/imgui/premake5.lua**
- **SpicesEngine/vendor/implot/premake5.lua**
- **SpicesEngine/vendor/yaml-cpp/premake5.lua**
- **SpicesTest/vendor/googletest/premake5.lua**
# Vulkan Render backend

---

- **Deferred Rendering Pipeline**

# Rasterization Pipeline

---

- **Scene screenshot**

![SwapChainImage.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721471601026-0add88cf-38b0-45ac-b818-d70de73ee049.png#averageHue=%23475924&clientId=u7a1e0cda-1053-4&from=drop&id=u90ca8fc9&originHeight=1300&originWidth=2200&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=412682&status=done&style=none&taskId=ud47f99ad-8d47-4540-8925-e7416419c49&title=)

- **Nsight screenshot**

![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721471634889-f4f303b5-c875-47d5-a7c5-e8b9c546cdae.png#averageHue=%23d68741&clientId=u7a1e0cda-1053-4&from=paste&height=295&id=u3dcdbb0e&originHeight=369&originWidth=2181&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=48638&status=done&style=none&taskId=ua414e5b7-a791-408e-825b-3d82550b86b&title=&width=1744.8)
![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721471798707-dffed65d-3c01-4a1b-9e3a-975b398fbc17.png#averageHue=%23343433&clientId=u7a1e0cda-1053-4&from=paste&height=699&id=u92d68e44&originHeight=874&originWidth=1476&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=94193&status=done&style=none&taskId=ud68c5d18-323f-421c-b719-f69acc46549&title=&width=1180.8)

- **c++ renderer code sample**

file in:  SpicesEngine/src/Render/Renderer/SpecificRenderer/BasePassRenderer.cpp
![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721471999802-48a549c7-b2bb-4881-86e8-dd524f13218f.png#averageHue=%23282727&clientId=u7a1e0cda-1053-4&from=paste&height=909&id=u7a2b109d&originHeight=1136&originWidth=1185&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=248199&status=done&style=none&taskId=ub87edc5f-9554-4ee0-b471-f793050480c&title=&width=948)
![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721472055403-6b4f3c0e-627a-4571-8e22-deef725b402a.png#averageHue=%23272626&clientId=u7a1e0cda-1053-4&from=paste&height=975&id=u5294526d&originHeight=1219&originWidth=1204&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=181096&status=done&style=none&taskId=u05993ca7-190d-4902-8bdd-4f6592223ba&title=&width=963.2)

- **shader code sample**

file in**:  **SpicesEngine/assets/Shaders/src/Shader.BasePassRenderer.Mesh.Default.vert
![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721472223309-77d85a29-7367-4d41-8f99-87a441568d14.png#averageHue=%23262626&clientId=u7a1e0cda-1053-4&from=paste&height=1026&id=ucc90eba8&originHeight=1282&originWidth=1027&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=125907&status=done&style=none&taskId=u89f3b53d-ae3d-4186-a915-8cb3d2813e6&title=&width=821.6)
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

file in:  SpicesEngine/src/Render/Renderer/SpecificRenderer/RayTracingRenderer.cpp
![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721473200196-6ad4e69e-6566-48c4-a2a4-ba597b6d6c02.png#averageHue=%23282726&clientId=u7a1e0cda-1053-4&from=paste&height=274&id=u231a3f51&originHeight=342&originWidth=1609&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=120175&status=done&style=none&taskId=ub6b7c628-3d20-4880-bf20-8a6fcfbfc7c&title=&width=1287.2)
![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721473221155-f8820f5d-dc24-4f6c-9c5f-09e03fc8673f.png#averageHue=%23272626&clientId=u7a1e0cda-1053-4&from=paste&height=727&id=u73e8dc90&originHeight=909&originWidth=1260&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=137881&status=done&style=none&taskId=u8068c5d2-c39a-4b9e-95d1-31ea38209fa&title=&width=1008)

- **shader code sample**

file in**:  **SpicesEngine/assets/Shaders/src/Header/ShaderClosestHit.glsl
![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721473325987-94759c37-ae33-4c27-b694-3a897fd60d3b.png#averageHue=%23282727&clientId=u7a1e0cda-1053-4&from=paste&height=967&id=u7a5a4309&originHeight=1064&originWidth=1020&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=126405&status=done&style=none&taskId=u14de1198-ed52-4036-b24b-823cd225c7b&title=&width=927.2727071746326)
# Physically Based Rendering (PBR) 

---

- **BRDF diffuse**

![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721473825047-6fdca373-fb26-4784-993e-1b211bcb2825.png#averageHue=%2353657d&clientId=u7a1e0cda-1053-4&from=paste&height=805&id=u7e610bbe&originHeight=1006&originWidth=1994&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=3347087&status=done&style=none&taskId=u76baa548-1ec8-47a1-b75b-62e71df6e5f&title=&width=1595.2)

- **BRDF specular - Normal distribution + Geometric shadowing term + Fresnel**

![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721474171005-c3b28cb0-8f2a-4716-8f19-4083e66358a8.png#averageHue=%23030300&clientId=u7a1e0cda-1053-4&from=paste&height=807&id=u1321c94c&originHeight=1009&originWidth=1986&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=760964&status=done&style=none&taskId=u09a6eaca-550a-454a-b76e-06b9770e679&title=&width=1588.8)

- **BRDF diffuse + specular**

![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721474313751-c45949c5-0711-473f-b945-78c5df42acb6.png#averageHue=%234d6984&clientId=u7a1e0cda-1053-4&from=paste&height=808&id=ua3cb8af2&originHeight=1010&originWidth=1995&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=3878116&status=done&style=none&taskId=u74090082-8037-45f9-93b0-56c862e7d10&title=&width=1596)

- **PBR Shader code sample**

file in**:  **SpicesEngine/assets/Shaders/src/Header/ShaderFunctionLibrary.glsl
![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721474431169-0c8a1f41-69d8-40c0-9119-f66345609643.png#averageHue=%23272626&clientId=u7a1e0cda-1053-4&from=paste&height=798&id=u7a6e78bb&originHeight=997&originWidth=1219&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=117018&status=done&style=none&taskId=ucbe3bb33-11d1-4a4c-a4f3-cd69ac44beb&title=&width=975.2)

- **Material sample**

file in**:  **SpicesEngine/assets/Materials/src/Material.BasePassRenderer.Mesh.Default.material
![image.png](https://cdn.nlark.com/yuque/0/2024/png/2894467/1721474579331-d98a15c3-23da-4e84-bb6b-8caa6745f400.png#averageHue=%23272727&clientId=u7a1e0cda-1053-4&from=paste&height=907&id=uddba999b&originHeight=1134&originWidth=1926&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=280073&status=done&style=none&taskId=ufe655883-f247-4c5b-a243-f41fdaae7bc&title=&width=1540.8)
# Entity Component System (ECS)

---


# Log System

---


# Documentation

---


# Rendering pipeline analysis

---


# Unit Test

---


# Git Version Control

---


# User Interface(Imgui)

---

