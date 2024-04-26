/**
* @file VulkanRenderPass.h.
* @brief The VulkanRenderPass Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include "Render/RendererResource/RendererResourcePool.h"

namespace Spiecs {

	/**
	* @brief VulkanRenderPass Class.
	* This class defines the VulkanRenderPass behaver.
	* This class is just a wapper of VkRenderPass, VkFramebuffer.
	*/
	class VulkanRenderPass : public VulkanObject
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] vulkanState The global VulkanState.
		* @param[in] vulkanDevice The shared pointer of VulkanDevice.
		*/
		VulkanRenderPass(VulkanState& vulkanState, std::shared_ptr<VulkanDevice> vulkanDevice, std::shared_ptr<RendererResourcePool> rendererResourcePool);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanRenderPass();

		/**
		* @brief Get VkRenderPass.
		* @return Returns the VkRenderPass.
		*/
		inline VkRenderPass& Get() { return m_RenderPass; };

		/**
		* @brief Get Framebuffer by index.
		* @param[in] index Frameindex in FrameInfo.
		* @return Returns the indexed Framebuffer.
		*/
		inline VkFramebuffer& GetFramebuffer(uint32_t index) { return m_SwapChainFramebuffers[index]; };

		/**
		* @brief Get VkClearValue.
		* @return Returns the array of attachments's VkClearValue.
		*/
		inline std::vector<VkClearValue>& GetClearValues() { return m_ClearValues; };

		/**
		* @brief Get VkPipelineColorBlendAttachmentState.
		* @return Returns the array of VkPipelineColorBlendAttachmentState.
		*/
		inline std::vector<VkPipelineColorBlendAttachmentState>& GetColorBlend() { return m_ColorBlendAttachment; };

		/**
		* @brief Add the SwapChian's Image Attachment to RenderPass and FrameBuffer.
		* @param[in] T The function pointer of how VkAttachmentDescription is.
		*/
		template<typename T>
		void AddSwapChainAttachment(T func);

		/**
		* @brief Add the Custom Color Image Attachment to RenderPass and FrameBuffer.
		* @param[in] T The function pointer of how VkAttachmentDescription is.
		* @param[in] attachmentName The name of the Custom Color Image Attachment.
		*/
		template<typename T>
		void AddColorAttachment(const std::string& attachmentName, T func);

		/**
		* @brief Add the Depth Image Attachment to RenderPass and FrameBuffer.
		* @param[in] T The function pointer of how VkAttachmentDescription is.
		*/
		template<typename T>
		void AddDepthAttachment(T func);

		/**
		* @brief Add the Depth Image Attachment to RenderPass and FrameBuffer.
		* @param[in] T The function pointer of how VkAttachmentDescription is.
		*/
		template<typename T>
		void AddInputAttachment(const std::string& attachmentName, T func);

		/**
		* @brief Create RenderPss and FrameBuffer.
		*/
		void Build();

	private:

		/**
		* @brief The shared pointer of VulkanDevice.
		*/
		std::shared_ptr<VulkanDevice> m_VulkanDevice;

		std::shared_ptr<RendererResourcePool> m_RendererResourcePool;

		/**
		* @brief The RenderPass this class mainly manage.
		*/
		VkRenderPass m_RenderPass{};

		/**
		* @brief The array of VkAttachmentReference.
		*/
		std::vector<VkAttachmentReference> m_InputAttachmentRef;

		/**
		* @brief The array of VkAttachmentReference.
		*/
		std::vector<VkAttachmentReference> m_ColorAttachmentRef;

		/**
		* @brief The VkAttachmentReference of Depth Attachment.
		*/
		VkAttachmentReference m_DepthAttachmentRef{};

		/**
		* @brief The array of VkAttachmentDescription.
		*/
		std::vector<VkAttachmentDescription> m_Attachments;

		/**
		* @brief The array of VkClearValue.
		*/
		std::vector<VkClearValue> m_ClearValues{};

		/**
		* @brief True if we call AddSwapChainAttachment().
		*/
		bool IsUseSwapChianImage = false;

		/**
		* @brief True if we call AddDepthAttachment().
		*/
		bool isUseDpth = false;

		/**
		* @brief The array of VkImageView.
		*/
		std::vector<VkImageView> m_AttachmentsView;

		/**
		* @brief This variable is used in Creating Pipeline.
		*/
		std::vector<VkPipelineColorBlendAttachmentState> m_ColorBlendAttachment;

		/**
		* @brief The FrameBuffers.
		*/
		std::array<VkFramebuffer, MaxFrameInFlight> m_SwapChainFramebuffers{};
	};

	template<typename T>
	void VulkanRenderPass::AddSwapChainAttachment(T func)
	{
		/**
		* @brief Instanced a VkAttachmentDescription with default value.
		*/
		VkAttachmentDescription attachmentDescription{};
		attachmentDescription.format = m_VulkanDevice->GetSwapChainSupport().format.format;
		attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
		attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;

		/**
		* @note Just set VK_ATTACHMENT_STORE_OP_STORE always.
		* Otherwise you will get a grid present image, which is different from swpachainimage shows,
		* for vulkan has changed it's layout.
		*/
		attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

		/**
		* @note Init With VK_IMAGE_LAYOUT_PRESENT_SRC_KHR.
		* Set VK_IMAGE_LAYOUT_UNDEFINED for the first renderpass.
		*/
		attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		/**
		* @brief Rewrite VkAttachmentDescription.
		* @param[in] attachmentDescription VkAttachmentDescription Reference.
		*/
		func(attachmentDescription);

		/**
		* @brief Instanced a VkAttachmentReference with default value.
		*/
		VkAttachmentReference attachmentRef{};
		attachmentRef.attachment = (uint32_t)m_ColorAttachmentRef.size();
		attachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		VkClearValue clearValue{};
		clearValue.color = { 0.1f, 0.1f, 0.1f, 1.0f };

		/**
		* @brief Add to local variable.
		*/
		m_ColorAttachmentRef.push_back(std::move(attachmentRef));
		m_Attachments.push_back(std::move(attachmentDescription));
		m_ClearValues.push_back(std::move(clearValue));

		/**
		* @brief Instanced a VkPipelineColorBlendAttachmentState with default value.
		*/
		VkPipelineColorBlendAttachmentState colorBlend{};
		colorBlend.colorWriteMask =
			VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
			VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlend.blendEnable = VK_FALSE;
		colorBlend.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlend.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlend.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlend.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlend.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlend.alphaBlendOp = VK_BLEND_OP_ADD;

		/**
		* @brief Add to local variable.
		*/
		m_ColorBlendAttachment.push_back(colorBlend);

		/**
		* @brief Set use Swapchain Image.
		*/
		IsUseSwapChianImage = true;
	}

	template<typename T>
	void VulkanRenderPass::AddColorAttachment(const std::string& attachmentName, T func)
	{
		/**
		* @brief Instanced a VkAttachmentDescription with default value.
		*/
		VkAttachmentDescription attachmentDescription{};
		attachmentDescription.format = m_VulkanDevice->GetSwapChainSupport().format.format;
		attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
		attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
		attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		/**
		* @brief Rewrite VkAttachmentDescription.
		* @param[in] attachmentDescription VkAttachmentDescription Reference.
		*/
		func(attachmentDescription);

		/**
		* @brief Instanced a VkAttachmentReference with default value.
		*/
		VkAttachmentReference attachmentRef{};
		attachmentRef.attachment = (uint32_t)m_ColorAttachmentRef.size();
		attachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		VkClearValue clearValue{};
		clearValue.color = { 0.1f, 0.1f, 0.1f, 1.0f };

		/**
		* @brief Add to local variable.
		*/
		m_ColorAttachmentRef.push_back(std::move(attachmentRef));
		m_Attachments.push_back(std::move(attachmentDescription));
		m_ClearValues.push_back(std::move(clearValue));

		/**
		* @brief Set View.
		*/
		RendererResourceCreateInfo Info;
		Info.description = attachmentDescription;
		Info.width = m_VulkanDevice->GetSwapChainSupport().surfaceSize.width;
		Info.height = m_VulkanDevice->GetSwapChainSupport().surfaceSize.height;

		m_AttachmentsView.push_back(m_RendererResourcePool->AccessResource(attachmentName, Info)->imageView);

		/**
		* @brief Instanced a VkPipelineColorBlendAttachmentState with default value.
		*/
		VkPipelineColorBlendAttachmentState colorBlend{};
		colorBlend.colorWriteMask =
			VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
			VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlend.blendEnable = VK_FALSE;
		colorBlend.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlend.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlend.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlend.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlend.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlend.alphaBlendOp = VK_BLEND_OP_ADD;

		/**
		* @brief Add to local variable.
		*/
		m_ColorBlendAttachment.push_back(colorBlend);
	}

	template<typename T>
	void VulkanRenderPass::AddDepthAttachment(T func)
	{
		/**
		* @brief Instanced a VkAttachmentDescription with default value.
		*/
		VkAttachmentDescription depthAttachment{};
		depthAttachment.format = VulkanSwapChain::FindDepthFormat(m_VulkanState.m_PhysicalDevice);
		depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

		/**
		* @note Init With VK_ATTACHMENT_LOAD_OP_LOAD.
		* Set VK_ATTACHMENT_LOAD_OP_CLEAR for the first renderpass.
		*/
		depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
		depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

		/**
		* @note Init With VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL.
		* Set VK_IMAGE_LAYOUT_UNDEFINED for the first renderpass.
		*/
		depthAttachment.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		/**
		* @brief Rewrite VkAttachmentDescription.
		* @param[in] attachmentDescription VkAttachmentDescription Reference.
		*/
		func(depthAttachment);

		/**
		* @brief Instanced a VkAttachmentReference with default value.
		*/
		VkAttachmentReference depthAttachmentRef{};
		depthAttachmentRef.attachment = (uint32_t)m_ColorAttachmentRef.size();
		depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		/**
		* @brief Instanced a VkClearValue with default value.
		*/
		VkClearValue clearValue{};
		clearValue.depthStencil = { 1.0f, 0 };
		
		/**
		* @brief Add to local variable.
		*/
		m_DepthAttachmentRef = depthAttachmentRef;
		m_Attachments.push_back(std::move(depthAttachment));
		m_ClearValues.push_back(std::move(clearValue));

		/**
		* @brief Set View.
		*/
		RendererResourceCreateInfo Info;
		Info.description = depthAttachment;
		Info.width = m_VulkanDevice->GetSwapChainSupport().surfaceSize.width;
		Info.height = m_VulkanDevice->GetSwapChainSupport().surfaceSize.height;
		Info.isDepthResource = true;

		/**
		* @brief Add to local variable.
		*/
		m_AttachmentsView.push_back(m_RendererResourcePool->AccessDepthResource(Info)->imageView);

		/**
		* @brief Set use Depth.
		*/
		isUseDpth = true;
	}

	template<typename T>
	inline void VulkanRenderPass::AddInputAttachment(const std::string& attachmentName, T func)
	{
		/**
		* @brief Instanced a VkAttachmentDescription with default value.
		*/
		VkAttachmentDescription attachmentDescription{};
		attachmentDescription.format = m_VulkanDevice->GetSwapChainSupport().format.format;
		attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
		attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
		attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		/**
		* @brief Rewrite VkAttachmentDescription.
		* @param[in] attachmentDescription VkAttachmentDescription Reference.
		*/
		func(attachmentDescription);

		/**
		* @brief Instanced a VkAttachmentReference with default value.
		*/
		VkAttachmentReference attachmentRef{};
		attachmentRef.attachment = (uint32_t)m_InputAttachmentRef.size();
		attachmentRef.layout = attachmentDescription.finalLayout;
		VkClearValue clearValue{};
		clearValue.color = { 0.0f, 0.0f, 0.0f, 1.0f };

		/**
		* @brief Add to local variable.
		*/
		m_InputAttachmentRef.push_back(std::move(attachmentRef));
		m_Attachments.push_back(std::move(attachmentDescription));
		m_ClearValues.push_back(std::move(clearValue));

		/**
		* @brief Set View.
		*/
		RendererResourceCreateInfo Info;
		Info.description = attachmentDescription;
		Info.width = m_VulkanDevice->GetSwapChainSupport().surfaceSize.width;
		Info.height = m_VulkanDevice->GetSwapChainSupport().surfaceSize.height;

		m_AttachmentsView.push_back(m_RendererResourcePool->AccessResource(attachmentName, Info)->imageView);
	}
}