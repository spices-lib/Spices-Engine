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

		Entity entity(m_Registry.create(), this);

		/**
		* @brief Add UUIDComponent default.
		*/
		entity.AddComponent<UUIDComponent>();

		/**
		* @brief Add TransformComponent default.
		*/
		entity.AddComponent<TransformComponent>();

		/**
		* @brief AddTagComponent default.
		*/
		entity.AddComponent<TagComponent>(name);

		m_EntityMap[uuid] = entity;
		return entity;
	}

	void World::DestroyEntity(Entity& entity)
	{
		SPICES_PROFILE_ZONE;

		m_Registry.destroy(entity);
		m_EntityMap.erase(entity.GetUUID());
	}

	Entity World::QueryEntitybyID(uint32_t id)
	{
		SPICES_PROFILE_ZONE;

		return id == -1 ? Entity() : Entity((entt::entity)id, this);
	}

	void World::RegistryBaseMesh(std::shared_ptr<MeshPack> meshPack)
	{
		SPICES_PROFILE_ZONE;

		m_BaseMeshMap[meshPack->GetMaterial()->GetName()][meshPack->GetUUID()] = meshPack.get();
	}

	void World::ClearMarkerWithBits(WorldMarkFlags flags)
	{
		SPICES_PROFILE_ZONE;

		if (m_Marker & flags)
		{
			m_Marker ^= flags;
		}
	}
}
