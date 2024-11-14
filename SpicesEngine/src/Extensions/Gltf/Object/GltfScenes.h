#pragma once
#include "Core/Core.h"
#include "GltfObject.h"

namespace Spices {

	class GltfScenes : public GltfObject
	{
	public:

		struct Item
		{
			std::string name;
			std::vector<uint32_t> nodes;
		};

	public:

		GltfScenes(const Json data) 
			: GltfObject(GltfObjectType::scenes, data) 
		{
			m_ScenesData.resize(data.size());

			for (int i = 0; i < data.size(); i++)
			{
				Item& item = m_ScenesData[i];
				const Json& json = data[i];

				item.name = json["name"];

				for (int n = 0; n < json["nodes"].size(); n++)
				{
					item.nodes.push_back(json["nodes"][n]);
				}
			}
		}

		virtual ~GltfScenes() override = default;

	private:
		std::vector<Item> m_ScenesData;
	};
}
