/**
* @file TextureLoader.cpp.
* @brief The TextureLoader Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "TextureLoader.h"
#include "Render/Vulkan/VulkanImage.h"
#include "Render/Vulkan/VulkanRenderBackend.h"
#include "Core/Library/FileLibrary.h"
#include "Systems/ResourceSystem.h"
#include "Resources/Texture/Transcoder.h"

#include <stb_image.h>

#include "Resources/Texture/Texture2D.h"
#include "Resources/Texture/Texture2DCube.h"

namespace Spices {

	/**
	* @brief Const variable: Original Image File Path.
	*/
	const std::string defaultTexturePath = "Textures/src/";
	const std::string binTexturePath     = "Textures/bin/";

	void TextureLoader::Load(const std::string& fileName, Texture2D* outTexture)
	{
		SPICES_PROFILE_ZONE;

		SearchFile(
		fileName, 
		[&](const std::string& it) {
			LoadBin(fileName, it, outTexture);
		}, 
		[&](const std::string& it) {
			LoadSrc(fileName, it, outTexture);
		}
		);
	}

	void TextureLoader::Load(const std::string& fileName, Texture2DCube* outTexture)
	{

	}

	bool TextureLoader::SearchFile(
		const std::string& fileName, 
		std::function<void(const std::string&)> binF, 
		std::function<void(const std::string&)> srcF
	)
	{
		SPICES_PROFILE_ZONE;

		std::vector<std::string> splitString = StringLibrary::SplitString(fileName, '.');

		for (auto& it : ResourceSystem::GetSearchFolder())
		{
			std::string filePath = it + binTexturePath + splitString[0] + ".ktx";
			if (FileLibrary::FileLibrary_Exists(filePath.c_str()))
			{
				binF(it);
				return true;
			}
		}
		for (auto& it : ResourceSystem::GetSearchFolder())
		{
			std::string filePath = it + defaultTexturePath + fileName;
			if (FileLibrary::FileLibrary_Exists(filePath.c_str()))
			{
				srcF(it);
				return true;
			}
		}

		std::stringstream ss;
		ss << "File: " << fileName << " Not Find";

		SPICES_CORE_ERROR(ss.str());
		return false;
	}

	bool TextureLoader::LoadBin(const std::string& fileName, const std::string& it, Texture2D* outTexture)
	{
		SPICES_PROFILE_ZONE;

		std::vector<std::string> splitString = StringLibrary::SplitString(fileName, '.');
		std::string binPath = it + binTexturePath + splitString[0] + ".ktx";

		ktxTexture2* texture = nullptr;
		Transcoder::LoadFromKTX(binPath, texture);

		/**
		* @brief Instance the VulkanImage as Texture2D Resource.
		*/
		outTexture->m_Resource = std::make_shared<VulkanImage>(VulkanRenderBackend::GetState());
		auto resourceptr = outTexture->GetResource<VulkanImage>();

		VkImageUsageFlags usage = VK_IMAGE_USAGE_SAMPLED_BIT;  // Can be Used for Sample
		bool hostCopy = VulkanImage::IsHostCopyable(resourceptr->m_VulkanState, static_cast<VkFormat>(texture->vkFormat));
		usage |= hostCopy ? VK_IMAGE_USAGE_HOST_TRANSFER_BIT_EXT : VK_IMAGE_USAGE_TRANSFER_DST_BIT;

		/**
		* @brief Instance the Resource.
		*/
		resourceptr->CreateImage(
			resourceptr->m_VulkanState                          ,
			fileName                                            ,
			static_cast<VkImageType>(texture->numDimensions - 1),
			texture->baseWidth                                  ,
			texture->baseHeight                                 ,
			texture->baseDepth                                  ,  // 1 layers.
			VK_SAMPLE_COUNT_1_BIT                               ,  // No MASS.
			static_cast<VkFormat>(texture->vkFormat)            ,  // Compress Format.
			VK_IMAGE_TILING_OPTIMAL                             ,  // Tiling Optimal.
			usage                                               ,
			0                                                   ,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT                 ,
			texture->numLevels
		);

		if (hostCopy)
		{
			/**
			* @brief Transform Image Layout from undefined to transfer_dst.
			*/
			resourceptr->TransitionImageLayout(
				resourceptr->m_Format,
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
			);

			std::vector<VkMemoryToImageCopyEXT> copys;
			for (uint32_t mip_level = 0; mip_level < texture->numLevels; mip_level++)
			{
				VkMemoryToImageCopyEXT                       memoryCopy{};
				memoryCopy.sType                           = VK_STRUCTURE_TYPE_MEMORY_TO_IMAGE_COPY_EXT;
				memoryCopy.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
				memoryCopy.imageSubresource.mipLevel       = mip_level;
				memoryCopy.imageSubresource.baseArrayLayer = 0;
				memoryCopy.imageSubresource.layerCount     = 1;
				memoryCopy.imageExtent.width               = std::max((uint32_t)1, texture->baseWidth >> mip_level);
				memoryCopy.imageExtent.height              = std::max((uint32_t)1, texture->baseHeight >> mip_level);
				memoryCopy.imageExtent.depth               = 1;
				memoryCopy.pHostPointer                    = texture->pData + Transcoder::GetMipmapOffset(texture, mip_level);

				copys.push_back(memoryCopy);
			}

			/**
			* @brief Copy Memory to Image.
			*/
			resourceptr->CopyMemoryToImageHost(copys);
		}
		else
		{
			/**
			* @brief Instance a staginBuffer.
			*/
			VulkanBuffer stagingBuffer(
				resourceptr->m_VulkanState,
				"StagingBuffer"                     ,
				texture->dataSize                   ,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT    ,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
				VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
			);

			/**
			* @brief Copy the data from texture bytes to staginBuffer.
			*/
			stagingBuffer.WriteToBuffer(texture->pData);

			/**
			* @brief Transform Image Layout from undefined to transfer_dst.
			*/
			resourceptr->TransitionImageLayout(
				resourceptr->m_Format               ,
				VK_IMAGE_LAYOUT_UNDEFINED           ,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
			);

			std::vector<VkBufferImageCopy> regions;
			for (uint32_t mip_level = 0; mip_level < texture->numLevels; mip_level++)
			{
				VkBufferImageCopy                        region {};
				region.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
				region.imageSubresource.mipLevel       = mip_level;
				region.imageSubresource.baseArrayLayer = 0;
				region.imageSubresource.layerCount     = 1;
				region.imageExtent.width               = std::max((uint32_t)1, texture->baseWidth  >> mip_level);
				region.imageExtent.height              = std::max((uint32_t)1, texture->baseHeight >> mip_level);
				region.imageExtent.depth               = 1;
				region.bufferOffset                    = Transcoder::GetMipmapOffset(texture, mip_level);

				regions.push_back(region);
			}

			/**
			* @brief Copy the data from staginBuffer to Image.
			*/
			resourceptr->CopyBufferToImage(
				stagingBuffer.Get()                         ,
				resourceptr->m_Image                        ,
				static_cast<uint32_t>(resourceptr->m_Width) ,
				static_cast<uint32_t>(resourceptr->m_Height),
				regions
			);
		
			/**
			* @brief Transform Image Layout from transfer_dst to shaderread.
			*/
			resourceptr->TransitionImageLayout(
				resourceptr->m_Format                   ,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL    ,
				VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
			);
		}

		/**
		* @brief Create Image View.
		*/
		resourceptr->CreateImageView(
			resourceptr->m_Format    ,
			VK_IMAGE_VIEW_TYPE_2D    ,
			VK_IMAGE_ASPECT_COLOR_BIT
		);

		/**
		* @brief Create Image Sampler.
		*/
		resourceptr->CreateSampler();

		/**
		* @brief Release ktx image data.
		*/
		Transcoder::DestroyktxTexture2(texture);

		return true;
	}

	bool TextureLoader::LoadSrc(const std::string& fileName, const std::string& it, Texture2D* outTexture)
	{
		SPICES_PROFILE_ZONE;

		std::vector<std::string> splitString = StringLibrary::SplitString(fileName, '.');
		std::string filePath = it + defaultTexturePath + fileName;
		std::string binPath  = it + binTexturePath + splitString[0] + ".ktx";

		/**
		* @brief Load Texture data.
		*/
		int width;
		int height;
		int texChannels;
		stbi_uc* pixels = stbi_load(filePath.c_str(), &width, &height, &texChannels, STBI_rgb_alpha);
		if (!pixels)
		{
			SPICES_CORE_ERROR("Failed to load texture image!");
		}

		/**
		* @brief Instance the VulkanImage as Texture2D Resource.
		*/
		outTexture->m_Resource = std::make_shared<VulkanImage>(VulkanRenderBackend::GetState());
		auto resourceptr = outTexture->GetResource<VulkanImage>();

		/**
		* @brief Set resource values.
		*/
		resourceptr->m_Width       = width;
		resourceptr->m_Height      = height;
		resourceptr->m_MipLevels   = static_cast<uint32_t>(std::floor(std::log2(std::max(resourceptr->m_Width, resourceptr->m_Height)))) + 1;	
		
		VkImageUsageFlags usage = VK_IMAGE_USAGE_SAMPLED_BIT;  // Can be Used for Sample
		bool hostCopy = VulkanImage::IsHostCopyable(resourceptr->m_VulkanState, VK_FORMAT_R8G8B8A8_UNORM);
		usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		if (hostCopy) usage |= VK_IMAGE_USAGE_HOST_TRANSFER_BIT_EXT;

		/**
		* @brief Instance the Resource.
		*/
		resourceptr->CreateImage(
			resourceptr->m_VulkanState,
			fileName                           ,
			VK_IMAGE_TYPE_2D                   ,
			resourceptr->m_Width               ,
			resourceptr->m_Height              ,
			1                                  ,          // 1 layers.
			VK_SAMPLE_COUNT_1_BIT              ,          // No MASS.
			VK_FORMAT_R8G8B8A8_UNORM           ,          // RGBA8 Format.
			VK_IMAGE_TILING_OPTIMAL            ,          // Tiling Optimal.
			usage                              ,
			0                                  ,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			resourceptr->m_MipLevels
		);

		/**
		* @brief Transform Image Layout from undefined to transfer_dst.
		*/
		resourceptr->TransitionImageLayout(
			resourceptr->m_Format,
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
		);

		/**
		* @brief Get Texture bytes.
		* @note 4 means 4 channels per texel, 1 means 1 bytes per texel channel.(RGBA8 Format support only)
		*/
		VkDeviceSize imageSize = static_cast<uint64_t>(resourceptr->m_Width * resourceptr->m_Height * 4 * 1);

		/**
		* @brief Instance a staginBuffer.
		*/
		VulkanBuffer stagingBuffer(
			resourceptr->m_VulkanState,
			"StagingBuffer",
			imageSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		);

		/**
		* @brief Copy the data from texture bytes to staginBuffer.
		*/
		stagingBuffer.WriteToBuffer(pixels);

		/**
		* @brief Copy the data from staginBuffer to Image.
		*/
		resourceptr->CopyBufferToImage(
			stagingBuffer.Get()                         , 
			resourceptr->m_Image                        , 
			static_cast<uint32_t>(resourceptr->m_Width) , 
			static_cast<uint32_t>(resourceptr->m_Height)
		);

		/**
		* @brief Release texture bytes.
		*/
		stbi_image_free(pixels);

		/**
		* @brief Generate Image Mipmaps.
		*/
		resourceptr->GenerateMipmaps(
			resourceptr->m_Format,
			resourceptr->m_Width,
			resourceptr->m_Height
		);

		/**
		* @brief Create Image View.
		*/
		resourceptr->CreateImageView(
			resourceptr->m_Format     ,
			VK_IMAGE_VIEW_TYPE_2D     ,
			VK_IMAGE_ASPECT_COLOR_BIT
		);

		/**
		* @brief Create Image Sampler.
		*/
		resourceptr->CreateSampler();

		auto deviceCopyF = [&](uint32_t w, uint32_t h, int mip, std::vector<unsigned char>& data) {

			/**
			* @brief Instance a VkBufferImageCopy.
			*/
			VkBufferImageCopy region{};
			region.bufferOffset                    = 0;
			region.bufferRowLength                 = 0;
			region.bufferImageHeight               = 0;
		
			region.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
			region.imageSubresource.mipLevel       = resourceptr->m_MipLevels - 1 - mip;
			region.imageSubresource.baseArrayLayer = 0;
			region.imageSubresource.layerCount     = resourceptr->m_Layers;
		
			region.imageOffset.x                   = 0;
			region.imageOffset.y                   = 0;
			region.imageExtent                     = { w, h, 1 };
		
			uint32_t size = w * h * 4;
		
			/**
			* @brief The temp buffer image date copy to.
			*/
			VulkanBuffer stagingbuffer(
				resourceptr->m_VulkanState,
				"StagingBuffer",
				size,
				VK_BUFFER_USAGE_TRANSFER_DST_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
			);
			
			resourceptr->CopyImageToBuffer(stagingbuffer.Get(), { region });
	
			stagingbuffer.WriteFromBuffer(reinterpret_cast<void*>(data.data()));
		};

		auto hostCopyF = [&](uint32_t w, uint32_t h, int mip, std::vector<unsigned char>& data) {

			/**
			* @brief Instance a VkImageToMemoryCopyEXT.
			*/
			VkImageToMemoryCopyEXT                           memoryCopy{};
			memoryCopy.sType                               = VK_STRUCTURE_TYPE_IMAGE_TO_MEMORY_COPY_EXT;
			memoryCopy.imageSubresource.aspectMask         = VK_IMAGE_ASPECT_COLOR_BIT;
			memoryCopy.imageSubresource.mipLevel           = resourceptr->m_MipLevels - 1 - mip;
			memoryCopy.imageSubresource.baseArrayLayer     = 0;
			memoryCopy.imageSubresource.layerCount         = resourceptr->m_Layers;
			memoryCopy.imageOffset                         = { 0, 0, 0 };
			memoryCopy.imageExtent                         = { w, h, 1 };
			memoryCopy.pHostPointer                        = data.data();

			resourceptr->CopyImageToMemoryHost({ memoryCopy });
		};

		ktxTexture2* ktxTexture = Transcoder::CreateKTX2Texture(resourceptr->m_Width, resourceptr->m_Height);
		for (int i = 0; i < resourceptr->m_MipLevels; i++)
		{
			uint32_t w      = std::max(1, resourceptr->m_Width  >> resourceptr->m_MipLevels - 1 - i);
			uint32_t h      = std::max(1, resourceptr->m_Height >> resourceptr->m_MipLevels - 1 - i);
			uint32_t size   = w * h * 4;

			std::vector<unsigned char> data;
			data.resize(size);

			if (hostCopy)
			{
				hostCopyF(w, h, i, data);
			}
			else
			{
				deviceCopyF(w, h, i, data);
			}

			/**
			* @brief Write mipmap data to ktxTexture.
			*/
			Transcoder::WriteData(ktxTexture, resourceptr->m_MipLevels - 1 - i, data.data(), size);
		}

		/**
		* @brief Save to disk.
		*/
		Transcoder::SaveToDisk(ktxTexture, binPath);

		return true;
	}
}