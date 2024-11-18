#pragma once
#include "Core/Core.h"
#include "../../GltfCollection.h"
#include "Resources/Material/Material.h"

namespace Spices {

	class GltfPack;

	class GltfLoader
	{
	public:

		static bool Load(const std::string& fileName, GltfCollection* collection);

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