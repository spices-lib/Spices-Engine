/**
* @file World.cpp.
* @brief The World Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "World.h"
#include "World/Entity.h"

namespace Spices {

	Entity World::CreateEntity(const std::string& name)
	{
		SPICES_PROFILE_ZONE;

		return CreateEntityWithUUID(UUID(), name);
	}

	Entity World::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		SPICES_PROFILE_ZONE;
		
		Entity entity = CreateEmptyEntity(uuid);
		entity.AddComponent<UUIDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(name);
		return entity;
	}

	void World::DestroyEntity(Entity& entity)
	{
		SPICES_PROFILE_ZONE;

		std::unique_lock<std::shared_mutex> lock(m_Mutex);
		
		m_Registry.destroy(entity);
		m_EntityMap.erase(entity.GetUUID());
	}

	Entity World::QueryEntitybyID(uint32_t id)
	{
		SPICES_PROFILE_ZONE;

		return id == -1 ? Entity() : Entity((entt::entity)id, this);
	}

	void World::ClearMarkerWithBits(WorldMarkFlags flags)
	{
		SPICES_PROFILE_ZONE;

		if (m_Marker & flags)
		{
			m_Marker ^= flags;
		}
	}

	Entity World::CreateEmptyEntity(UUID uuid)
	{
		SPICES_PROFILE_ZONE;
		
		std::unique_lock<std::shared_mutex> lock(m_Mutex);
		
		Entity entity(m_Registry.create(), this);
		m_EntityMap[uuid] = entity;
		return entity;
	}
}
