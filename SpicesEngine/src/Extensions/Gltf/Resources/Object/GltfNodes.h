/**
* @file GltfNodes.h.
* @brief The GltfNodes Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "GltfObject.h"

#include <glm/gtx/quaternion.hpp>

namespace Spices {

	/**
	* @brief Wrapper of Gltf Json Nodes.
	*/
	class GltfNodes : public GltfObject
	{
	public:

		/**
		* @brief Nodes Item data.
		*/
		struct Item
		{
			std::string name;                      /* @brief Node name.             */
			std::vector<uint32_t> children;        /* @brief Child node index.      */
			int mesh;                              /* @brief Meshes index.          */
			int skin;                              /* @brief Skins index.           */

			glm::mat4 matrix;                      /* @brief Model matrix.          */
			glm::vec4 translation;                 /* @brief Translation component. */
			glm::vec4 scale;                       /* @brief Scale component.       */
			glm::mat4 rotation;                    /* @brief Rotation component.    */
		};

	public:

		/**
		* @brief Constructor Function.
		* @param[in] data Specific Json element.
		*/
		explicit GltfNodes(const Json& data)
			: GltfObject(data) 
		{
			SPICES_PROFILE_ZONE;

			m_NodesData.resize(data.size());

			for (int i = 0; i < data.size(); i++)
			{
				Item& item = m_NodesData[i];
				Json::object_t node = data[i];

				item.name = GltfHelper::GetElementString(node, "name", "");

				if (node.find("children") != node.end())
				{
					for (int c = 0; c < node["children"].size(); c++)
					{
						uint32_t nodeId = node["children"][c];
						item.children.push_back(nodeId);
					}
				}

				item.mesh = GltfHelper::GetElementInt(node, "mesh", -1);
				item.skin = GltfHelper::GetElementInt(node, "skin", -1);

				if (node.find("matrix") != node.end())
				{
					item.matrix      = GltfHelper::GetMatrix(node["matrix"].get<Json::array_t>());
				}
				else
				{
					item.translation = GltfHelper::GetElementVector(node, "translation", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
					item.scale       = GltfHelper::GetElementVector(node, "scale", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
					glm::vec4 rotate = GltfHelper::GetElementVector(node, "rotation", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
					item.rotation    = glm::toMat4(glm::quat({ rotate.w, rotate.x, rotate.y, rotate.z }));

					item.matrix      = glm::translate(glm::mat4(1.0f), glm::vec3(item.translation)) * item.rotation * glm::scale(glm::mat4(1.0f), glm::vec3(item.scale));
				}
			}
		}

		/**
		* @brief Destructor Function.
		*/
		virtual ~GltfNodes() override = default;

	private:

		/**
		* @brief Data of Gltf Json Nodes.
		*/
		std::vector<Item> m_NodesData;

		/**
		* @brief Allow GltfLoader and GltfCollection access all data this class.
		*/
		friend class GltfLoader;
		friend class GltfCollection;
	};
}
