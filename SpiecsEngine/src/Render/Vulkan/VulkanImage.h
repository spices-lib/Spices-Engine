/**
* @file VulkanImage.h.
* @brief The VulkanImage Class Definitions.
* @author Spiecs & Khronos Vulkan Tutorial.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

namespace Spiecs {

	/**
	* @breif VulkanImage is a wapper of VkImage.
	* It's a basic object of Vulkan.
	* @todo Wapper by Texture.
	*/
	class VulkanImage : public VulkanObject
	{
	public:

		/**
		* @brief Constructor Function.
		* Init class variable.
		* Init a empty VulkanImage, used in TextureLoad.
		* @param[in] vulkanState, The VulkanObject in used this frame.
		*/
		VulkanImage(VulkanState& vulkanState) 
			: VulkanObject(vulkanState) 
		{};

		/**
		* @breif Constructor Function.
		* Init class variable.
		* Init a empty VulkanImage, used in TextureLoad.
		* @param[in] vulkanState, The VulkanObject in used this frame.
		* @param[in] filePath, The filepath of local texture.
		* @attention Not in use.
		*/
		//VulkanImage(VulkanState& vulkanState, const std::string& filePath) : VulkanObject(vulkanState) {};

		/**
		* @breif Constructor Function.
		* Init class variable.
		* Create the vkImage by specific parameters.
		* @param[in] vulkanState, The VulkanObject in used this frame.
		* @param[in] name, Image's name.
		* @param[in] width, Image's width.
		* @param[in] height, Image's height.
		* @param[in] numSamples, Image's MSAA sample num.(8 if enable MSAA).
		* @param[in] format, Image's format.
		* @param[in] tiling, Image's tilling.
		* @param[in] usage, Image's used stage.
		* @param[in] properties, Image's data memory requirement.
		* @param[in] mipLevels, Image's mipnum, if need.
		*/
		VulkanImage(
			VulkanState&          vulkanState  ,
			const std::string&    name         ,
			uint32_t              width        ,
			uint32_t              height       ,
			VkSampleCountFlagBits numSamples   ,
			VkFormat              format       ,
			VkImageTiling         tiling       ,
			VkImageUsageFlags     usage        ,
			VkMemoryPropertyFlags properties   ,
			uint32_t              mipLevels     
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanImage();

		/**
		* @breif Get VkImageView of this VkImage.
		* Need Create before call this API.
		* @return Returns VkImageView reference.
		*/
		inline VkImageView& GetView() { return m_ImageView; };

		/**
		* @breif Get this VkImage.
		* @return Returns VkImage reference.
		*/
		inline VkImage& GetImage() { return m_Image; };

	public:

		/**
		* @breif Get VkDescriptorImageInfo.
		* @param[in] imageLayout, Usually is VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, also support custom layout.
		* @return Returns VkDescriptorImageInfo row pointer, avoiding copy, use pointer here.
		* fell free to useing row pointer here.
		*/
		VkDescriptorImageInfo* GetImageInfo(
			VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
		);

		/**
		* @breif Transform this VkImage's layout.
		* @param[in] format, This VkImage's format, see m_Format.
		* @param[in] oldLayout, The original layout, pass VK_IMAGE_LAYOUT_UNDEFINED if don't known the actural layout.
		* @param[in] newLayout, The new layout.
		*/
		void TransitionImageLayout(
			VkFormat        format     , 
			VkImageLayout   oldLayout  , 
			VkImageLayout   newLayout
		);

		/**
		* @brief Copy the Buffer's data to this VkImage.
		* @param[in] buffer, The buffer we want copy from.
		* @param[in] image, The image we want copy to, usually this VkImage Object.
		* @param[in] width, The image's width.
		* @param[in] height, The image's height.
		*/
		void CopyBufferToImage(
			VkBuffer   buffer   , 
			VkImage    image    , 
			uint32_t   width    , 
			uint32_t   height
		);

		/**
		* @breif 
		*/
		void CopyImageTexelToBuffer(uint32_t x, uint32_t y, void* out_rgba);
		void GenerateMipmaps(VkFormat imageFormat, int32_t texWidth, int32_t texHeight);
		void CreateImageView(VkFormat format, VkImageAspectFlags aspectFlags);
		void CreateSampler();
		void CreateImage(
			VulkanState& vulkanState, 
			const std::string& name,
			uint32_t width, 
			uint32_t height, 
			VkSampleCountFlagBits numSamples, 
			VkFormat format, 
			VkImageTiling tiling, 
			VkImageUsageFlags usage, 
			VkMemoryPropertyFlags properties, 
			uint32_t mipLevels
		);
		void CreateDescriptorSet(uint32_t binding);
		VkDescriptorSet& GetDescriptorSet() { return m_DescriptorSet; };
		void DestroyDescriptorSetLayout();

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

		bool m_IsCreateSet = false;
		VkDescriptorSetLayout m_DescriptorSetLayout;
		VkDescriptorSet m_DescriptorSet{};

		VkFormat m_Format;

		friend class TextureLoader;
	};
}