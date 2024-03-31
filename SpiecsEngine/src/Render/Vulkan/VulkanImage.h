#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

namespace Spiecs {

	class VulkanImage : public VulkanObject
	{
	public:
		VulkanImage(VulkanState& vulkanState) : VulkanObject(vulkanState) {};
		VulkanImage(VulkanState& vulkanState, const std::string& filePath);
		VulkanImage(VulkanState& vulkanState, uint32_t width, uint32_t height, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, uint32_t mipLevels);
		virtual ~VulkanImage();

		VkImageView& GetView() { return m_ImageView; };

	public:
		void TransitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		void GenerateMipmaps(VkFormat imageFormat, int32_t texWidth, int32_t texHeight);
		void CreateImageView(VkFormat format, VkImageAspectFlags aspectFlags);
		void CreateSampler();
		void CreateImage(VulkanState& vulkanState, uint32_t width, uint32_t height, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, uint32_t mipLevels);

	private:
		int m_Width;
		int m_Height;
		int m_Layers = 1;

		uint32_t m_MipLevels = 1;
		VkImage m_Image;
		VkDeviceMemory m_ImageMemory;
		VkImageView m_ImageView;
		VkSampler m_TextureSampler;
	};
}