#pragma once
#include "Core/Core.h"
#include "GltfObject.h"

#include <glm/gtx/quaternion.hpp>

namespace Spices {

	class GltfNodes : public GltfObject
	{
	public:
		struct Item
		{
			std::string name;
			std::vector<uint32_t> children;
			int meshIndex;
			int skinIndex;

			glm::mat4 matrix;
			glm::vec4 translation;
			glm::vec4 scale;
			glm::mat4 rotation;
		};

	public:

		GltfNodes(const Json& data) 
			: GltfObject(data) 
		{
			m_NodesData.resize(data.size());

			for (int i = 0; i < data.size(); i++)
			{
				Item& item = m_NodesData[i];
				Json::object_t node = data[i];

				item.name = node["name"];

				if (node.find("children") != node.end())
				{
					for (int c = 0; c < node["children"].size(); c++)
					{
						uint32_t nodeId = node["children"][c];
						item.children.push_back(nodeId);
					}
				}

				item.meshIndex = GltfHelper::GetElementInt(node, "mesh", -1);
				item.skinIndex = GltfHelper::GetElementInt(node, "skin", -1);

				if (node.find("matrix") != node.end())
				{
					item.matrix      = GltfHelper::GetMatrix(node["matrix"].get<Json::array_t>());
				}
				else
				{
					item.translation = GltfHelper::GetElementVector(node, "translation", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
					item.scale       = GltfHelper::GetElementVector(node, "scale", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
					glm::vec4 rotate = GltfHelper::GetElementVector(node, "rotation", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
					item.rotation    = glm::toMat4(glm::quat({ rotate.x, rotate.y, rotate.z, rotate.w }));

					item.matrix      = glm::translate(glm::mat4(1.0f), glm::vec3(item.translation)) * item.rotation * glm::scale(glm::mat4(1.0f), glm::vec3(item.scale));
				}
			}
		}

		virtual ~GltfNodes() override = default;

	private:

		std::vector<Item> m_NodesData;
		friend class GltfCollection;
	};
}
