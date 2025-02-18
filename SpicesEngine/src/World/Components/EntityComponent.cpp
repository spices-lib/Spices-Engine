/**
* @file EntityComponent.cpp.
* @brief The EntityComponent Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "EntityComponent.h"
#include "Render/FrameInfo.h"
#include "World/World/World.h"
#include "World/Entity.h"

namespace Spices {

	void EntityComponent::OnSerialize()
	{
		
	}
	void EntityComponent::OnDeSerialize()
	{
		
	}

	void EntityComponent::DrawThis()
	{
		SPICES_PROFILE_ZONE;
	}

	void EntityComponent::AddEntity(uint32_t entity)
	{
		SPICES_PROFILE_ZONE;

		m_Entities.insert(entity);
		
		Entity e((entt::entity)entity, FrameInfo::Get().m_World.get());
		e.RemoveFromRoot();
	}

	void EntityComponent::RemoveEntity(uint32_t entity)
	{
		SPICES_PROFILE_ZONE;

		m_Entities.erase(entity);
		Entity e((entt::entity)entity, FrameInfo::Get().m_World.get());
		e.AddToRoot();
	}
}
