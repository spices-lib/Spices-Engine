#pragma once
#include "Core/Core.h"
#include "../../GltfCollection.h"

namespace Spices {

	class GltfPack;

	class GltfLoader
	{
	public:

		static bool Load(const std::string& fileName, GltfCollection* collection);

		static bool LoadPack(GltfPack* pack, const GltfMeshes::Primitive& primitive, GltfAccessors* accessors, GltfBuffers* buffers, GltfBufferViews* bufferViews);
	};
}