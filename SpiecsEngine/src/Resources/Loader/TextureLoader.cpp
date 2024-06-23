/**
* @file TextureLoader.cpp.
* @brief The TextureLoader Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "TextureLoader.h"
#include "Render/Vulkan/VulkanImage.h"
#include "Render/Vulkan/VulkanRenderBackend.h"
#include "Core/Library/FileLibrary.h"

#include <stb_image.h>

#include "Resources/Texture/Texture2D.h"
#include "Resources/Texture/Texture2DCube.h"

namespace Spiecs {

	/**
	* @brief Const variable: Original Image File Path.
	*/
	const std::string defaultTexturePath = SPIECS_ENGINE_ASSETS_PATH + "Textures/src/";

	void TextureLoader::Load(const std::string& filePath, Texture2D* outTexture)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Get full texture path.
		*/
		const std::string _filePath = defaultTexturePath + filePath;

		/**
		* @brief Return if file is not find.
		*/
		if (!FileLibrary::FileLibrary_Exists(_filePath.c_str()))
		{
			std::stringstream ss;
			ss << "File: " << _filePath << " Not Find";

			SPIECS_CORE_ERROR(ss.str());
			return;
		}

		/**
		* @brief Instance the VulkanImage as Texture2D Resource.
		*/
		outTexture->m_Resource = std::make_shared<VulkanImage>(VulkanRenderBackend::GetState());
		auto resourceptr = outTexture->GetResource<VulkanImage>();

		/**
		* @brief Load Texture data.
		*/
		int texChannels;
		stbi_uc* pixels = stbi_load(_filePath.c_str(), &resourceptr->m_Width, &resourceptr->m_Height, &texChannels, STBI_rgb_alpha);
		if (!pixels)
		{
			SPIECS_CORE_ERROR("Failed to load texture image!");
		}

		/**
		* @brief Set resource miplevels.
		*/
		resourceptr->m_MipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(resourceptr->m_Width, resourceptr->m_Height)))) + 1;

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
			imageSize, 
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		);

		/**
		* @brief Copy the data from texture bytes to staginBuffer.
		*/
		void* data;
		vkMapMemory(resourceptr->m_VulkanState.m_Device, stagingBuffer.GetMomory(), 0, imageSize, 0, &data);
		memcpy(data, pixels, static_cast<size_t>(imageSize));
		vkUnmapMemory(resourceptr->m_VulkanState.m_Device, stagingBuffer.GetMomory());

		/**
		* @brief Release texture bytes.
		*/
		stbi_image_free(pixels);

		/**
		* @brief Instance the Resource.
		*/
		resourceptr->CreateImage(
			resourceptr->m_VulkanState,
			filePath,
			VK_IMAGE_TYPE_2D,
			resourceptr->m_Width,
			resourceptr->m_Height,
			1,                                         // 1 layers.
			VK_SAMPLE_COUNT_1_BIT,                     // No MASS.
			VK_FORMAT_R8G8B8A8_UNORM,                  // RGBA8 Format.
			VK_IMAGE_TILING_OPTIMAL,                   // Tiling Optimal.
			VK_IMAGE_USAGE_TRANSFER_SRC_BIT |          // Can be Used for Transfer_src
			VK_IMAGE_USAGE_TRANSFER_DST_BIT |          // Can be Used for Transfer_dst
			VK_IMAGE_USAGE_SAMPLED_BIT,                // Can be Used for Sample
			0,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			resourceptr->m_MipLevels
		);

		/**
		* @brief Transform Image Layout from undefined to transfer_dst.
		*/
		resourceptr->TransitionImageLayout(
			VK_FORMAT_R8G8B8A8_UNORM, 
			VK_IMAGE_LAYOUT_UNDEFINED, 
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
		);

		/**
		* @brief Copy the data from staginBuffer to Image.
		*/
		resourceptr->CopyBufferToImage(
			stagingBuffer.Get(), 
			resourceptr->m_Image, 
			static_cast<uint32_t>(resourceptr->m_Width), 
			static_cast<uint32_t>(resourceptr->m_Height)
		);

		/**
		* @brief Generate Image Mipmaps.
		*/
		resourceptr->GenerateMipmaps(
			VK_FORMAT_R8G8B8A8_UNORM, 
			resourceptr->m_Width, 
			resourceptr->m_Height
		);

		/**
		* @brief Create Image View.
		*/
		resourceptr->CreateImageView(
			VK_FORMAT_R8G8B8A8_UNORM, 
			VK_IMAGE_VIEW_TYPE_2D,
			VK_IMAGE_ASPECT_COLOR_BIT
		);

		/**
		* @brief Create Image Sampler.
		*/
		resourceptr->CreateSampler();
	}

	void TextureLoader::Load(const std::string& filePath, Texture2DCube* outTexture)
	{
	}
}