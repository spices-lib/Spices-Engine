/**
* @file VulkanImage.cpp.
* @brief The VulkanImage Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "VulkanImage.h"

#include "VulkanBuffer.h"
#include "VulkanCommandBuffer.h"
#include "VulkanDescriptor.h"
#include "Render/Vulkan/VulkanRenderBackend.h"

namespace Spices {

	VulkanImage::VulkanImage(
		VulkanState&          vulkanState , 
		const std::string&    name        ,
		VkImageType           type        ,
		uint32_t              width       , 
		uint32_t              height      , 
		uint32_t              layers      ,
		VkSampleCountFlagBits numSamples  , 
		VkFormat              format      ,   
		VkImageTiling         tiling      , 
		VkImageUsageFlags     usage       ,
		VkImageCreateFlags    flags      ,
		VkMemoryPropertyFlags properties  , 
		uint32_t              mipLevels
	)
		: VulkanObject(vulkanState)
		, m_Width(static_cast<int>(width))
		, m_Height(static_cast<int>(height))
		, m_Layers(layers)
		, m_ImageType(type)
		, m_Format(format)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Create Image.
		*/
		CreateImage(
			vulkanState, 
			name, 
			type,
			width, 
			height, 
			layers, 
			numSamples, 
			format, 
			tiling, 
			usage,
			flags,
			properties, 
			mipLevels
		);
	}

	VulkanImage::~VulkanImage()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Destroy DescriptorSet.
		*/
		DestroyDescriptorSetLayout();

		/**
		* @brief Destroy reltative object.
		*/
		vkDestroySampler(m_VulkanState.m_Device, m_TextureSampler, nullptr);
		vkDestroyImageView(m_VulkanState.m_Device, m_ImageView, nullptr);

#ifdef VMA_ALLOCATOR

		/**
		* @brief Destroy VkImage.
		*/
		vmaDestroyImage(m_VulkanState.m_VmaAllocator, m_Image, m_Alloc);

#else

		/**
		* @brief Destroy VkImage.
		*/
		vkDestroyImage(m_VulkanState.m_Device, m_Image, nullptr);
		vkFreeMemory(m_VulkanState.m_Device, m_ImageMemory, nullptr);

