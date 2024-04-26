/**
* @file SlateRenderer.cpp.
* @brief The SlateRenderer Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "SlateRenderer.h"
#include "Systems/SlateSystem.h"

#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>

namespace Spiecs {

	void SlateRenderer::CreateRenderPass()
	{
		/**
		* @brief Declear an empty VulkanRenderPass Object.
		*/
		m_RenderPass = std::make_unique<VulkanRenderPass>(m_VulkanState, m_Device, m_RendererResourcePool);

		/**
		* @brief Add SwapChian Attachment.
		*/
		m_RenderPass->AddSwapChainAttachment([](VkAttachmentDescription& description) {
			description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		});

		/**
		* @brief Create VkRenderPass, Resource, FrameBuffer.
		*/
		m_RenderPass->Build();
	}

	void SlateRenderer::CreatePipelineLayoutAndDescriptor()
	{}

	void SlateRenderer::OnSystemInitialize()
	{
		Renderer::OnSystemInitialize();
		InitImgui();
	}

	void SlateRenderer::InitImgui()
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer backends

		ImGui_ImplGlfw_InitForVulkan(m_VulkanState.m_Windows, true);
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = m_VulkanState.m_Instance;
		init_info.PhysicalDevice = m_VulkanState.m_PhysicalDevice;
		init_info.Device = m_VulkanState.m_Device;
		init_info.QueueFamily = m_VulkanState.m_GraphicQueueFamily;
		init_info.Queue = m_VulkanState.m_GraphicQueue;
		init_info.PipelineCache = VK_NULL_HANDLE;
		init_info.DescriptorPool = m_DesctiptorPool->GetPool();
		init_info.RenderPass = m_RenderPass->Get();
		init_info.Subpass = 0;
		init_info.MinImageCount = 2;
		init_info.ImageCount = MaxFrameInFlight;
		init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		init_info.Allocator = VK_NULL_HANDLE;
		init_info.CheckVkResultFn = VK_NULL_HANDLE;
		ImGui_ImplVulkan_Init(&init_info);

		ImGui_ImplVulkan_CreateFontsTexture();
		vkDeviceWaitIdle(m_VulkanState.m_Device);
	}

	void SlateRenderer::ShutdownImgui()
	{
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void SlateRenderer::BeginImguiFrame()
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void SlateRenderer::EndImguiFrame(uint32_t index)
	{
		ImGuiIO& io = ImGui::GetIO();

		ImGui::Render();
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), m_VulkanState.m_CommandBuffer[index]);

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void SlateRenderer::Render(FrameInfo& frameInfo)
	{
		RenderBehaverBuilder builder{ this, frameInfo.m_FrameIndex };

		BeginImguiFrame();

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::Begin("DockSpace Demo", nullptr, window_flags);
		// Dockspace
		ImGui::DockSpace(ImGui::GetID("MyDockspace"));
		
		SlateSystem::GetRegister()->OnRender();

		ImGui::End();

		EndImguiFrame(frameInfo.m_FrameIndex);
		builder.EndRenderPass();
	}

}