/**
* @file World.cpp.
* @brief The World Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "World.h"
#include "World/Entity.h"

namespace Spiecs {

	Entity World::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity World::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity(m_Registry.create(), this, name);

		/**
		* @brief Add UUIDComponent default.
		*/
		entity.AddComponent<UUIDComponent>();

		/**
		* @brief Add TransformComponent default.
		*/
		entity.AddComponent<TransformComponent>();

		/**
		* @todo Add TagComponent default.
		*/

		m_EntityMap[uuid] = entity;
		return entity;
	}

	void World::DestroyEntity(Entity& entity)
	{
		m_Registry.destroy(entity);
		m_EntityMap.erase(entity.GetUUID());
	}
}