#endif

	}

	VkDescriptorImageInfo* VulkanImage::GetImageInfo(VkImageLayout imageLayout)
	{
		SPICES_PROFILE_ZONE;

		m_ImageInfo.imageLayout = imageLayout;
		m_ImageInfo.imageView = m_ImageView;
		m_ImageInfo.sampler = m_TextureSampler;

		return &m_ImageInfo;
	}

	void VulkanImage::TransitionImageLayout(
		VkFormat      format    , 
		VkImageLayout oldLayout , 
		VkImageLayout newLayout
	)
	{
		SPICES_PROFILE_ZONE;

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		/**
		* @brief Instance a VkImageMemoryBarrier.
		*/
		VkImageMemoryBarrier barrier{};
		barrier.sType                             = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout                         = oldLayout;
		barrier.newLayout                         = newLayout;
		barrier.srcQueueFamilyIndex               = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex               = VK_QUEUE_FAMILY_IGNORED;
		barrier.image                             = m_Image;
		barrier.subresourceRange.aspectMask       = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel     = 0;
		barrier.subresourceRange.levelCount       = m_MipLevels;
		barrier.subresourceRange.baseArrayLayer   = 0;
		barrier.subresourceRange.layerCount       = m_Layers;
		barrier.srcAccessMask                     = 0;
		barrier.dstAccessMask                     = 0;

		/**
		* @breif transform imagelayout from undefined to transferdst.
		* Used during CopyBuffertoImage.
		*/
		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) 
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			// Don't care what stage the pipeline is in at the start.
			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

			// Used for copying
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}

		/**
		* @breif transform imagelayout from undefined to transfersrc.
		* Used during CopyImagetoBuffer.
		*/
		else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) 
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

			// Don't care what stage the pipeline is in at the start.
			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

			// Used for copying
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}

		/**
		* @breif transform imagelayout from transferdst to shaderread.
		* Used combine with CopyBuffertoImage.
		*/
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			// From a copying stage to...
			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;

			// The fragment stage.
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}

		/**
		* @brief transform imagelayout from transfersrc to shaderread.
		* Used combine with CopyImagetoBuffer.
		*/
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			// From a copying stage to...
			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;

			// The fragment stage.
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}

		/**
		* @brief transfer imagelayout from undefined to depthattachment.
		* Used during Creating depth renderresource.
		*/
		else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) 
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}

		/**
		* @brief transfer imagelayout from undefined to depthattachment.
		* Used during Creating depth renderresource.
		*/
		else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_GENERAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_IMAGE_ASPECT_NONE;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		}
		else 
		{
			SPICES_CORE_WARN("Unsupported layout transition!");
		}

		if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) 
		{
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

			if (format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT) 
			{
				barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			}
		}
		else 
		{
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		}

		/**
		* @brief Use Custom Cmd.
		*/
		VulkanCommandBuffer::CustomGraphicCmd(m_VulkanState, [&](auto& commandBuffer) {
			vkCmdPipelineBarrier(
				commandBuffer,
				sourceStage, 
				destinationStage,
				0,
				0, 
				nullptr,
				0, 
				nullptr,
				1, 
				&barrier
			);
		});
	}

	void VulkanImage::CopyBufferToImage(
		VkBuffer buffer  , 
		VkImage  image   , 
		uint32_t width   , 
		uint32_t height
	) const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Instance a VkBufferImageCopy.
		*/
		VkBufferImageCopy region{};
		region.bufferOffset                    = 0;
		region.bufferRowLength                 = 0;
		region.bufferImageHeight               = 0;

		region.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel       = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount     = m_Layers;

		region.imageOffset                     = { 0, 0, 0 };
		region.imageExtent                     = { width, height, 1 };

		/**
		* @brief Use Custom Cmd.
		*/
		VulkanCommandBuffer::CustomGraphicCmd(m_VulkanState, [&](VkCommandBuffer& commandBuffer) {
			vkCmdCopyBufferToImage(
				commandBuffer, 
				buffer, 
				image, 
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
				1, 
				&region
			);
		});
	}

	void VulkanImage::CopyImageTexelToBuffer(uint32_t x, uint32_t y, void* out_rgba)
	{
		SPICES_PROFILE_ZONE;

		uint32_t channelize = 4;

		/**
		* @todo Support all type;
		*/
		switch (m_Format)
		{
		case VK_FORMAT_B8G8R8A8_UNORM:           // 4 bytes.
			channelize = 4;			         
			break;						         
		case VK_FORMAT_R32_SFLOAT:               // 4 bytes.
			channelize = 4;
			break;
		case VK_FORMAT_R32G32B32A32_SFLOAT:      // 16 bytes.
			channelize = 16;
			break;
		default:
			break;
		}

		/**
		* @brief The temp buffer image date copy to.
		*/
		VulkanBuffer stagingbuffer(
			m_VulkanState, 
			channelize,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		);

		/*
		* @brief Transfer image layout from whatever to trasfer src.
		*/
		TransitionImageLayout(
			m_Format,
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
		);

		/**
		* @brief Instance a VkBufferImageCopy.
		*/
		VkBufferImageCopy region{};
		region.bufferOffset                    = 0;
		region.bufferRowLength                 = 0;
		region.bufferImageHeight               = 0;

		region.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel       = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount     = m_Layers;

		region.imageOffset.x                   = static_cast<int32_t>(x);
		region.imageOffset.y                   = static_cast<int32_t>(y);
		region.imageExtent                     = { 1, 1, 1 };

		/**
		* @brief Use Custom Cmd.
		*/
		VulkanCommandBuffer::CustomGraphicCmd(m_VulkanState, [&](VkCommandBuffer& commandBuffer) {
			vkCmdCopyImageToBuffer(commandBuffer, m_Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, stagingbuffer.Get(), 1, &region);
		});

		/*
		* @brief Transfer image layout from transfer src to shader read.
		* means only can get data from a shader read layout image.
		* @note In SpicesEngine, we need transform layout to shader read in scenecomposerenderer first,
		* and after that, you can do this here.
		*/
		TransitionImageLayout(
			m_Format,
			VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
		);

		/**
		* @brief Write data to stagingbuffer.
		*/
		stagingbuffer.WriteFromBuffer(out_rgba);
	}

	void VulkanImage::GenerateMipmaps(VkFormat imageFormat, int32_t texWidth, int32_t texHeight) const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Get video memory properties.
		*/
		VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(m_VulkanState.m_PhysicalDevice, imageFormat, &formatProperties);

		/**
		* @brief Must require VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT.
		*/
		if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) 
		{
			SPICES_CORE_ERROR("texture image format does not support linear blitting!");
		}

		/**
		* @brief Instance a VkImageMemoryBarrier.
		*/
		VkImageMemoryBarrier barrier{};
		barrier.sType                                = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.image                                = m_Image;
		barrier.srcQueueFamilyIndex                  = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex                  = VK_QUEUE_FAMILY_IGNORED;
		barrier.subresourceRange.aspectMask          = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseArrayLayer      = 0;
		barrier.subresourceRange.layerCount          = m_Layers;
		barrier.subresourceRange.levelCount          = 1;

		int32_t mipWidth = texWidth;
		int32_t mipHeight = texHeight;

		/**
		* @brief Use Custom Cmd.
		*/
		VulkanCommandBuffer::CustomGraphicCmd(m_VulkanState, [&](VkCommandBuffer& commandBuffer) {

			/**
			* @brief Iter all mips.
			*/
			for (uint32_t i = 1; i < m_MipLevels; i++) 
			{
				barrier.subresourceRange.baseMipLevel = i - 1;
				barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

				/**
				* @breif Pipeline Barrier.
				*/
				vkCmdPipelineBarrier(commandBuffer,
					VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
					0, nullptr,
					0, nullptr,
					1, &barrier);

				/**
				* @brief Instance a VkImageBlit.
				*/
				VkImageBlit blit{};
				blit.srcOffsets[0]                       = { 0, 0, 0 };
				blit.srcOffsets[1]                       = { mipWidth, mipHeight, 1 };
				blit.srcSubresource.aspectMask           = VK_IMAGE_ASPECT_COLOR_BIT;
				blit.srcSubresource.mipLevel             = i - 1;
				blit.srcSubresource.baseArrayLayer       = 0;
				blit.srcSubresource.layerCount           = m_Layers;
				blit.dstOffsets[0]                       = { 0, 0, 0 };
				blit.dstOffsets[1]                       = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
				blit.dstSubresource.aspectMask           = VK_IMAGE_ASPECT_COLOR_BIT;
				blit.dstSubresource.mipLevel             = i;
				blit.dstSubresource.baseArrayLayer       = 0;
				blit.dstSubresource.layerCount           = m_Layers;

				/**
				* @brief BitlImage.
				*/
				vkCmdBlitImage(commandBuffer,
					m_Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
					m_Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
					1, &blit,
					VK_FILTER_LINEAR);

				barrier.oldLayout                        = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
				barrier.newLayout                        = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				barrier.srcAccessMask                    = VK_ACCESS_TRANSFER_READ_BIT;
				barrier.dstAccessMask                    = VK_ACCESS_SHADER_READ_BIT;

				/**
				* @brief Pipeline Barrier.
				*/
				vkCmdPipelineBarrier(commandBuffer,
					VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
					0, nullptr,
					0, nullptr,
					1, &barrier);

				if (mipWidth > 1)  mipWidth  /= 2;
				if (mipHeight > 1) mipHeight /= 2;
			}

			barrier.subresourceRange.baseMipLevel        = m_MipLevels - 1;
			barrier.oldLayout                            = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.newLayout                            = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			barrier.srcAccessMask                        = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask                        = VK_ACCESS_SHADER_READ_BIT;

			/**
			* @brief Pipeline Barrier.
			*/
			vkCmdPipelineBarrier(commandBuffer,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
				0, nullptr,
				0, nullptr,
				1, &barrier);
		});
	}

	void VulkanImage::CreateImageView(VkFormat format, VkImageViewType viewType, VkImageAspectFlags aspectFlags)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Instance a VkImageViewCreateInfo.
		*/
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType                             = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image                             = m_Image;
		viewInfo.viewType                          = viewType;
		viewInfo.format                            = format;
		viewInfo.subresourceRange.aspectMask       = aspectFlags;
		viewInfo.subresourceRange.baseMipLevel     = 0;
		viewInfo.subresourceRange.levelCount       = m_MipLevels;                                 // mipmaps num.
		viewInfo.subresourceRange.baseArrayLayer   = 0;                                           // layer index.(access given index layer of texture array/texture cube)
		viewInfo.subresourceRange.layerCount       = m_Layers;                                    // layer num.

		/**
		* @brief Create ImageView. 
		*/
		VK_CHECK(vkCreateImageView(m_VulkanState.m_Device, &viewInfo, nullptr, &m_ImageView));
		DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_IMAGE_VIEW, (uint64_t)m_ImageView, m_VulkanState.m_Device, "ImageView")
	}

	void VulkanImage::CreateSampler()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Instance a VkSamplerCreateInfo.
		*/
		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType                          = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter                      = VK_FILTER_LINEAR;
		samplerInfo.minFilter                      = VK_FILTER_LINEAR;
		samplerInfo.addressModeU                   = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV                   = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW                   = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable               = VK_TRUE;

		/**
		* @brief Get VkPhysicalDeviceProperties.
		*/
		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(m_VulkanState.m_PhysicalDevice, &properties);

		samplerInfo.maxAnisotropy                  = properties.limits.maxSamplerAnisotropy;
		samplerInfo.borderColor                    = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates        = VK_FALSE;
		samplerInfo.compareEnable                  = VK_FALSE;
		samplerInfo.compareOp                      = VK_COMPARE_OP_ALWAYS;

		samplerInfo.mipmapMode                     = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias                     = 0.0f;
		samplerInfo.minLod                         = 0;
		samplerInfo.maxLod                         = static_cast<float>(m_MipLevels);

		/**
		* @brief Create Sampler.
		*/
		VK_CHECK(vkCreateSampler(m_VulkanState.m_Device, &samplerInfo, nullptr, &m_TextureSampler));
		DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_SAMPLER, (uint64_t)m_TextureSampler, m_VulkanState.m_Device, "ImageSampler")
	}

	void VulkanImage::CreateImage(
		VulkanState&          vulkanState ,
		const std::string&    name        ,
		VkImageType           type        ,
		uint32_t              width       , 
		uint32_t              height      , 
		uint32_t              layers      ,
		VkSampleCountFlagBits numSamples  , 
		VkFormat              format      , 
		VkImageTiling         tiling      , 
		VkImageUsageFlags     usage       , 
		VkImageCreateFlags    flags      ,
		VkMemoryPropertyFlags properties  , 
		uint32_t              mipLevels
	)
	{
		SPICES_PROFILE_ZONE;

		m_MipLevels = mipLevels;
		m_Format = format;

		/**
		* @brief Instance a VkImageCreateInfo.
		*/
		VkImageCreateInfo                            imageInfo{};
		imageInfo.sType                            = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType                        = type;
		imageInfo.extent.width                     = width;
		imageInfo.extent.height                    = height;
		imageInfo.extent.depth                     = 1;
		imageInfo.mipLevels                        = mipLevels;
		imageInfo.arrayLayers                      = layers;
		imageInfo.format                           = format;
		imageInfo.tiling                           = tiling;
		imageInfo.initialLayout                    = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage                            = usage;
		imageInfo.sharingMode                      = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.samples                          = numSamples;
		imageInfo.flags                            = flags;

#ifdef VMA_ALLOCATOR

		/**
		* @brief Instance a VmaAllocationCreateInfo.
		*/
		VmaAllocationCreateInfo        createInfo{};
		createInfo.usage             = VMA_MEMORY_USAGE_AUTO;

		/**
		* @brief Create Image.
		*/
		VK_CHECK(vmaCreateImage(vulkanState.m_VmaAllocator, &imageInfo, &createInfo, &m_Image, &m_Alloc, nullptr))
		DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_IMAGE, (uint64_t)m_Image, m_VulkanState.m_Device, name)

