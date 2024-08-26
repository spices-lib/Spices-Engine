/**
* @file WorldFunctions.h
* @brief The WorldFunctions Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Core/Thread/ThreadPool.h"
#include "Resources/Mesh/MeshPack.h"

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
		* @brief Create Entity with a MeshComponent and ConePack.
		* @param[in] world Specific World.
		* @param[in] args ConePack construct parameters.
		* @return Returns entity created.
		*/
		template<typename ...Args>
		static Entity CreateConeEntity(World* world, Args... args);

		/**
		* @brief Create Entity with a MeshComponent and ConePack Async.
		* @param[in] world Specific World.
		* @param[in] args ConePack construct parameters.
		*/
		template<typename ...Args>
		static void CreateConeEntityAsync(World* world, Args... args);

		/**
		* @brief Create Entity with a MeshComponent and CubePack.
		* @param[in] world Specific World.
		* @param[in] args CubePack construct parameters.
		* @return Returns entity created.
		*/
		template<typename ...Args>
		static Entity CreateCubeEntity(World* world, Args... args);

		/**
		* @brief Create Entity with a MeshComponent and CubePack Async.
		* @param[in] world Specific World.
		* @param[in] args CubePack construct parameters.
		*/
		template<typename ...Args>
		static void CreateCubeEntityAsync(World* world, Args... args);

		/**
		* @brief Create Entity with a MeshComponent and CylinderPack.
		* @param[in] world Specific World.
		* @param[in] args CylinderPack construct parameters.
		* @return Returns entity created.
		*/
		template<typename ...Args>
		static Entity CreateCylinderEntity(World* world, Args... args);

		/**
		* @brief Create Entity with a MeshComponent and CylinderPack Async.
		* @param[in] world Specific World.
		* @param[in] args CylinderPack construct parameters.
		*/
		template<typename ...Args>
		static void CreateCylinderEntityAsync(World* world, Args... args);

		/**
		* @brief Create Entity with a MeshComponent and DiskPack.
		* @param[in] world Specific World.
		* @param[in] args DiskPack construct parameters.
		* @return Returns entity created.
		*/
		template<typename ...Args>
		static Entity CreateDiskEntity(World* world, Args... args);

		/**
		* @brief Create Entity with a MeshComponent and DiskPack Async.
		* @param[in] world Specific World.
		* @param[in] args DiskPack construct parameters.
		*/
		template<typename ...Args>
		static void CreateDiskEntityAsync(World* world, Args... args);

		/**
		* @brief Create Entity with a MeshComponent and PlanePack.
		* @param[in] world Specific World.
		* @param[in] args PlanePack construct parameters.
		* @return Returns entity created.
		*/
		template<typename ...Args>
		static Entity CreatePlaneEntity(World* world, Args... args);

		/**
		* @brief Create Entity with a MeshComponent and PlanePack Async.
		* @param[in] world Specific World.
		* @param[in] args PlanePack construct parameters.
		*/
		template<typename ...Args>
		static void CreatePlaneEntityAsync(World* world, Args... args);

		/**
		* @brief Create Entity with a MeshComponent and SpherePack.
		* @param[in] world Specific World.
		* @param[in] args SpherePack construct parameters.
		* @return Returns entity created.
		*/
		template<typename ...Args>
		static Entity CreateSphereEntity(World* world, Args... args);

		/**
		* @brief Create Entity with a MeshComponent and SpherePack Async.
		* @param[in] world Specific World.
		* @param[in] args SpherePack construct parameters.
		*/
		template<typename ...Args>
		static void CreateSphereEntityAsync(World* world, Args... args);

		/**
		* @brief Create Entity with a MeshComponent and TorusPack.
		* @param[in] world Specific World.
		* @param[in] args TorusPack construct parameters.
		* @return Returns entity created.
		*/
		template<typename ...Args>
		static Entity CreateTorusEntity(World* world, Args... args);

		/**
		* @brief Create Entity with a MeshComponent and TorusPack Async.
		* @param[in] world Specific World.
		* @param[in] args TorusPack construct parameters.
		*/
		template<typename ...Args>
		static void CreateTorusEntityAsync(World* world, Args... args);

		/****************************************************************************************/

	private:

		static Entity CreateMeshEntity(World* world, const std::string& name, std::shared_ptr<MeshPack> pack);
	};

	template<typename ...Args>
	inline Entity WorldFunctions::CreateConeEntity(World* world, Args ...args)
	{
		return Entity();
	}

	template<typename ...Args>
	inline void WorldFunctions::CreateConeEntityAsync(World* world, Args ...args)
	{
	}

	template<typename ...Args>
	inline Entity WorldFunctions::CreateCubeEntity(World* world, Args ...args)
	{
		SPICES_PROFILE_ZONE;

		std::shared_ptr<CubePack> pack = std::make_shared<CubePack>(std::forward<Args>(args)...);
		pack->SetMaterial("BasePassRenderer.Mesh.ground");

		return CreateMeshEntity(world, "Cube", pack);
	}

	template<typename ...Args>
	inline void WorldFunctions::CreateCubeEntityAsync(World* world, Args ...args)
	{
	}

	template<typename ...Args>
	inline Entity WorldFunctions::CreateCylinderEntity(World* world, Args ...args)
	{
		return Entity();
	}

	template<typename ...Args>
	inline void WorldFunctions::CreateCylinderEntityAsync(World* world, Args ...args)
	{
	}

	template<typename ...Args>
	inline Entity WorldFunctions::CreateDiskEntity(World* world, Args ...args)
	{
		return Entity();
	}

	template<typename ...Args>
	inline void WorldFunctions::CreateDiskEntityAsync(World* world, Args ...args)
	{
	}

	template<typename ...Args>
	inline Entity WorldFunctions::CreatePlaneEntity(World* world, Args ...args)
	{
		SPICES_PROFILE_ZONE;

		std::shared_ptr<PlanePack> pack = std::make_shared<PlanePack>(std::forward<Args>(args)...);
		pack->SetMaterial("BasePassRenderer.Mesh.ground");

		return CreateMeshEntity(world, "Plane", pack);
	}

	template<typename ...Args>
	inline void WorldFunctions::CreatePlaneEntityAsync(World* world, Args ...args)
	{
	}

	template<typename ...Args>
	inline Entity WorldFunctions::CreateSphereEntity(World* world, Args ...args)
	{
		SPICES_PROFILE_ZONE;

		std::shared_ptr<SpherePack> pack = std::make_shared<SpherePack>(std::forward<Args>(args)...);
		pack->SetMaterial("BasePassRenderer.Mesh.ground");

		return CreateMeshEntity(world, "Sphere", pack);
	}

	template<typename ...Args>
	inline void WorldFunctions::CreateSphereEntityAsync(World* world, Args ...args)
	{
	}

	template<typename ...Args>
	inline Entity WorldFunctions::CreateTorusEntity(World* world, Args ...args)
	{
		return Entity();
	}

	template<typename ...Args>
	inline void WorldFunctions::CreateTorusEntityAsync(World* world, Args ...args)
	{
	}

}