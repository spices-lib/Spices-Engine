#pragma once
#include "Core/Core.h"
#include "entt.hpp"
#include "World/Components/UUIDComponent.h"
#include "World/World.h"
#include <string>

namespace Spiecs {

	class Entity
	{
	public:
		Entity(entt::entity handle, World* world, const std::string& entityName)
			: m_EntityHandle(handle)
			, m_World(world)
		{};

		virtual ~Entity() {};

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			T& component = m_World->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_World->OnComponentAdded<T>(this, component);
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

		UUID GetUUID() { return GetComponent<UUIDComponent>().GetUUID(); }

		operator bool() const { return m_EntityHandle != entt::null; };
		operator uint32_t() const { return (uint32_t)m_EntityHandle; };
		operator entt::entity() const { return m_EntityHandle; };

		bool operator ==(const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle && m_World == other.m_World;
		};

		bool operator !=(const Entity& other) const
		{
			return !operator==(other);
		};

	private:
		entt::entity m_EntityHandle{ entt::null };
		World* m_World;
	};
}