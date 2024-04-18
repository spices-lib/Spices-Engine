/**
* @file Entity.h.
* @brief The Entity Class Definitions.
* @author The Cherno.
*/

#pragma once
#include "Core/Core.h"
#include "entt.hpp"
#include "World/Components/UUIDComponent.h"
#include "World/World.h"
#include <string>

namespace Spiecs {

	/**
	* @brief Entity Class.
	* This class defines the specific behaver of Entity.
	*/
	class Entity
	{
	public:

		/**
		* @brief Constructor Function.
		* Init class variable.
		* Usually call it.
		* @param[in] handle entt::entity, ESC identify.
		* @param[in] world World pointer.
		* @param[in] entityName Name.
		* @todo Remove name.
		*/
		Entity(entt::entity handle, World* world, const std::string& entityName)
			: m_EntityHandle(handle)
			, m_World(world)
		{};

		/**
		* @brief Destructor Function.
		*/
		virtual ~Entity() {};

		/**
		* @brief Template Function.
		* Used for add specific component to entity.
		* @param[in] T Specific component.
		* @return Returns The specific component reference that added.
		*/
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			T& component = m_World->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_World->OnComponentAdded<T>(this, component);
			return component;
		}

		/**
		* @brief Get Component owned by this entity.
		* @param[in] T Which Component we will get.
		* @return Returns the specific Component.
		*/
		template<typename T>
		T& GetComponent()
		{
			return m_World->m_Registry.get<T>(m_EntityHandle);
		}

		/**
		* @brief Remove Component owned from this entity.
		* @param[in] T Which Component we will remove.
		*/
		template<typename T>
		void RemoveComponent()
		{
			m_World->m_Registry.remove<T>(m_EntityHandle);
		}

		/**
		* @brief Judje Component is owned by this entity or not.
		* @param[in] T Which Component we will search.
		* @return Returns true if finded.
		*/
		template<typename T>
		bool HasComponent()
		{
			return m_World->m_Registry.all_of<T>(m_EntityHandle);
		}

		/**
		* @brief Get UUID form UUIDComponent.
		* @return Returns UUID.
		*/
		const UUID GetUUID() { return GetComponent<UUIDComponent>().GetUUID(); }

		/**
		* @brief Empty Operation.
		* @return Returns true if m_EntityHandle is valid.
		*/
		operator bool() const { return m_EntityHandle != entt::null; };

		/**
		* @brief Empty Operation.
		* @return Returns m_EntityHandle's value.
		*/
		operator uint32_t() const { return (uint32_t)m_EntityHandle; };

		/**
		* @brief Empty Operation.
		* @return Returns m_EntityHandle.
		*/
		operator entt::entity() const { return m_EntityHandle; };

		/**
		* @brief Equal Operation.
		* @param[in] other Another Entity.
		* @return Returns true if euqal.
		*/
		bool operator ==(const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle && m_World == other.m_World;
		};

		/**
		* @brief Not equal Operation.
		* @param[in] other Another Entity.
		* @return Returns true if not euqal.
		*/
		bool operator !=(const Entity& other) const
		{
			return !operator==(other);
		};

	private:

		/**
		* @brief This entity's identify in ECS.
		*/
		entt::entity m_EntityHandle{ entt::null };

		/**
		* @brief A specific world Pointer.
		*/
		World* m_World;
	};
}