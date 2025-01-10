/**
* @file GltfCollection.h
* @brief The GltfCollection Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "World/Components/TransformComponent.h"

#include "Resources/Object/GltfAccessors.h"
#include "Resources/Object/GltfAnimations.h"
#include "Resources/Object/GltfAsset.h"
#include "Resources/Object/GltfBuffers.h"
#include "Resources/Object/GltfBufferViews.h"
#include "Resources/Object/GltfCameras.h"
#include "Resources/Object/GltfImages.h"
#include "Resources/Object/GltfLights.h"
#include "Resources/Object/GltfMaterials.h"
#include "Resources/Object/GltfMeshes.h"
#include "Resources/Object/GltfNodes.h"
#include "Resources/Object/GltfSamplers.h"
#include "Resources/Object/GltfScene.h"
#include "Resources/Object/GltfScenes.h"
#include "Resources/Object/GltfSkins.h"
#include "Resources/Object/GltfTextures.h"

namespace Spices {

	/**
	* @brief Forward Declare.
	*/
	class GltfLoader;
	class World;
	class GltfPack;

	/**
	* @brief Wrapper of Gltf file data.
	*/
	class GltfCollection : public std::enable_shared_from_this<GltfCollection>
	{
	public:

		struct LoadingState
		{
			std::atomic_uint32_t loadedMeshes = 0;
		};

	public:

		/**
		* @brief Constructor Function.
		* @param[in] path gltf file path.
		*/
		explicit GltfCollection(const std::string& path);

		/**
		* @brief Destructor Function.
		*/
		virtual ~GltfCollection() = default;

		/**
		* @brief Create Entity instance from gltf file.
		* @param[in] world World Pointer.
		* @param[in] tag Entity name.
		* @param[in] transform GLTF Instance transform
		*/
		void CreateEntity(World* world, const std::string& tag, Transform transform = {});

	private:

		/**
		* @brief Create Entity recursive from gltf file nodes.
		* @param[in] world World Pointer.
		* @param[in] tag Entity name.
		* @param[in] node gltf nodes index.
		* @param[in] model recursive model matrix.
		* @param[in] loadingState LoadingState.
		*/
		void CreateEntityRecursive(
			World*             world ,
			const std::string& tag   ,
			uint32_t           node  ,
			const glm::mat4&   model ,
			std::shared_ptr<LoadingState> loadingState);

	private:

		std::unique_ptr<GltfAccessors  >   m_Accessors;         /* @brief BufferView Clip.      */
		std::unique_ptr<GltfAnimations >   m_Animations;        /* @brief Animation data.       */
		std::unique_ptr<GltfAsset      >   m_Asset;             /* @brief Gltf file information.*/
		std::unique_ptr<GltfBuffers    >   m_Buffers;           /* @brief Mesh Buffer data.     */
		std::unique_ptr<GltfBufferViews>   m_BufferViews;       /* @brief Buffer Clip.          */
		std::unique_ptr<GltfCameras    >   m_Cameras;           /* @brief Camera data.          */
		std::unique_ptr<GltfImages     >   m_Images;            /* @brief Image data.           */
		std::unique_ptr<GltfLights     >   m_Lights;            /* @brief Light data.           */
		std::unique_ptr<GltfMaterials  >   m_Materials;         /* @brief Material data.        */
		std::unique_ptr<GltfMeshes     >   m_Meshes;            /* @brief Mesh data.            */
		std::unique_ptr<GltfNodes      >   m_Nodes;             /* @brief Node data.            */
		std::unique_ptr<GltfSamplers   >   m_Samplers;          /* @brief Image sampler data.   */
		std::unique_ptr<GltfScene      >   m_Scene;             /* @brief Scene data.           */
		std::unique_ptr<GltfScenes     >   m_Scenes;            /* @brief Scenes data.          */
		std::unique_ptr<GltfSkins      >   m_Skins;             /* @brief Skins data.           */
		std::unique_ptr<GltfTextures   >   m_Textures;          /* @brief Texture data.         */

		/**
		* @brief Allow GltfLoader access all data this class.
		*/
		friend class GltfLoader;
	};
}