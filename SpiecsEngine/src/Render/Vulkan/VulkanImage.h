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
		VulkanImage(
			VulkanState& vulkanState
		)
			: VulkanObject(vulkanState) 
		{};

		/**
		* @breif Constructor Function.
		* Init class variable.
		* Create the vkImage by specific parameters.
		* @param[in] vulkanState, The VulkanObject in used this frame.
		* @param[in] name, Image's name.
		* @param[in] type, Image's type.
		* @param[in] width, Image's width.
		* @param[in] height, Image's height.
		* @param[in] layers, Image's layers(texturecube).
		* @param[in] numSamples, Image's MSAA sample num.(8 if enable MSAA).
		* @param[in] format, Image's format.
		* @param[in] tiling, Image's tilling.
		* @param[in] usage, Image's used stage.
		* @param[in] flages, Image's used flages.
		* @param[in] properties, Image's data memory requirement.
		* @param[in] mipLevels, Image's mipnum, if need.
		*/
		VulkanImage(
			VulkanState&          vulkanState  ,
			const std::string&    name         ,
			VkImageType           type         ,
			uint32_t              width        ,
			uint32_t              height       ,
			uint32_t              layers       ,
			VkSampleCountFlagBits numSamples   ,
			VkFormat              format       ,
			VkImageTiling         tiling       ,
			VkImageUsageFlags     usage        ,
			VkImageCreateFlags    flages       ,
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

		/**
		* @brief Get this Layers.
		* @return Returns the m_Layers.
		*/
		inline uint32_t GetLayers() { return m_Layers; };

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
		* @breif Copy one texel data from a VkImage.
		* @param[in] x The sampled position x.
		* @param[in] y The sampled position y.
		* @param[in] out_rgba The sampled data.
		*/
		void CopyImageTexelToBuffer(
			uint32_t x        , 
			uint32_t y        , 
			void*    out_rgba
		);

		/**
		* @brief Generate mipmaps with the VkImage.
		* @param[in] imageFormat VkFormat.
		* @param[in] texWidth The Image Width.
		* @param[in] texHeight The Image Height.
		*/
		void GenerateMipmaps(
			VkFormat imageFormat , 
			int32_t  texWidth    , 
			int32_t  texHeight
		);

		/**
		* @brief Create Image View.
		* @param[in] format VkFormat.
		* @param[in] viewType VkImageViewType.
		* @param[in] aspectFlags VkImageAspectFlags.
		*/
		void CreateImageView(
			VkFormat           format      , 
			VkImageViewType    viewType    ,
			VkImageAspectFlags aspectFlags
		);

		/**
		* @brief Create a Sampler.
		*/
		void CreateSampler();

		/**
		* @breif Create a VkImage.
		* @param[in] vulkanState, The VulkanObject in used this frame.
		* @param[in] name, Image's name.
		* @param[in] type, Image's type.
		* @param[in] width, Image's width.
		* @param[in] height, Image's height.
		* @param[in] layers, Image's layers(texturecube).
		* @param[in] numSamples, Image's MSAA sample num.(8 if enable MSAA).
		* @param[in] format, Image's format.
		* @param[in] tiling, Image's tilling.
		* @param[in] usage, Image's used stage.
		* @param[in] flages, Image's used flages.
		* @param[in] properties, Image's data memory requirement.
		* @param[in] mipLevels, Image's mipnum, if need.
		*/
		void CreateImage(
			VulkanState&           vulkanState   , 
			const std::string&     name          ,
			VkImageType            type          ,
			uint32_t               width         , 
			uint32_t               height        , 
			uint32_t               layers        ,
			VkSampleCountFlagBits  numSamples    , 
			VkFormat               format        , 
			VkImageTiling          tiling        , 
			VkImageUsageFlags      usage         , 
			VkImageCreateFlags     flages        ,
			VkMemoryPropertyFlags  properties    , 
			uint32_t               mipLevels
		);

		/**
		* @brief Create DescriptorSet with single image.
		* @param[in] binding Which binding will be use.
		*/
		void CreateDescriptorSet(uint32_t binding);

		/**
		* @brief Get VkDescriptorSet.
		* @return Returns the VkDescriptorSet.
		*/
		inline VkDescriptorSet& GetDescriptorSet() { return m_DescriptorSet; };

	private:

		/**
		* @brief Destroy the DescriptorSetLayout if Created a DescriptorSet.
		*/
		void DestroyDescriptorSetLayout();

	private:

		/**
		* @brief Image width.
		*/
		int m_Width = 0;

		/**
		* @brief Image height.
		*/
		int m_Height = 0;

		/**
		* @brief Image layer(texturecube: 6).
		*/
		uint32_t m_Layers = 1;

		/**
		* @brief Image mipmaps num.
		*/
		uint32_t m_MipLevels = 1;

		/**
		* @brief Image Type.
		*/
		VkImageType m_ImageType {};

		/**
		* @brief The image format.
		*/
		VkFormat m_Format{};

		/**
		* @brief The Vkimage this Class Wappered.
		*/
		VkImage m_Image{};

		/**
		* @brief The image video memory.
		*/
		VkDeviceMemory m_ImageMemory{};

		/**
		* @brief The image view.
		*/
		VkImageView m_ImageView{};

		/**
		* @brief The image sampler.
		*/
		VkSampler m_TextureSampler{};

		/**
		* @brief VkDescriptorImageInfo.
		*/
		VkDescriptorImageInfo m_ImageInfo{};

		/**
		* @brief True if Called Create DescriptorSet.
		*/
		bool m_IsCreateSet = false;

		/**
		* @brief VkDescriptorSetLayout.
		*/
		VkDescriptorSetLayout m_DescriptorSetLayout{};

		/**
		* @brief VkDescriptorSet.
		*/
		VkDescriptorSet m_DescriptorSet{};

		/**
		* @brief Allow TextureLoader access all data.
		*/
		friend class TextureLoader;
	};
}