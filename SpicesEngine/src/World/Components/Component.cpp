/**
* @file Component.cpp.
* @brief The Component Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "Component.h"
#include "World/Entity.h"
#include "Resources/ResourcePool/ResourcePool.h"
#include "Resources/Texture/Texture2D.h"
#include "Render/Vulkan/VulkanImage.h"
#include "Slate/Imgui/ImguiHelper.h"

namespace Spices {

	void Component::OnComponentAdded(const entt::entity& entity)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Set m_Owner Variable.
		*/
		m_Owner = entity;
	}
}