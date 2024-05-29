/**
* @file Component.cpp.
* @brief The Component Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "Component.h"
#include "Resources/ResourcePool/ResourcePool.h"
#include "Resources/Texture/Texture2D.h"
#include "Render/Vulkan/VulkanImage.h"
#include "Slate/Imgui/ImguiHelper.h"

namespace Spiecs {

	Component::Component()
	{
		ZoneScoped;

		/**
		* @brief Set m_SlateStageIconPath.
		*/
		m_SlateStageIconPath = DefineStageIcon();
	}

	void Component::OnComponentAdded(const entt::entity& entity)
	{
		ZoneScoped;

		/**
		* @brief Set m_Owner Variable.
		*/
		m_Owner = entity;

		if (!m_SlateStageIconPath.empty())
		{
			/**
			* @brief Load Specific icon.
			*/
			LoadSlateIcon(m_SlateStageIcon, m_SlateStageIconPath);
		}
		else
		{
			/**
			* @brief Load Default icon.
			*/
			LoadSlateIcon(m_SlateStageIcon, "default.jpg");
		}
	}

	void Component::LoadSlateIcon(ImTextureID& id, const std::string& iconFile)
	{
		ZoneScoped;

		/**
		* @brief Get info from ResourcePool.
		*/
		auto rowPtr = ResourcePool<Texture>::Load<Texture2D>(iconFile);
		auto info = rowPtr->GetResource<VulkanImage>()->GetImageInfo();

		/**
		* @brief Create DescriptorSet.
		*/
		id = ImGui_ImplVulkan_AddTexture(info->sampler, info->imageView, info->imageLayout);
	}
}