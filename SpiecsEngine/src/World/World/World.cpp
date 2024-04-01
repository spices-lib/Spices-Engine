#include "pchheader.h"
#include "World.h"

#include "World/Components/CameraComponent.h"
#include "World/Components/MeshComponent.h"
#include "World/Components/TagComponent.h"
#include "World/Components/TransformComponent.h"
#include "World/Components/UUIDComponent.h"

namespace Spiecs {

	Entity World::CreateEntity(const std::string& name)
	{
		Entity entity(name);
		entity.AddComponent<UUIDComponent>();
		entity.AddComponent<TransformComponent>();

		return entity;
	}
	void World::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}
}