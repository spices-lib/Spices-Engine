/**
* @file SlateRenderer.cpp.
* @brief The SlateRenderer Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "SlateRenderer.h"

#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>

namespace Spiecs {

	struct PushConstant
	{
		/**
		* @brief Meshpack ModelMatrix.
		*/
		glm::mat4 model = glm::mat4(1.0f);

		/**
		* @brief Entityid, cast from entt::entity.
		*/
		int entityID = -1;

		/**
		* @brief Meshpackid, from arrayindex of meshpack.
		*/
		int meshpackID = -1;
	};

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
		});

		/**
		* @brief Add Depth Attachment.
		*/
		m_RenderPass->AddDepthAttachment([](VkAttachmentDescription& description) {
		});

		/**
		* @brief Create VkRenderPass, Resource, FrameBuffer.
		*/
		m_RenderPass->Build();
	}

	void SlateRenderer::CreatePipelineLayoutAndDescriptor()
	{
		PipelineLayoutBuilder{ this }
		.AddPushConstant<PushConstant>()
		.AddTexture<Texture2D>(0, 0, 0, VK_SHADER_STAGE_FRAGMENT_BIT)
		.Build();
	}

	void SlateRenderer::OnSystemInitialize()
	{
		Renderer::OnSystemInitialize();
		InitImgui();

		/*for (int i = 0; i < MaxFrameInFlight; i++)
		{
			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageView = m_VulkanState.m_SwapChainImageViews[i];
			imageInfo.sampler = m_VulkanState.m_SwapChainImageSamplers[i];
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			m_DescriptorSet[i] = VulkanImage::CreateDescriptorSet(0, 0, m_VulkanState, imageInfo);
		}*/


		m_TestImage = ResourcePool<Texture>::Load<Texture2D>("alexander.jpg");
		m_TestImage->GetResource<VulkanImage>()->CreateDescriptorSet(1, 0);

		m_DescriptorSet[0] = m_TestImage->GetResource<VulkanImage>()->GetDescriptorSet();
	}

	void SlateRenderer::InitImgui()
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

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
		ImGui::Render();
		ImDrawData* main_draw_data = ImGui::GetDrawData();
		ImGui_ImplVulkan_RenderDrawData(main_draw_data, m_VulkanState.m_CommandBuffer[index]);
	}

	void SlateRenderer::Render(FrameInfo& frameInfo)
	{
		RenderBehaverBuilder builder{ this ,frameInfo.m_FrameIndex };
		BeginImguiFrame();
		
		ImGui::ShowDemoWindow();

		{
			ImGui::Begin("Viewport");
			ImGui::Image(&m_DescriptorSet[0], ImVec2(100.0f, 100.0f));
			ImGui::End();
		}

		EndImguiFrame(frameInfo.m_FrameIndex);
		builder.EndRenderPass();
	}

}