#include "pchheader.h"
#include "World.h"
#include "World/Entity.h"

#include "World/Components/CameraComponent.h"
#include "World/Components/MeshComponent.h"
#include "World/Components/TagComponent.h"
#include "World/Components/TransformComponent.h"
#include "World/Components/UUIDComponent.h"

namespace Spiecs {

	Entity World::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity World::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity(m_Registry.create(), this, name);
		entity.AddComponent<UUIDComponent>();
		entity.AddComponent<TransformComponent>();

		m_EntityMap[uuid] = entity;
		return entity;
	}

	void World::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
		m_EntityMap.erase(entity.GetUUID());
	}
}