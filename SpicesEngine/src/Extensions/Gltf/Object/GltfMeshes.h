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
		GltfMeshes(const Json& data) 
			: GltfObject(GltfObjectType::meshes, data)
		{
			m_MeshesData.resize(data.size());

			for (int i = 0; i < data.size(); i++)
			{
				Item& item = m_MeshesData[i];
				const Json& json = data[i];

				item.name = json["name"];

				auto& primitives = json["primitives"];
				item.primitives.resize(primitives.size());
				for (int j = 0; j < primitives.size(); j++)
				{
					Primitive& prim = item.primitives[i];

					prim.POSITION   = primitives[j]["attributes"]["POSITION"];
					prim.NORMAL     = primitives[j]["attributes"]["NORMAL"];
					prim.TEXCOORD_0 = primitives[j]["attributes"]["TEXCOORD_0"];
					prim.TANGENT    = primitives[j]["attributes"]["TANGENT"];

					prim.indices    = primitives[j]["indices"];
					prim.material   = primitives[j]["material"];
					prim.mode       = primitives[j]["mode"];
				}
			}
		}

		virtual ~GltfMeshes() override = default;

	private:

		std::vector<Item> m_MeshesData;
	};
}
