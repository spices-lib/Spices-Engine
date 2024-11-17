#include "Pchheader.h"
#include "GltfMeshes.h"

namespace Spices {

	GltfMeshes::GltfMeshes(const Json& data)
		: GltfObject(data)
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
				Primitive& prim = item.primitives[j];

				prim.POSITION   = primitives[j]["attributes"]["POSITION"];
				prim.NORMAL     = primitives[j]["attributes"]["NORMAL"];
				prim.TEXCOORD_0 = primitives[j]["attributes"]["TEXCOORD_0"];
				prim.TANGENT    = primitives[j]["attributes"]["TANGENT"];

				prim.indices   = primitives[j]["indices"];
				prim.material  = primitives[j]["material"];
				prim.mode      = primitives[j]["mode"];
			}
		}
	}

}