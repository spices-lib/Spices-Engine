/**
* @file WorldFunctions.h
* @brief The WorldFunctions Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Resources/Mesh/MeshPack.h"
#include "..\..\Core\Thread\ThreadModel.h"

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
		* @brief Create Entity with a Basic MeshComponent Async.
		* @tparam T Specific MeshPack type.
		* @param[in] world Specific World.
		* @param[in] args MeshPack construct parameters.
		*/
		template<typename T, typename ...Args>
		static void CreateBasicMeshEntity(World* world, Args... args);

		/****************************************************************************************/

	private:
		
		/**
		* @brief Create Entity with MeshComponent.
		* Lightweight for game thread.
		* @param world Which world entity belongs to.
		* @param name Entity name.
		* @param mesh Entity MeshComponent Mesh.
		*/
		static void CreateMeshEntity(World* world, const std::string& name, const std::shared_ptr<Mesh>& mesh);
	};

	template<typename T, typename ...Args>
	inline void WorldFunctions::CreateBasicMeshEntity(World* world, Args ...args)
	{
		AsyncTask(ThreadPoolEnum::Custom, [=]() {

			SPICES_PROFILE_ZONEN("CreateBasicMeshEntity");
			
			const std::shared_ptr<T> pack    = std::make_shared<T>(std::forward<Args>(args)...);
			const std::shared_ptr<Mesh> mesh = Mesh::Builder().AddPack(pack).Build();

			pack->SetMaterial("BasePassRenderer.Mesh.Default");

			CreateMeshEntity(world, "Cube", mesh);
		});
	}
}