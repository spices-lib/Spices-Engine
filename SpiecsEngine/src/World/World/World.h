#pragma once
#include "Core/Core.h"
#include "Core/UUID.h"
#include "entt.hpp"

namespace Spiecs {

	class Entity;

	class World
	{
	public:
		World() {};
		virtual ~World() {};

		virtual void OnPreActivate() = 0;
		virtual void OnActivate() = 0;
		virtual void OnDeactivate() = 0;

		// empty entity
		Entity CreateEntity(const std::string& name = "None");
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = "None");
		void DestroyEntity(Entity entity);

		entt::registry& GetRegistry() { return m_Registry; };

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	protected:
		entt::registry m_Registry;
		std::unordered_map<UUID, entt::entity> m_EntityMap;

		friend class Entity;
	};

	template<typename T>
	inline void World::OnComponentAdded(Entity entity, T& component)
	{}
}
