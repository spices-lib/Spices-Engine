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

		/**
		* @brief Fill in format.
		*/
		attachmentDescription.format = m_VulkanDevice->GetSwapChainSupport().format.format;

		/**
		* @brief Fill in samples.
		*/
		attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;

		/**
		* @brief Fill in loadOp.
		* @note Init With VK_ATTACHMENT_LOAD_OP_LOAD.
		* Set VK_ATTACHMENT_LOAD_OP_CLEAR for the first renderpass.
		*/
		attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;

		/**
		* @brief Fill in storeOp.
		* @note Just set VK_ATTACHMENT_STORE_OP_STORE always.
		* Otherwise you will get a grid present image, which is different from swpachainimage shows,
		* for vulkan has changed it's layout.
		*/
		attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

		/**
		* @brief Fill in stencilLoadOp.
		*/
		attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;

		/**
		* @brief Fill in stencilStoreOp.
		*/
		attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

		/**
		* @brief Fill in initialLayout.
		* @note Init With VK_IMAGE_LAYOUT_PRESENT_SRC_KHR.
		* Set VK_IMAGE_LAYOUT_UNDEFINED for the first renderpass.
		*/
		attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		/**
		* @brief Fill in finalLayout.
		*/
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

		/**
		* @brief Fill in attachment.
		*/
		attachmentRef.attachment = (uint32_t)m_ColorAttachmentRef.size();

		/**
		* @brief Fill in layout.
		*/
		attachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		
		/**
		* @brief Instanced a VkClearValue with default value.
		*/
		VkClearValue clearValue{};

		/**
		* @brief Fill in color.
		*/
		clearValue.color = { 0.1f, 0.1f, 0.1f, 1.0f };

		/**
		* @brief Fill in variable.
		*/
		m_ColorAttachmentRef.push_back(std::move(attachmentRef));
		m_Attachments.push_back(std::move(attachmentDescription));
		m_ClearValues.push_back(std::move(clearValue));

		/**
		* @brief Instanced a VkPipelineColorBlendAttachmentState with default value.
		*/
		VkPipelineColorBlendAttachmentState colorBlend{};

		/**
		* @brief Fill in colorWriteMask.
		*/
		colorBlend.colorWriteMask =
			VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
			VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

		/**
		* @brief Fill in blendEnable.
		*/
		colorBlend.blendEnable = VK_FALSE;

		/**
		* @brief Fill in srcColorBlendFactor.
		*/
		colorBlend.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;

		/**
		* @brief Fill in dstColorBlendFactor.
		*/
		colorBlend.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;

		/**
		* @brief Fill in colorBlendOp.
		*/
		colorBlend.colorBlendOp = VK_BLEND_OP_ADD;

		/**
		* @brief Fill in srcAlphaBlendFactor.
		*/
		colorBlend.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;

		/**
		* @brief Fill in dstAlphaBlendFactor.
		*/
		colorBlend.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;

		/**
		* @brief Fill in alphaBlendOp.
		*/
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

		/**
		* @brief Fill in format.
		*/
		attachmentDescription.format = m_VulkanDevice->GetSwapChainSupport().format.format;

		/**
		* @brief Fill in samples.
		*/
		attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;

		/**
		* @brief Fill in loadOp.
		* @note Init With VK_ATTACHMENT_LOAD_OP_LOAD.
		* Set VK_ATTACHMENT_LOAD_OP_CLEAR for the first renderpass.
		*/
		attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;

		/**
		* @brief Fill in storeOp.
		*/
		attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

		/**
		* @brief Fill in stencilLoadOp.
		*/
		attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;

		/**
		* @brief Fill in stencilStoreOp.
		*/
		attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

		/**
		* @brief Fill in initialLayout.
		* @note Init With VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL.
		* Set VK_IMAGE_LAYOUT_UNDEFINED for the first renderpass.
		*/
		attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		/**
		* @brief Fill in finalLayout.
		*/
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

		/**
		* @brief Fill in attachment.
		*/
		attachmentRef.attachment = (uint32_t)m_ColorAttachmentRef.size();

		/**
		* @brief Fill in layout.
		*/
		attachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		/**
		* @brief Instanced a VkClearValue with default value.
		*/
		VkClearValue clearValue{};

		/**
		* @brief Fill in color.
		*/
		clearValue.color = { 0.1f, 0.1f, 0.1f, 1.0f };

		/**
		* @brief Fill in variable.
		*/
		m_ColorAttachmentRef.push_back(std::move(attachmentRef));
		m_Attachments.push_back(std::move(attachmentDescription));
		m_ClearValues.push_back(std::move(clearValue));

		/**
		* @brief Set View.
		*/
		RendererResourceCreateInfo Info;
		Info.description = attachmentDescription;
		Info.width = m_VulkanDevice->GetSwapChainSupport().extent.width;
		Info.height = m_VulkanDevice->GetSwapChainSupport().extent.height;

		m_AttachmentsView.push_back(m_RendererResourcePool->AccessResource(attachmentName, Info)->imageView);

		/**
		* @brief Instanced a VkPipelineColorBlendAttachmentState with default value.
		*/
		VkPipelineColorBlendAttachmentState colorBlend{};

		/**
		* @brief Fill in colorWriteMask.
		*/
		colorBlend.colorWriteMask =
			VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
			VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

		/**
		* @brief Fill in blendEnable.
		*/
		colorBlend.blendEnable = VK_FALSE;

		/**
		* @brief Fill in srcColorBlendFactor.
		*/
		colorBlend.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;

		/**
		* @brief Fill in dstColorBlendFactor.
		*/
		colorBlend.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;

		/**
		* @brief Fill in colorBlendOp.
		*/
		colorBlend.colorBlendOp = VK_BLEND_OP_ADD;

		/**
		* @brief Fill in srcAlphaBlendFactor.
		*/
		colorBlend.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;

		/**
		* @brief Fill in dstAlphaBlendFactor.
		*/
		colorBlend.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;

		/**
		* @brief Fill in alphaBlendOp.
		*/
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

		/**
		* @brief Fill in format.
		*/
		depthAttachment.format = VulkanSwapChain::FindDepthFormat(m_VulkanState.m_PhysicalDevice);

		/**
		* @brief Fill in samples.
		*/
		depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

		/**
		* @brief Fill in loadOp.
		* @note Init With VK_ATTACHMENT_LOAD_OP_LOAD.
		* Set VK_ATTACHMENT_LOAD_OP_CLEAR for the first renderpass.
		*/
		depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;

		/**
		* @brief Fill in storeOp.
		*/
		depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

		/**
		* @brief Fill in stencilLoadOp.
		*/
		depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;

		/**
		* @brief Fill in stencilStoreOp.
		*/
		depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

		/**
		* @brief Fill in initialLayout.
		* @note Init With VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL.
		* Set VK_IMAGE_LAYOUT_UNDEFINED for the first renderpass.
		*/
		depthAttachment.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		/**
		* @brief Fill in finalLayout.
		*/
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

		/**
		* @brief Fill in attachment.
		*/
		depthAttachmentRef.attachment = (uint32_t)m_ColorAttachmentRef.size();

		/**
		* @brief Fill in layout.
		*/
		depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		/**
		* @brief Instanced a VkClearValue with default value.
		*/
		VkClearValue clearValue{};

		/**
		* @brief Fill in depthStencil.
		*/
		clearValue.depthStencil = { 1.0f, 0 };
		
		/**
		* @brief Fill in variable.
		*/
		m_DepthAttachmentRef = depthAttachmentRef;
		m_Attachments.push_back(std::move(depthAttachment));
		m_ClearValues.push_back(std::move(clearValue));

		/**
		* @brief Set View.
		*/
		RendererResourceCreateInfo Info;
		Info.description = depthAttachment;
		Info.width = m_VulkanDevice->GetSwapChainSupport().extent.width;
		Info.height = m_VulkanDevice->GetSwapChainSupport().extent.height;
		Info.isDepthResource = true;

		m_AttachmentsView.push_back(m_RendererResourcePool->AccessDepthResource(Info)->imageView);

		isUseDpth = true;
	}
}