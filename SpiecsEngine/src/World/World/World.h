/**
* @file World.h.
* @brief The World Class Definitions.
* @author Spiecs.
*/

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

	/**
	* Forward Declare
	*/
	class Entity;

	/**
	* @brief World Class.
	* This class defines the basic behaver of World.
	* When we create an new world, we need inherit from this.
	*/
	class World
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		World() {};

		/**
		* @brief Destructor Function.
		*/
		virtual ~World() {};

		/**
		* @brief This interface define the specific world behaver before on actived.
		*/
		virtual void OnPreActivate() = 0;

		/**
		* @brief This interface define the specific world behaver on actived.
		* @param[in] ts TimeStep.
		*/
		virtual void OnActivate(TimeStep& ts) = 0;

		/**
		* @brief This interface defines the specific world behaver after on actived.
		*/
		virtual void OnDeactivate() = 0;

		/**
		* @brief Create a new empty entity in this world.
		* @param[in] name Entity name.
		*/
		Entity CreateEntity(const std::string& name = "None");

		/**
		* @brief Create a new empty entity with a uuid in this world.
		* @param[in] uuid UUID.
		* @param[in] name Entity name.
		*/
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = "None");

		/**
		* @brief Destroy a entity from this world.
		* @param[in] entity Entity.
		*/
		void DestroyEntity(Entity& entity);

		/**
		* @brief Get Registry variable.
		* @return Returns the Registry variable.
		*/
		inline entt::registry& GetRegistry() { return m_Registry; };
		
	private:

		/**
		* @brief Called On any Component Added to this world.
		* @param[in] entity Entity row pointer.
		* @param[in] T Specific Component reference.
		*/
		template<typename T>
		void OnComponentAdded(Entity* entity, T& component);

	protected:

		/**
		* @brief This variable handles all entity.
		*/
		entt::registry m_Registry;

		/**
		* @brief This variable is a cache.
		* @noto Not in use now.
		* @todo use it.
		*/
		std::unordered_map<UUID, entt::entity> m_EntityMap;

		/**
		* Allow Entity access all data.
		*/
		friend class Entity;
	};

	template<typename T>
	inline void World::OnComponentAdded(Entity* entity, T& component)
	{
		component.OnComponentAdded(*entity);
	}
}
