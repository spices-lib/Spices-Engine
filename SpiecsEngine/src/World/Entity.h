#pragma once
#include "Core/Core.h"
#include "entt.hpp"

namespace Spiecs {

	class World;

	class Entity
	{
	public:
		Entity(const std::string& entityName) : m_EntityName(entityName) {};
		virtual ~Entity() {};

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			T& component = m_World->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_World->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			return m_World->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			m_World->m_Registry.remove<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_World->m_Registry.all_of<T>(m_EntityHandle);
		}

	private:
		entt::entity m_EntityHandle{ entt::null };
		std::shared_ptr<World> m_World;
		std::string m_EntityName;
	};
}