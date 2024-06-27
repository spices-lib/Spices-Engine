/**
* @file Texture2D.cpp.
* @brief The Texture2D Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "Texture2D.h"
#include "Render/Vulkan/VulkanImage.h"
#include "Render/Vulkan/VulkanRenderBackend.h"
#include "Render/RendererResource/RendererResource.h"

namespace Spiecs {

	Texture2D::Texture2D(const RendererResourceCreateInfo& info)
	{
		m_ResourcePath = "NONE";

		if (!info.isDepthResource)
		{
			m_Resource = std::make_shared<VulkanImage>(
				VulkanRenderBackend::GetState(),
				info.name,
				VK_IMAGE_TYPE_2D,
				info.width,
				info.height,
				1,
				info.description.samples,
				info.description.format,
				VK_IMAGE_TILING_OPTIMAL,
				info.usage                            |
				VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT   |  // Can be used for ColorAttachment.
				VK_IMAGE_USAGE_SAMPLED_BIT            |  // Can be used for ShaderRead.
				VK_IMAGE_USAGE_TRANSFER_SRC_BIT       |  // Can be used for TransferSrc.        /*todo: Configurable*/
				VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,     // Can be used for InputAttachment.
				0,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
				1
			);

			auto resourceptr = GetResource<VulkanImage>();
			resourceptr->CreateImageView(info.description.format, VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT);
			
			if (info.usage & VK_IMAGE_USAGE_STORAGE_BIT)
			{
				resourceptr->TransitionImageLayout(
					info.description.format,
					VK_IMAGE_LAYOUT_UNDEFINED,
					VK_IMAGE_LAYOUT_GENERAL
				);
			}

			resourceptr->CreateSampler();
		}
		else
		{
			m_Resource = std::make_shared<VulkanImage>(
				VulkanRenderBackend::GetState(),
				info.name,
				VK_IMAGE_TYPE_2D,
				info.width,
				info.height,
				1,
				info.description.samples,
				info.description.format,
				VK_IMAGE_TILING_OPTIMAL,
				info.usage                                    |
				VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT   | // Can be used for DepthAttachment.
				VK_IMAGE_USAGE_SAMPLED_BIT                    | // Can be used for ShaderRead.
				VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,            // Can be used for InputAttachment.
				0,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
				1
			);

			auto resourceptr = GetResource<VulkanImage>();
			resourceptr->CreateImageView(info.description.format, VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_DEPTH_BIT);

			resourceptr->TransitionImageLayout(
				info.description.format,
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
			);

			resourceptr->CreateSampler();
		}
	}

	Texture2D::Texture2D(const std::string& path)
	{
		m_ResourcePath = path;
		TextureLoader::Load(m_ResourcePath, this);
	}
}