#else

		/**
		* @brief Create Image.
		*/
		VK_CHECK(vkCreateImage(vulkanState.m_Device, &imageInfo, nullptr, &m_Image));
		DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_IMAGE, (uint64_t)m_Image, m_VulkanState.m_Device, name)

		/**
		* @brief Get VkMemoryRequirements.
		*/
		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(vulkanState.m_Device, m_Image, &memRequirements);

		/**
		* @brief Instance a VkMemoryAllocateInfo.
		*/
		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType                     = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize            = memRequirements.size;

		/**
		* @brief Get VkPhysicalDeviceMemoryProperties.
		*/
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(vulkanState.m_PhysicalDevice, &memProperties);

		/**
		* @brief Get suitable video memory index.
		*/
		allocInfo.memoryTypeIndex = 0;
		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) 
		{
			if (memRequirements.memoryTypeBits & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) 
			{
				allocInfo.memoryTypeIndex = i;
			}
		}

		/**
		* @brief Allocate video memory.
		*/
		VK_CHECK(vkAllocateMemory(vulkanState.m_Device, &allocInfo, nullptr, &m_ImageMemory))

		/**
		* @brief Bind video memory.
		*/
		vkBindImageMemory(vulkanState.m_Device, m_Image, m_ImageMemory, 0);

