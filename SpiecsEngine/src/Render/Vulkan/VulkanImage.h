#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

#include <any>

namespace Spiecs {

	class VulkanImage : public VulkanObject
	{
	public:
		VulkanImage(VulkanState& vulkanState) : VulkanObject(vulkanState) {};
		VulkanImage(VulkanState& vulkanState, const std::string& filePath);
		VulkanImage(VulkanState& vulkanState, uint32_t width, uint32_t height, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, uint32_t mipLevels);
		virtual ~VulkanImage();

		VkImageView& GetView() { return m_ImageView; };
		VkImage& GetImage() { return m_Image; };

	public:
		VkDescriptorImageInfo* GetImageInfo(VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		void TransitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		void CopyImageTexelToBuffer(uint32_t x, uint32_t y, std::any out_rgba);
		void GenerateMipmaps(VkFormat imageFormat, int32_t texWidth, int32_t texHeight);
		void CreateImageView(VkFormat format, VkImageAspectFlags aspectFlags);
		void CreateSampler();
		void CreateImage(VulkanState& vulkanState, uint32_t width, uint32_t height, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, uint32_t mipLevels);
		void CreateDescriptorSet(uint32_t set, uint32_t binding);
		VkDescriptorSet& GetDescriptorSet() { return m_DescriptorSet; };

		static VkDescriptorSet CreateDescriptorSet(uint32_t set, uint32_t binding, VulkanState& vulkanState, VkDescriptorImageInfo imageInfo);

	private:
		int m_Width = 0;
		int m_Height = 0;
		int m_Layers = 1;

		uint32_t m_MipLevels = 1;
		VkImage m_Image{};
		VkDeviceMemory m_ImageMemory{};
		VkImageView m_ImageView{};
		VkSampler m_TextureSampler{};

		VkDescriptorImageInfo m_ImageInfo{};
		VkDescriptorSet m_DescriptorSet{};

		VkFormat m_Format;

		friend class TextureLoader;
	};
}