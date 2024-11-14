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
			glm::mat4 matrix;
			int meshIndex;
			int skinIndex;
			glm::vec4 translation;
			glm::vec4 scale;
			glm::mat4 rotation;
		};

	public:

		GltfNodes(const Json& data) 
			: GltfObject(GltfObjectType::nodes, data) 
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

				item.translation = GltfHelper::GetElementVector(node, "translation", glm::vec4(0, 0, 0, 0));
				item.scale       = GltfHelper::GetElementVector(node, "scale", glm::vec4(0, 0, 0, 0));

				if (node.find("rotation") != node.end())
				{
					glm::vec3 rotate = GltfHelper::GetElementVector(node, "rotation", glm::vec4(0, 0, 0, 0));
					item.rotation = glm::toMat4(glm::quat({ glm::radians(rotate.x), glm::radians(rotate.y), glm::radians(rotate.z) }));
				}
				else if(node.find("matrix") != node.end())
				{
					item.rotation = GltfHelper::GetMatrix(node["matrix"].get<Json::array_t>());
				}
				else
				{
					item.rotation = glm::mat4(1.0f);
				}
			}
		}
		virtual ~GltfNodes() override = default;

	private:

		std::vector<Item> m_NodesData;
	};
}
