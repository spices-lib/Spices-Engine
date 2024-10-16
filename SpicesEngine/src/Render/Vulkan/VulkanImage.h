/**
* @file VulkanImage.h.
* @brief The VulkanImage Class Definitions.
* @author Spices & Khronos Vulkan Tutorial.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"
#include <ktxvulkan.h>

namespace Spices {

	/**
	* @breif VulkanImage is a wrapper of VkImage.
	* It's a basic object of Vulkan.
	* @todo Wrapper by Texture.
	*/
	class VulkanImage : public VulkanObject
	{
	public:

		/**
		* @brief Constructor Function.
		* Init class variable.
		* Init a empty VulkanImage, used in TextureLoad.
		* @param[in] vulkanState The VulkanObject in used this frame.
		*/
		VulkanImage(
			VulkanState& vulkanState
		)
			: VulkanObject(vulkanState) 
		{}

		/**
		* @breif Constructor Function.
		* Init class variable.
		* Create the vkImage by specific parameters.
		* @param[in] vulkanState The VulkanObject in used this frame.
		* @param[in] name Image's name.
		* @param[in] type Image's type.
		* @param[in] width Image's width.
		* @param[in] height Image's height.
		* @param[in] layers Image's layers(texture cube).
		* @param[in] numSamples Image's MSAA sample num.(8 if enable MSAA).
		* @param[in] format Image's format.
		* @param[in] tiling Image's tilling.
		* @param[in] usage Image's used stage.
		* @param[in] flags Image's used flags.
		* @param[in] properties Image's data memory requirement.
		* @param[in] mipLevels Image's mip num, if need.
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
			VkImageCreateFlags    flags       ,
			VkMemoryPropertyFlags properties   ,
			uint32_t              mipLevels     
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanImage() override;

		/**
		* @breif Get VkImageView of this VkImage.
		* Need Create before call this API.
		* @return Returns VkImageView reference.
		*/
		VkImageView& GetView() { return m_ImageView; }

		/**
		* @breif Get this VkImage.
		* @return Returns VkImage reference.
		*/
		VkImage& GetImage() { return m_Image; }

		/**
		* @brief Get this Layers.
		* @return Returns the m_Layers.
		*/
		uint32_t GetLayers() const { return m_Layers; }

	public:

		/**
		* @breif Get VkDescriptorImageInfo.
		* @param[in] imageLayout Usually is VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, also support custom layout.
		* @return Returns VkDescriptorImageInfo row pointer, avoiding copy, use pointer here.
		* fell free to using row pointer here.
		*/
		VkDescriptorImageInfo* GetImageInfo(
			VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
		);

		/**
		* @breif Transform this VkImage's layout.
		* @param[in] format This VkImage's format, see m_Format.
		* @param[in] oldLayout The original layout, pass VK_IMAGE_LAYOUT_UNDEFINED if don't known the actual layout.
		* @param[in] newLayout The new layout.
		*/
		void TransitionImageLayout(
			VkFormat        format     , 
			VkImageLayout   oldLayout  , 
			VkImageLayout   newLayout
		);

		/**
		* @brief Copy the Buffer's data to this VkImage.
		* @param[in] buffer The buffer we want copy from.
		* @param[in] image The image we want copy to, usually this VkImage Object.
		* @param[in] width The image's width.
		* @param[in] height The image's height.
		*/
		void CopyBufferToImage(
			VkBuffer   buffer   , 
			VkImage    image    , 
			uint32_t   width    , 
			uint32_t   height
		) const;

		/**
		* @brief Copy the Buffer's data to this VkImage.
		* Used to create image data (include mipmaps), which owns a compressed format.
		* @param[in] buffer The buffer we want copy from.
		* @param[in] image The image we want copy to, usually this VkImage Object.
		* @param[in] width The image's width.
		* @param[in] height The image's height.
		* @param[in] regions Specific Regions.
		*/
		void CopyBufferToImage(
			VkBuffer   buffer   , 
			VkImage    image    , 
			uint32_t   width    , 
			uint32_t   height   ,
			const std::vector<VkBufferImageCopy>& regions
		) const;

		/**
		* @brief Copy the Memory's data to this VkImage.
		* Used to create image data (include mipmaps), which owns a compressed format.
		* @param[in] copys The copy informations.
		*/
		void CopyMemoryToImageHost(
			const std::vector<VkMemoryToImageCopyEXT>& copys
		) const;

		/**
		* @brief Copy the Memory's data to this VkImage.
		* @param[in] data Memory data.
		*/
		void CopyMemoryToImageHost(const void* data) const;

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
		* @breif Copy data from a VkImage.
		* @param[in] buffer The Dst Buffer.
		* @param[in] regions Specific Regions.
		*/
		void CopyImageToBuffer(
			VkBuffer dstBuffer,
			const std::vector<VkBufferImageCopy>& regions
		);

		/**
		* @breif Copy memory from a VkImage.
		* @param[in] data Outdata.
		*/
		void CopyImageToMemoryHost(void* data);

		/**
		* @breif Copy memory from a VkImage.
		* @param[in] copys Copy Informations.
		*/
		void CopyImageToMemoryHost(const std::vector<VkImageToMemoryCopyEXT>& copys);

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
		) const;

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
		* @param[in] vulkanState The VulkanObject in used this frame.
		* @param[in] name Image's name.
		* @param[in] type Image's type.
		* @param[in] width Image's width.
		* @param[in] height Image's height.
		* @param[in] layers Image's layers(texture cube).
		* @param[in] numSamples Image's MSAA sample num.(8 if enable MSAA).
		* @param[in] format Image's format.
		* @param[in] tiling Image's tilling.
		* @param[in] usage Image's used stage.
		* @param[in] flags Image's used flags.
		* @param[in] properties Image's data memory requirement.
		* @param[in] mipLevels Image's mip num, if need.
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
			VkImageCreateFlags     flags        ,
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
		inline VkDescriptorSet& GetDescriptorSet() { return m_DescriptorSet; }

		/**
		* @brief Check if this image format can copy from host to gpu directly.
		* @return Returns true if can do that.
		*/
		bool IsHostCopyable();

		/**
		* @brief Check if this image format can copy from host to gpu directly.
		* @param[in] state VulkanState.
		* @param[in] format VkFormat.
		* @return Returns true if can do that.
		*/
		static bool IsHostCopyable(VulkanState& state, VkFormat format);

	private:

		/**
		* @brief Destroy the DescriptorSetLayout if Created a DescriptorSet.
		*/
		void DestroyDescriptorSetLayout() const;

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
		* @brief Image layer(texture cube: 6).
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
		* @brief The Vkimage this Class Wrapped.
		*/
		VkImage m_Image{};

		/**
		* @brief VMA allocation.
		*/
		VmaAllocation m_Alloc{};

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
		* @brief Allow access all data.
		*/
		friend class TextureLoader;
		friend class Transcoder;
	};
}