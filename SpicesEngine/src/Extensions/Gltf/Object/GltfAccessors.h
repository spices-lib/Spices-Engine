#pragma once
#include "Core/Core.h"
#include "GltfObject.h"

namespace Spices {

	class GltfAccessors : public GltfObject
	{
	public:

		struct Item
		{
			uint32_t bufferView;
			uint32_t byteOffset;
			uint32_t componentType;
			uint32_t count;
			glm::vec3 max;
			glm::vec3 min;
			std::string type;
		};

	public:

		GltfAccessors(const Json& data) 
			: GltfObject(GltfObjectType::accessors, data) 
		{
			m_AccessorsData.resize(data.size());

			for (int i = 0; i < data.size(); i++)
			{
				Item& item = m_AccessorsData[i];
				const Json& json = data[i];

				item.bufferView = json["bufferView"];
				item.byteOffset = json["byteOffset"];
				item.componentType = json["componentType"];
				item.count = json["count"];
				item.max.x = json["max"][0];
				item.max.y = json["max"][1];
				item.max.z = json["max"][2];
				item.min.x = json["min"][0];
				item.min.y = json["min"][1];
				item.min.z = json["min"][2];
				item.type = json["type"];
			}
		}

		virtual ~GltfAccessors() override = default;

	private:
		std::vector<Item> m_AccessorsData;
	};
}