#endif

	}

	void VulkanImage::CreateDescriptorSet(uint32_t binding)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Destroy old DescriptorSetLayout.
		*/
		DestroyDescriptorSetLayout();

		m_IsCreateSet = true;

		/**
		* @brief Instance a VkDescriptorSetLayoutBinding.
		*/
		VkDescriptorSetLayoutBinding samplerLayoutBinding{};
		samplerLayoutBinding.binding                      = binding;
		samplerLayoutBinding.descriptorCount              = 1;
		samplerLayoutBinding.descriptorType               = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.pImmutableSamplers           = nullptr;
		samplerLayoutBinding.stageFlags                   = VK_SHADER_STAGE_FRAGMENT_BIT;

		/**
		* @brief Instance a VkDescriptorSetLayoutCreateInfo.
		*/
		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType                                  = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount                           = 1;
		layoutInfo.pBindings                              = &samplerLayoutBinding;

		/**
		* @brief Create DescriptorSetLayout.
		*/
		VK_CHECK(vkCreateDescriptorSetLayout(m_VulkanState.m_Device, &layoutInfo, nullptr, &m_DescriptorSetLayout))
		DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT, (uint64_t)m_DescriptorSetLayout, m_VulkanState.m_Device, "DescriptorSetLayoutImage")

		/**
		* @brief Instance a VkDescriptorSetAllocateInfo.
		*/
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType                                   = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool                          = VulkanRenderBackend::GetDescriptorPool()->GetPool();
		allocInfo.descriptorSetCount                      = 1;
		allocInfo.pSetLayouts                             = &m_DescriptorSetLayout;

		/**
		* @brief Allocate DescriptorSets.
		*/
		VK_CHECK(vkAllocateDescriptorSets(m_VulkanState.m_Device, &allocInfo, &m_DescriptorSet))
		DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_DESCRIPTOR_SET, (uint64_t)m_DescriptorSet, m_VulkanState.m_Device, "DescriptorSetImage")
		
		/**
		* @brief Instance a VkDescriptorImageInfo.
		*/
		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout                             = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView                               = m_ImageView;
		imageInfo.sampler                                 = m_TextureSampler;

		/**
		* @brief Instance a VkWriteDescriptorSet.
		*/
		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType                             = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet                            = m_DescriptorSet;
		descriptorWrite.dstBinding                        = binding;
		descriptorWrite.dstArrayElement                   = 0;
		descriptorWrite.descriptorType                    = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrite.descriptorCount                   = 1;
		descriptorWrite.pImageInfo                        = &imageInfo;

		/**
		* @brief Update DecriptorSet.
		*/
		vkUpdateDescriptorSets(m_VulkanState.m_Device, 1, &descriptorWrite, 0, nullptr);
	}

	void VulkanImage::DestroyDescriptorSetLayout() const
	{
		SPICES_PROFILE_ZONE;

		if (m_IsCreateSet)
		{
			/**
			* @brief Destroy DescriptorSetLayout.
			*/
			vkDestroyDescriptorSetLayout(m_VulkanState.m_Device, m_DescriptorSetLayout, nullptr);
		}
	}
}