#pragma once
#include "Core/Core.h"
#include "World/Entity.h"

namespace Spiecs {

	class World
	{
	public:
		World() {};
		virtual ~World() {};

		virtual void OnPreActivate() = 0;
		virtual void OnActivate() = 0;
		virtual void OnDeactivate() = 0;

		// empty entity
		Entity CreateEntity(const std::string& name);
		void DestroyEntity(Entity entity);

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	protected:
		entt::registry m_Registry;
	};

	template<typename T>
	inline void World::OnComponentAdded(Entity entity, T& component)
	{}
}