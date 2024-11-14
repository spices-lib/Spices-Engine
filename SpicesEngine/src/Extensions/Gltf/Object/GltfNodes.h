#pragma once
#include "Core/Core.h"
#include "GltfObject.h"

namespace Spices {

	class GltfNodes : public GltfObject
	{
	public:
		struct Item
		{
			std::string name;
			std::vector<uint32_t> children;
			glm::mat4 matrix;
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



				item.matrix[0][0] = node["matrix"][0];
				item.matrix[0][1] = node["matrix"][1];
				item.matrix[0][2] = node["matrix"][2];
				item.matrix[0][3] = node["matrix"][3];
				item.matrix[1][0] = node["matrix"][4];
				item.matrix[1][1] = node["matrix"][5];
				item.matrix[1][2] = node["matrix"][6];
				item.matrix[1][3] = node["matrix"][7];
				item.matrix[2][0] = node["matrix"][8];
				item.matrix[2][1] = node["matrix"][9];
				item.matrix[2][2] = node["matrix"][10];
				item.matrix[2][3] = node["matrix"][11];
				item.matrix[3][0] = node["matrix"][12];
				item.matrix[3][1] = node["matrix"][13];
				item.matrix[3][2] = node["matrix"][14];
				item.matrix[3][3] = node["matrix"][15];
			}
		}
		virtual ~GltfNodes() override = default;

	private:

		std::vector<Item> m_NodesData;
	};
}
