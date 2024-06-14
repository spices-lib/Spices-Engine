/**
* @file TextureLoader.cpp.
* @brief The TextureLoader Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "TextureLoader.h"
#include "Render/Vulkan/VulkanImage.h"
#include "Render/Vulkan/VulkanRenderBackend.h"

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
		const std::string _filePath = defaultTexturePath + filePath;

		outTexture->m_Resource = std::make_shared<VulkanImage>(VulkanRenderBackend::GetState());
		auto resourceptr = outTexture->GetResource<VulkanImage>();

		int texChannels;
		stbi_uc* pixels = stbi_load(_filePath.c_str(), &resourceptr->m_Width, &resourceptr->m_Height, &texChannels, STBI_rgb_alpha);
		resourceptr->m_MipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(resourceptr->m_Width, resourceptr->m_Height)))) + 1;

		VkDeviceSize imageSize = resourceptr->m_Width * resourceptr->m_Height * 4;

		if (!pixels) {
			SPIECS_CORE_ERROR("Failed to load texture image!");
		}

		VulkanBuffer stagingBuffer(resourceptr->m_VulkanState, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		void* data;
		vkMapMemory(resourceptr->m_VulkanState.m_Device, stagingBuffer.GetMomory(), 0, imageSize, 0, &data);
		memcpy(data, pixels, static_cast<size_t>(imageSize));
		vkUnmapMemory(resourceptr->m_VulkanState.m_Device, stagingBuffer.GetMomory());

		stbi_image_free(pixels);

		resourceptr->CreateImage(
			resourceptr->m_VulkanState,
			filePath,
			resourceptr->m_Width,
			resourceptr->m_Height,
			VK_SAMPLE_COUNT_1_BIT,
			VK_FORMAT_R8G8B8A8_UNORM,
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			resourceptr->m_MipLevels
		);

		resourceptr->TransitionImageLayout(VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		resourceptr->CopyBufferToImage(stagingBuffer.Get(), resourceptr->m_Image, static_cast<uint32_t>(resourceptr->m_Width), static_cast<uint32_t>(resourceptr->m_Height));

		resourceptr->GenerateMipmaps(VK_FORMAT_R8G8B8A8_UNORM, resourceptr->m_Width, resourceptr->m_Height);

		resourceptr->CreateImageView(VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT);

		resourceptr->CreateSampler();
	}

	void TextureLoader::Load(const std::string& filePath, Texture2DCube* outTexture)
	{
		const std::string _filePath = defaultTexturePath + filePath;

		outTexture->m_Resource = std::make_shared<VulkanImage>(VulkanRenderBackend::GetState());
		auto resourceptr = outTexture->GetResource<VulkanImage>();

		int texChannels;
		stbi_uc* pixels = stbi_load(_filePath.c_str(), &resourceptr->m_Width, &resourceptr->m_Height, &texChannels, STBI_rgb_alpha);
		resourceptr->m_MipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(resourceptr->m_Width, resourceptr->m_Height)))) + 1;

		VkDeviceSize imageSize = resourceptr->m_Width * resourceptr->m_Height * 4;

		if (!pixels) {
			throw std::runtime_error("failed to load texture image!");
		}

		VulkanBuffer stagingBuffer(resourceptr->m_VulkanState, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		void* data;
		vkMapMemory(resourceptr->m_VulkanState.m_Device, stagingBuffer.GetMomory(), 0, imageSize, 0, &data);
		memcpy(data, pixels, static_cast<size_t>(imageSize));
		vkUnmapMemory(resourceptr->m_VulkanState.m_Device, stagingBuffer.GetMomory());

		stbi_image_free(pixels);

		resourceptr->CreateImage(
			resourceptr->m_VulkanState,
			filePath,
			resourceptr->m_Width,
			resourceptr->m_Height,
			VK_SAMPLE_COUNT_1_BIT,
			VK_FORMAT_R8G8B8A8_UNORM,
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			resourceptr->m_MipLevels
		);

		resourceptr->TransitionImageLayout(VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		resourceptr->CopyBufferToImage(stagingBuffer.Get(), resourceptr->m_Image, static_cast<uint32_t>(resourceptr->m_Width), static_cast<uint32_t>(resourceptr->m_Height));

		resourceptr->GenerateMipmaps(VK_FORMAT_R8G8B8A8_UNORM, resourceptr->m_Width, resourceptr->m_Height);

		resourceptr->CreateImageView(VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT);

		resourceptr->CreateSampler();
	}
}