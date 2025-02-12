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
		* @param[in] world Specific World.
		* @param[in] name .
		* @param[in] onMeshCreated function executed to create mesh.
		* @param[in] onAdded function executed after Entity has been added to world.
		*/
		static void CreateMeshEntity(
			World*                                 world         ,
			const std::string&                     name          ,
			std::function<std::shared_ptr<Mesh>()> onMeshCreated ,
			std::function<void(Entity&)>           onAdded = nullptr
		);

		/****************************************************************************************/

	private:
		
		/**
		* @brief Create Entity with MeshComponent.
		* Lightweight for game thread.
		* @param[in] world Which world entity belongs to.
		* @param[in] name Entity name.
		* @param[in] mesh Entity MeshComponent Mesh.
		* @param[in] onAdded function executed after Entity has been added to world.
		*/
		static void CreateMeshEntity(
			World*                       world ,
			const std::string&           name  ,
			const std::shared_ptr<Mesh>& mesh  ,
			std::function<void(Entity&)> onAdded = nullptr
		);
	};
}