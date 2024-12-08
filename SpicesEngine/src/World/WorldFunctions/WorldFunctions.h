/**
* @file WorldFunctions.h
* @brief The WorldFunctions Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Core/Thread/ThreadPool.h"
#include "Resources/Mesh/MeshPack.h"
#include "Core/Thread/ThrealModel.h"

namespace Spices {

	/**
	* @brief Forward declare of World.
	*/
	class World;

	/**
	* @brief Forward declare of Entity.
	*/
	class Entity;

	/**
	* @brief World Functions Class.
	*/
	class WorldFunctions
	{
	public:

		/*************************************CreateMesh*****************************************/

		/**
		* @brief Create Entity with a MeshComponent and ConePack Async.
		* @param[in] world Specific World.
		* @param[in] args ConePack construct parameters.
		*/
		template<typename ...Args>
		static void CreateConeEntity(World* world, Args... args);

		/**
		* @brief Create Entity with a MeshComponent and CubePack Async.
		* @param[in] world Specific World.
		* @param[in] args CubePack construct parameters.
		*/
		template<typename ...Args>
		static void CreateCubeEntity(World* world, Args... args);

		/**
		* @brief Create Entity with a MeshComponent and CylinderPack Async.
		* @param[in] world Specific World.
		* @param[in] args CylinderPack construct parameters.
		*/
		template<typename ...Args>
		static void CreateCylinderEntity(World* world, Args... args);

		/**
		* @brief Create Entity with a MeshComponent and DiskPack Async.
		* @param[in] world Specific World.
		* @param[in] args DiskPack construct parameters.
		*/
		template<typename ...Args>
		static void CreateDiskEntity(World* world, Args... args);

		/**
		* @brief Create Entity with a MeshComponent and PlanePack Async.
		* @param[in] world Specific World.
		* @param[in] args PlanePack construct parameters.
		*/
		template<typename ...Args>
		static void CreatePlaneEntity(World* world, Args... args);

		/**
		* @brief Create Entity with a MeshComponent and SpherePack Async.
		* @param[in] world Specific World.
		* @param[in] args SpherePack construct parameters.
		*/
		template<typename ...Args>
		static void CreateSphereEntity(World* world, Args... args);

		/**
		* @brief Create Entity with a MeshComponent and TorusPack Async.
		* @param[in] world Specific World.
		* @param[in] args TorusPack construct parameters.
		*/
		template<typename ...Args>
		static void CreateTorusEntity(World* world, Args... args);

		/****************************************************************************************/

	private:

		static Entity CreateMeshEntity(World* world, const std::string& name, std::shared_ptr<MeshPack> pack);
	};

	template<typename ...Args>
	inline void WorldFunctions::CreateConeEntity(World* world, Args ...args)
	{
	}

	template<typename ...Args>
	inline void WorldFunctions::CreateCubeEntity(World* world, Args ...args)
	{
		SPICES_PROFILE_ZONE;

		AnyscTask(ThreadPoolEnum::Game, [=]() {
			std::shared_ptr<CubePack> pack = std::make_shared<CubePack>(std::forward<Args>(args)...);
			pack->SetMaterial("BasePassRenderer.Mesh.ground");
			CreateMeshEntity(world, "Cube", pack);
		});
	}

	template<typename ...Args>
	inline void WorldFunctions::CreateCylinderEntity(World* world, Args ...args)
	{
	}

	template<typename ...Args>
	inline void WorldFunctions::CreateDiskEntity(World* world, Args ...args)
	{
	}

	template<typename ...Args>
	inline void WorldFunctions::CreatePlaneEntity(World* world, Args ...args)
	{
		SPICES_PROFILE_ZONE;

		std::shared_ptr<PlanePack> pack = std::make_shared<PlanePack>(std::forward<Args>(args)...);
		pack->SetMaterial("BasePassRenderer.Mesh.ground");

		 CreateMeshEntity(world, "Plane", pack);
	}

	template<typename ...Args>
	inline void WorldFunctions::CreateSphereEntity(World* world, Args ...args)
	{
		SPICES_PROFILE_ZONE;

		std::shared_ptr<SpherePack> pack = std::make_shared<SpherePack>(std::forward<Args>(args)...);
		pack->SetMaterial("BasePassRenderer.Mesh.ground");

		CreateMeshEntity(world, "Sphere", pack);
	}

	template<typename ...Args>
	inline void WorldFunctions::CreateTorusEntity(World* world, Args ...args)
	{
	}

}