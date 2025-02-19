/**
* @file World.h.
* @brief The World Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Core/UUID.h"
#include "entt.hpp"
#include "World/WorldFunctions/WorldFunctions.h"

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
#include "World/Components/EntityComponent.h"
#include "World/Components/LandscapeComponent.h"
#include "World/Components/ParticleComponent.h"
#include "World/Components/WidgetComponent.h"

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

		/**
		* @brief View all component in this world.
		* @tparam T Component.
		* @param fn View function.
		*/
		template<typename T, typename F>
		void ViewComponent(F&& fn);

		/**
		* @brief View all component in this world in ranges.
		* @tparam T Component.
		* @param[in] ranges view ranges.
		* @param fn View function.
		*/
		template<typename T, typename F>
		void ViewComponent(const std::vector<uint32_t>& ranges, F&& fn);

		/**
		* @brief View all component in this world in ranges.
		* @tparam T Component.
		* @param[in] ranges view ranges.
		* @param[in] floor ranges floor.
		* @param[in] ceil ranges ceil.
		* @param fn View function.
		*/
		template<typename T, typename F>
		void ViewComponent(const std::vector<uint32_t>& ranges, uint32_t floor, uint32_t ceil, F&& fn);

		/**
		* @brief View all root in this world.
		* @param fn View function.
		*/
		template<typename F>
		void ViewRoot(F&& fn);

		/**
		* @brief Template Function.
		* Used for add specific component to entity.
		* @tparam T Specific component.
		* @param[in] e entt::entity.
		* @param[in] args Component construct parameters.
		* @return Returns The specific component reference that added.
		*/
		template<typename T, typename... Args>
		T& AddComponent(entt::entity e, Args&&... args);

		/**
		* @brief Get Component owned by this entity.
		* @tparam T Which Component we will get.
		* @param[in] e entt::entity.
		* @return Returns the specific Component.
		*/
		template<typename T>
		T& GetComponent(entt::entity e);

		/**
		* @brief Remove Component owned from this entity.
		* @tparam T Which Component we will remove.
		* @param[in] e entt::entity.
		*/
		template<typename T>
		void RemoveComponent(entt::entity e);

		/**
		* @brief If Component is owned by this entity or not.
		* @tparam T Which Component we will search.
		* @param[in] e entt::entity.
		* @return Returns true if found.
		*/
		template<typename T>
		bool HasComponent(entt::entity e);
		
		/**
		* @brief Remove a entity from this world root.
		* @param[in] entity Entity.
		*/
		void RemoveFromRoot(Entity& entity);

		/**
		* @brief Add a entity to this world root.
		* @param[in] entity Entity.
		*/
		void AddToRoot(Entity& entity);

		/**
		* @brief Determine if a entity is in root.
		* @param[in] entity Entity.
		*/
		bool IsRootEntity(Entity& entity);

	private:

		Entity CreateEmptyEntity(UUID uuid);
		
		/**
		* @brief Called On any Component Added to this world.
		* @param[in] entity Entity row pointer.
		* @param[in] component Specific Component reference.
		*/
		template<typename T>
		void OnComponentAdded(Entity* entity, T& component);

	protected:

		/**
		* @brief Mutex for world.
		*/
		std::shared_mutex m_Mutex;

		/**
		* @brief This variable handles all entity.
		*/
		entt::registry m_Registry;

		/**
		* @brief This variable is a cache.
		* @noto Not in use now.
		* @todo use it.
		*/
		std::unordered_map<UUID, entt::entity> m_RootEntityMap;

		/**
		* Allow Entity access all data.
		*/
		friend class Entity;

		/**
		* Allow WorldFunctions access all data.
		*/
		friend class WorldFunctions;

		/**
		* @brief World State this frame.
		*/
		WorldMarkFlags m_Marker = WorldMarkBits::Clean;
	};

	template <typename T, typename F>
	void World::ViewComponent(F&& fn)
	{
		SPICES_PROFILE_ZONE;
		
		std::shared_lock<std::shared_mutex> lock(m_Mutex);
		
		auto view = m_Registry.view<T>();

		for(auto e : view)
		{
			auto& comp = m_Registry.get<T>(e);
			
			if(fn(e, comp)) break;
		}
	}

	template <typename T, typename F>
	void World::ViewComponent(const std::vector<uint32_t>& ranges, F&& fn)
	{
		SPICES_PROFILE_ZONE;
		
		std::shared_lock<std::shared_mutex> lock(m_Mutex);

		for(auto range : ranges)
		{
			auto e = static_cast<entt::entity>(range);
			auto& comp = m_Registry.get<T>(e);

			fn(e, comp);
		}
	}

	template<typename T, typename F>
	inline void World::ViewComponent(const std::vector<uint32_t>& ranges, uint32_t floor, uint32_t ceil, F&& fn)
	{
		SPICES_PROFILE_ZONE;

		std::shared_lock<std::shared_mutex> lock(m_Mutex);

		assert(floor >= 0);
		assert(ceil >= floor);
		assert(ceil <= ranges.size() - 1);

		for(int32_t i = floor; i < ceil; i++)
		{
			auto e = static_cast<entt::entity>(ranges[i]);
			auto& comp = m_Registry.get<T>(e);

			fn(e, comp);
		}
	}

	template<typename F>
	inline void World::ViewRoot(F&& fn)
	{
		SPICES_PROFILE_ZONE;

		std::shared_lock<std::shared_mutex> lock(m_Mutex);

		for (auto entity : m_RootEntityMap)
		{
			Entity e(entity.second, this);
			fn(e);
		}
	}

	template <typename T, typename ... Args>
	T& World::AddComponent(entt::entity e, Args&&... args)
	{
		SPICES_PROFILE_ZONE;

		std::unique_lock<std::shared_mutex> lock(m_Mutex);

		return m_Registry.emplace<T>(e, std::forward<Args>(args)...);
	}

	template <typename T>
	T& World::GetComponent(entt::entity e)
	{
		SPICES_PROFILE_ZONE;
		
		/**
		* @note lock cause bug here.
		*/
		//std::shared_lock<std::shared_mutex> lock(m_Mutex);

		return m_Registry.get<T>(e);
	}

	template <typename T>
	void World::RemoveComponent(entt::entity e)
	{
		SPICES_PROFILE_ZONE;

		std::unique_lock<std::shared_mutex> lock(m_Mutex);
		
		m_Registry.remove<T>(e);
	}

	template <typename T>
	bool World::HasComponent(entt::entity e)
	{
		SPICES_PROFILE_ZONE;

		std::shared_lock<std::shared_mutex> lock(m_Mutex);

		return m_Registry.all_of<T>(e);
	}

	template<typename T>
	void World::OnComponentAdded(Entity* entity, T& component)
	{
		SPICES_PROFILE_ZONE;
		
		component.OnComponentAdded(*entity);
	}
}
