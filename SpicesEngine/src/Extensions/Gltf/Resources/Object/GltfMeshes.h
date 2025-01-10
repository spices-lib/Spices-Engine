/**
* @file GltfMeshes.h.
* @brief The GltfMeshes Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "GltfObject.h"

namespace Spices {

	/**
	* @brief Wrapper of Gltf Json Meshes.
	*/
	class GltfMeshes : public GltfObject
	{
	public:

		/**
		* @brief Primitive Item data.
		*/
		struct Primitive
		{
			int POSITION;          /* @brief Position accessor index. */
			int NORMAL;            /* @brief Normal accessor index.   */
			int TEXCOORD_0;        /* @brief TexCoord accessor index. */
			int TANGENT;           /* @brief Tangent accessor index.  */

			int indices;           /* @brief Indices accessor index.  */
			int material;          /* @brief Material accessor index. */
			int mode;              /* @todo                           */
		};

		/**
		* @brief Meshes Item data.
		*/
		struct Item
		{
			std::string name;                       /* @brief Mesh name.       */
			std::vector<Primitive> primitives;      /* @brief Mesh Primitives. */
		};

	public:

		/**
		* @brief Constructor Function.
		* @param[in] data Specific Json element.
		*/
		explicit GltfMeshes(const Json& data)
			: GltfObject(data)
		{
			SPICES_PROFILE_ZONE;

			m_MeshesData.resize(data.size());

			for (int i = 0; i < data.size(); i++)
			{
				Item& item = m_MeshesData[i];
				const Json& json = data[i];

				item.name = GltfHelper::GetElementString(json, "name", "");

				if (json.find("primitives") == json.end()) continue;

				auto& primitives = json["primitives"];
				item.primitives.resize(primitives.size());
				for (int j = 0; j < primitives.size(); j++)
				{
					Primitive& prim = item.primitives[j];
					auto& primitive = primitives[j];

					prim.POSITION   = GltfHelper::GetElementInt(primitive["attributes"], "POSITION", -1);
					prim.NORMAL     = GltfHelper::GetElementInt(primitive["attributes"], "NORMAL", -1);
					prim.TEXCOORD_0 = GltfHelper::GetElementInt(primitive["attributes"], "TEXCOORD_0", -1);
					prim.TANGENT    = GltfHelper::GetElementInt(primitive["attributes"], "TANGENT", -1);

					prim.indices   = GltfHelper::GetElementInt(primitive, "indices", -1);
					prim.material  = GltfHelper::GetElementInt(primitive, "material", -1);
					prim.mode      = GltfHelper::GetElementInt(primitive, "mode", -1);
				}
			}

			std::stringstream ss;
			ss << "Meshes Counts: " << data.size();

			SPICES_CORE_INFO(ss.str())
		}

		/**
		* @brief Destructor Function.
		*/
		virtual ~GltfMeshes() override = default;

		/**
		* @brief Get MeshesData Size.
		* @return Returns MeshesData Size.
		*/
		size_t GetNMeshes() { return m_MeshesData.size(); }

	private:

		/**
		* @brief Data of Gltf Json Meshes.
		*/
		std::vector<Item> m_MeshesData;

		/**
		* @brief Allow GltfLoader and GltfCollection access all data this class.
		*/
		friend class GltfLoader;
		friend class GltfCollection;
	};
}
