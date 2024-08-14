/**
* @file World.h.
* @brief The World Class Definitions.
* @author Spices.
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
#include "World/Components/NativeScriptComponent.h"
#include "World/Components/PointLightComponent.h"
#include "World/Components/SkyBoxComponent.h"
#include "World/Components/SpriteComponent.h"

namespace Spices {

	/**
	* Forward Declare
	*/
	class Entity;

	/**
	* @brief World Class.
	* This class defines the basic behaves of World.
	* When we create an new world, we need inherit from this.
	*/
	class World
	{
	public:

		enum WorldMarkBits
		{
			Clean            = 0x00000001,
			MeshAddedToWorld = 0x00000002,
			FrushStableFrame = 0x00000004,
			NeedUpdateTLAS   = 0x00000008,
			ReBuildMaterial  = 0x00000010,
			MAX              = 0x7FFFFFFF
		};

		typedef uint32_t WorldMarkFlags;

	public:

		/**
		* @brief Constructor Function.
		*/
		World() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~World() = default;

		/**
		* @brief This interface define the specific world behaves before on activated.
		*/
		virtual void OnPreActivate() = 0;

		/**
		* @brief This interface define the specific world behaves on activated.
		* @param[in] ts TimeStep.
		*/
		virtual void OnActivate(TimeStep& ts) = 0;

		/**
		* @brief This interface defines the specific world behaves after on activated.
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
		entt::registry& GetRegistry() { return m_Registry; }

		/**
		* @brief Get World Entity by id(entt::entity).
		* @param[in] id Id(entt::entity)
		* @return Returns valid Entity if fined.
		*/
		Entity QueryEntitybyID(uint32_t id);

		/**
		* @brief Registry a meshpack to BaseMeshMap.
		* @param[in] materialName meshpack's material name.
		* @param[in] uuid meshpack's uuid.
		*/
		void RegistryBaseMesh(const std::string& materialName, UUID uuid);

		/**
		* @brief Get WorldMarkFlags this frame.
		* @return Returns the WorldMarkFlags this frame.
		*/
		WorldMarkFlags GetMarker() const { return m_Marker; }

		/**
		* @brief Mark WorldMarkFlags with flags.
		* @param[in] flags In flags.
		*/
		void Mark(WorldMarkFlags flags) { m_Marker |= flags; }

		/**
		* @brief Reset WorldMarkFlags to Clean.
		*/
		void ReserMark() { m_Marker = WorldMarkBits::Clean; }

		/**
		* @brief Clear WorldMarkFlags with flags.
		* @param[in] flags In flags.
		*/
		void ClearMarkerWithBits(WorldMarkFlags flags);
		
	private:

		/**
		* @brief Called On any Component Added to this world.
		* @param[in] entity Entity row pointer.
		* @param[in] component Specific Component reference.
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
		* @brief Container of Material - [ MeshPack UUID - Any ]
		*/
		std::unordered_map<std::string, std::unordered_map<UUID, int>> m_BaseMeshMap;

		/**
		* Allow Entity access all data.
		*/
		friend class Entity;

		/**
		* @brief World State this frame.
		*/
		WorldMarkFlags m_Marker = WorldMarkBits::Clean;
	};

	template<typename T>
	void World::OnComponentAdded(Entity* entity, T& component)
	{
		component.OnComponentAdded(*entity);
	}
}
