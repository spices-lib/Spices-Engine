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

	bool TextureLoader::SearchFile(const std::string& fileName, std::function<void(const std::string&)> binF, std::function<void(const std::string&)> srcF)
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

		ktxVulkanTexture vkTexture;
		Transcoder::LoadFromKTX(binPath, &vkTexture);

		outTexture->m_Resource       = std::make_shared<VulkanImage>(VulkanRenderBackend::GetState());
		auto resourceptr             = outTexture->GetResource<VulkanImage>();

		resourceptr->m_IsBin         = true;
		resourceptr->m_Image         = vkTexture.image;
		resourceptr->m_Format        = vkTexture.imageFormat;
		resourceptr->m_ImageMemory   = vkTexture.deviceMemory;
		resourceptr->m_Height        = vkTexture.height;
		resourceptr->m_Width         = vkTexture.width;
		resourceptr->m_Layers        = vkTexture.layerCount;
		resourceptr->m_MipLevels     = vkTexture.levelCount;

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
		* @brief Save to disk.
		*/
		Transcoder::SaveSrcToKTX(binPath, pixels, width, height);

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
		* @brief Release texture bytes.
		*/
		stbi_image_free(pixels);

		/**
		* @brief Instance the Resource.
		*/
		resourceptr->CreateImage(
			resourceptr->m_VulkanState,
			fileName,
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

		return true;
	}
}