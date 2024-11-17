#pragma once
#include "Core/Core.h"
#include "GltfObject.h"

namespace Spices {

	class GltfMeshes : public GltfObject
	{
	public:

		struct Primitive
		{
			uint32_t POSITION;
			uint32_t NORMAL;
			uint32_t TEXCOORD_0;
			uint32_t TANGENT;

			uint32_t indices;
			uint32_t material;
			uint32_t mode;
		};

		struct Item
		{
			std::string name;
			std::vector<Primitive> primitives;
		};

	public:
		GltfMeshes(const Json& data);
		virtual ~GltfMeshes() override = default;

	private:

		std::vector<Item> m_MeshesData;
		friend class GltfLoader;
		friend class GltfCollection;
	};
}
