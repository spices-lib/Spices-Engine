/**
* @file GltfLoader.h
* @brief The GltfLoader Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "../../GltfCollection.h"
#include "Resources/Material/Material.h"

namespace Spices {

	/**
	* @brief Forward Declare.
	*/
	class GltfPack;

	/**
	* @brief Loader of load gltf file or items.
	*/
	class GltfLoader
	{
	public:

		/**
		* @brief Load a gltf file to GltfCollection.
		* @param[in] fileName gltf file name.
		* @param[in] collection GltfCollection.
		* @reutrn Returns true if load succeed.
		*/
		static bool Load(const std::string& fileName, GltfCollection* collection);

		/**
		* @brief Load a gltf mesh to GltfPack.
		* @param[in] pack GltfPack.
		* @param[in] primitive Gltf Primitive.
		* @param[in] accessors Gltf Accessors.
		* @param[in] buffers Gltf Buffers.
		* @param[in] bufferViews Gltf BufferViews.
		* @reutrn Returns true if load succeed.
		*/
		static bool LoadPack(GltfPack* pack, const GltfMeshes::Primitive& primitive, GltfAccessors* accessors, GltfBuffers* buffers, GltfBufferViews* bufferViews);

		/**
		* @brief Load a Material from Gltf file material component.
		* @param[in] material Gltf file material component item.
		* @param[in] images Gltf Images.
		* @return Returns Material.
		* @attention This material not in ResourcesPool.
		*/
		static std::shared_ptr<Material> LoadMaterial(const GltfMaterials::Item& material, GltfImages* images);
	};
}