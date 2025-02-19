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

namespace Spices {

	/**
	* @brief Entity Class.
	* This class defines the specific behaves of Entity.
	*/
	class Entity
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		Entity() = default;

		/**
		* @brief Constructor Function.
		* Init class variable.
		* Usually call it.
		* @param[in] handle entt::entity, ESC identify.
		* @param[in] world World pointer.
		*/
		Entity(entt::entity handle, World* world)
			: m_EntityHandle(handle)
			, m_World(world)
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~Entity() = default;

		/**
		* @brief Template Function.
		* Used for add specific component to entity.
		* @tparam T Specific component.
		* @param[in] args Component construct parameters.
		* @return Returns The specific component reference that added.
		*/
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			if (HasComponent<T>())
			{
				std::stringstream ss;
				ss << "Entity: " << (int)m_EntityHandle << " already has such component.";

				SPICES_CORE_WARN(ss.str())
				return GetComponent<T>();
			}

			T& component = m_World->AddComponent<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_World->OnComponentAdded<T>(this, component);
			return component;
		}

		/**
		* @brief Get Component owned by this entity.
		* @tparam T Which Component we will get.
		* @return Returns the specific Component.
		*/
		template<typename T>
		T& GetComponent() const
		{
			return m_World->GetComponent<T>(m_EntityHandle);
		}

		/**
		* @brief Remove Component owned from this entity.
		* @tparam T Which Component we will remove.
		*/
		template<typename T>
		void RemoveComponent() const
		{
			m_World->RemoveComponent<T>(m_EntityHandle);
		}

		/**
		* @brief Remove a entity from this world root.
		*/
		void RemoveFromRoot()
		{
			m_World->RemoveFromRoot(*this);
		}

		/**
		* @brief Add a entity to this world root.
		*/
		void AddToRoot()
		{
			m_World->AddToRoot(*this);
		}

		/**
		* @brief If Component is owned by this entity or not.
		* @tparam T Which Component we will search.
		* @return Returns true if found.
		*/
		template<typename T>
		bool HasComponent() const
		{
			return m_World->HasComponent<T>(m_EntityHandle);
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
		operator bool() const { return m_EntityHandle != entt::null; }

		/**
		* @brief Empty Operation.
		* @return Returns m_EntityHandle's value.
		*/
		operator uint32_t() const { return static_cast<uint32_t>(m_EntityHandle); }

		/**
		* @brief Empty Operation.
		* @return Returns m_EntityHandle.
		*/
		operator entt::entity() const { return m_EntityHandle; }

		/**
		* @brief Equal Operation.
		* @param[in] other Another Entity.
		* @return Returns true if euqal.
		*/
		bool operator ==(const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle && m_World == other.m_World;
		}

		/**
		* @brief Not equal Operation.
		* @param[in] other Another Entity.
		* @return Returns true if not equal.
		*/
		bool operator !=(const Entity& other) const
		{
			return !operator==(other);
		}

	private:

		/**
		* @brief This entity's identify in ECS.
		*/
		entt::entity m_EntityHandle{ entt::null };

		/**
		* @brief A specific world Pointer.
		*/
		World* m_World = nullptr;
	};
}