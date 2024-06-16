/**
* @file Texture2D.cpp.
* @brief The Texture2D Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "Texture2DCube.h"
#include "Render/Vulkan/VulkanImage.h"
#include "Render/Vulkan/VulkanRenderBackend.h"
#include "Render/RendererResource/RendererResource.h"

namespace Spiecs {

	Texture2DCube::Texture2DCube(const RendererResourceCreateInfo& info)
	{
		m_ResourcePath = "NONE";

		if (!info.isDepthResource)
		{
			m_Resource = std::make_shared<VulkanImage>(
				VulkanRenderBackend::GetState(),
				info.name,
				VK_IMAGE_TYPE_2D,
				glm::max(info.width, info.height),
				glm::max(info.width, info.height),
				6,
				info.description.samples,
				info.description.format,
				VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |    // Can be used for ColorAttachment.
				VK_IMAGE_USAGE_SAMPLED_BIT          |    // Can be used for ShaderRead.
				VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT |    // Can be used for InputAttachment.
				VK_IMAGE_USAGE_TRANSFER_SRC_BIT,         // Can be used for TransferSrc.
				VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
				1
			);

			auto resourceptr = GetResource<VulkanImage>();
			resourceptr->CreateImageView(info.description.format, VK_IMAGE_VIEW_TYPE_CUBE, VK_IMAGE_ASPECT_COLOR_BIT);

			resourceptr->CreateSampler();
		}
		//else
		//{
		//	m_Resource = std::make_shared<VulkanImage>(
		//		VulkanRenderBackend::GetState(),
		//		info.name,
		//		VK_IMAGE_TYPE_2D,
		//		glm::max(info.width, info.height),
		//		glm::max(info.width, info.height),
		//		6,
		//		info.description.samples,
		//		info.description.format,
		//		VK_IMAGE_TILING_OPTIMAL,
		//		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT |   // Can be used for DepthAttachment.
		//		VK_IMAGE_USAGE_SAMPLED_BIT |                    // Can be used for ShaderRead.
		//		VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,            // Can be used for InputAttachment.
		//		VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
		//		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		//		1
		//	);

		//	auto resourceptr = GetResource<VulkanImage>();
		//	resourceptr->CreateImageView(info.description.format, VK_IMAGE_VIEW_TYPE_CUBE, VK_IMAGE_ASPECT_DEPTH_BIT);

		//	resourceptr->TransitionImageLayout(
		//		info.description.format,
		//		VK_IMAGE_LAYOUT_UNDEFINED,
		//		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
		//	);

		//	resourceptr->CreateSampler();
		//}
	}

	Texture2DCube::Texture2DCube(const std::string& path)
	{
		m_ResourcePath = path;
		TextureLoader::Load(m_ResourcePath, this);
	}
}