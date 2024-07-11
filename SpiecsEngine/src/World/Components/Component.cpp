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

	void Component::OnComponentAdded(const entt::entity& entity)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Set m_Owner Variable.
		*/
		m_Owner = entity;
	}
}