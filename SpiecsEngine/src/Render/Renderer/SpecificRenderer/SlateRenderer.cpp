/**
* @file SlateRenderer.cpp.
* @brief The SlateRenderer Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "SlateRenderer.h"
#include "Systems/SlateSystem.h"

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
		// UI

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
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;    // Enable Multi-Viewport / Platform Windows

		ImGuiH::SetStyle();
		ImGuiH::SetFonts();

		// Setup Platform/Renderer backends

		ImGui_ImplGlfw_InitForVulkan(m_VulkanState.m_Windows, true);

		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance                 = m_VulkanState.m_Instance;
		init_info.PhysicalDevice           = m_VulkanState.m_PhysicalDevice;
		init_info.Device                   = m_VulkanState.m_Device;
		init_info.QueueFamily              = m_VulkanState.m_GraphicQueueFamily;
		init_info.Queue                    = m_VulkanState.m_GraphicQueue;
		init_info.PipelineCache            = VK_NULL_HANDLE;
		init_info.DescriptorPool           = m_DesctiptorPool->GetPool();
		init_info.RenderPass               = m_RenderPass->Get();
		init_info.Subpass                  = 0;
		init_info.MinImageCount            = 2;
		init_info.ImageCount               = MaxFrameInFlight;
		init_info.MSAASamples              = VK_SAMPLE_COUNT_1_BIT;
		init_info.Allocator                = VK_NULL_HANDLE;
		init_info.CheckVkResultFn          = VK_NULL_HANDLE;
		ImGui_ImplVulkan_Init(&init_info);

		ImGui_ImplVulkan_CreateFontsTexture();
		vkDeviceWaitIdle(m_VulkanState.m_Device);
	}

	void SlateRenderer::ShutdownImgui()
	{
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImPlot::DestroyContext();
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
		RenderBehaverBuilder builder{ this, frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		BeginImguiFrame();

		SlateSystem::GetRegister()->OnRender();

		EndImguiFrame(frameInfo.m_FrameIndex);
		builder.EndRenderPass();
	}

}