/**
* @file SlateRenderer.cpp.
* @brief The SlateRenderer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "SlateRenderer.h"
#include "Systems/SlateSystem.h"
#include "Debugger/Perf/NsightPerf.h"

namespace Spices {

	void SlateRenderer::CreateRendererPass()
	{
		SPICES_PROFILE_ZONE;

		RendererPassBuilder{ "Slate", this }
		.AddSubPass("Slate")
		.AddSwapChainAttachment([](VkAttachmentDescription& description) {
			description.initialLayout                 = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp                        = VK_ATTACHMENT_LOAD_OP_CLEAR;
		})
		.EndSubPass()
		.Build();
	}

	void SlateRenderer::CreateDescriptorSet()
	{}

	void SlateRenderer::OnSystemInitialize()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Call Parent Function.
		*/
		Renderer::OnSystemInitialize();

		/**
		* @breif Init ImGui.
		*/
		InitImgui();
	}

	void SlateRenderer::OnWindowResizeOver()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Just call OnSlateResize.
		*/
		Renderer::OnSlateResize();
	}

	void SlateRenderer::InitImgui()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Context, a concept of OpenGL, means OpenGL objects in used during a render stage.
		* Likes VkInstance.
		*/
		ImGui::CreateContext();

		/**
		* @breif Init ImPlot Library's Context.
		*/
		ImPlot::CreateContext();

		/**
		* @breif Configuration of ImGui.
		* Can Set custom UI style here.
		*/
		ImGuiIO& io = ImGui::GetIO();

		/**
		* @brief ImGui Slate Layout cache file.
		* Set nullptr, if not need.
		*/
		io.IniFilename = "SlateLayout.ini";
		io.LogFilename = nullptr;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;    // Enable Multi-Viewport / Platform Windows

		ImGuiH::SetStyle();
		ImGuiH::SetFonts();

		/**
		* @brief Setup Platform / Renderer backends.
		*/
		ImGui_ImplGlfw_InitForVulkan(m_VulkanState.m_Windows, true);

		/**
		* @brief Instance a ImGui_ImplVulkan_InitInfo.
		*/
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance                  = m_VulkanState.m_Instance;
		init_info.PhysicalDevice            = m_VulkanState.m_PhysicalDevice;
		init_info.Device                    = m_VulkanState.m_Device;
		init_info.QueueFamily               = m_VulkanState.m_GraphicQueueFamily;
		init_info.Queue                     = m_VulkanState.m_GraphicQueue;
		init_info.PipelineCache             = VK_NULL_HANDLE;
		init_info.DescriptorPool            = m_DescriptorPool->GetPool();
		init_info.RenderPass                = m_Pass->Get();
		init_info.Subpass                   = 0;
		init_info.MinImageCount             = MaxFrameInFlight;
		init_info.ImageCount                = MaxFrameInFlight;
		init_info.MSAASamples               = VK_SAMPLE_COUNT_1_BIT;
		init_info.Allocator                 = VK_NULL_HANDLE;
		init_info.CheckVkResultFn           = VK_NULL_HANDLE;

		/**
		* @brief ImGui Init for Vulkan.
		*/
		ImGui_ImplVulkan_Init(&init_info);

		/**
		* @brief Create IconFont.
		*/
		ImGui_ImplVulkan_CreateFontsTexture();

		/**
		* @brief Init NsightPerf HUD Renderer.
		*/
		PERF_INITHUDRENDERER;

		vkDeviceWaitIdle(m_VulkanState.m_Device);
	}

	void SlateRenderer::ShutdownImgui()
	{
		SPICES_PROFILE_ZONE;

		PERFREPORT_RESET(m_VulkanState)
		PERF_RESET

		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImPlot::DestroyContext();
		ImGui::DestroyContext();
	}

	void SlateRenderer::BeginImguiFrame()
	{
		SPICES_PROFILE_ZONE;

		{
			SPICES_PROFILE_ZONEN("ImGui_ImplVulkan_NewFrame");

			ImGui_ImplVulkan_NewFrame();
		}

		{
			SPICES_PROFILE_ZONEN("ImGui_ImplGlfw_NewFrame");

			ImGui_ImplGlfw_NewFrame();
		}

		{
			SPICES_PROFILE_ZONEN("ImGui::NewFrame");

			ImGui::NewFrame();
			ImGuiH::MainDockSpace();
		}

		{
			SPICES_PROFILE_ZONEN("ImGuizmo::BeginFrame");

			ImGuizmo::BeginFrame();
		}

		{
			SPICES_PROFILE_ZONEN("NvPerf::Consume");

			PERF_FRAMECONSUME
		}
	}

	void SlateRenderer::EndImguiFrame(uint32_t index)
	{
		SPICES_PROFILE_ZONE;

		const ImGuiIO& io = ImGui::GetIO();

		ImGui::Render();
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), m_VulkanState.m_GraphicCommandBuffer[index]);

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void SlateRenderer::EndImguiFrameAsync(uint32_t index)
	{
		SPICES_PROFILE_ZONE;

		const ImGuiIO& io = ImGui::GetIO();

		ImGui::Render();

		SubmitCmdsParallel(m_VulkanState.m_GraphicCommandBuffer[index], 0, [&](VkCommandBuffer cmdBuffer) {
			ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmdBuffer);
		});

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void SlateRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		RenderBehaveBuilder builder{ this, frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.BeginRenderPass();

		BeginImguiFrame();

		SlateSystem::GetRegister()->OnRender();

		EndImguiFrame(frameInfo.m_FrameIndex);
		builder.EndRenderPass();
	}
}