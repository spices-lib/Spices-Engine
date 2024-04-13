#pragma once
#include "Core/Core.h"
#include "Core/UUID.h"
#include "entt.hpp"

#include "World/Components/CameraComponent.h"
#include "World/Components/TransformComponent.h"
#include "World/Components/MeshComponent.h"
#include "World/Components/TagComponent.h"
#include "World/Components/UUIDComponent.h"
#include "World/Components/DirectionalLightComponent.h"
#include "World/Components/PointLightComponent.h"
#include "World/Components/SkyBoxComponent.h"

namespace Spiecs {

	class Entity;

	class World
	{
	public:
		World() {};
		virtual ~World() {};

		virtual void OnPreActivate() = 0;

		virtual void OnActivate(TimeStep& ts) = 0;
		virtual void OnDeactivate() = 0;

		// empty entity
		Entity CreateEntity(const std::string& name = "None");
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = "None");
		void DestroyEntity(Entity entity);

		entt::registry& GetRegistry() { return m_Registry; };
		
	private:
		template<typename T>
		void OnComponentAdded(Entity* entity, T& component);

	protected:
		entt::registry m_Registry;
		std::unordered_map<UUID, entt::entity> m_EntityMap;

		friend class Entity;
	};

	template<typename T>
	inline void World::OnComponentAdded(Entity* entity, T& component)
	{
		component.OnComponentAdded(*entity);
	}
}
