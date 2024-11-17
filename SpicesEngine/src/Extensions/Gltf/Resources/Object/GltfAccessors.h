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
			: GltfObject(data) 
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
				item.max = GltfHelper::GetVector(GltfHelper::GetElementJsonArray(json, "max", { 0.0, 0.0, 0.0, 0.0 }));
				item.min = GltfHelper::GetVector(GltfHelper::GetElementJsonArray(json, "min", { 0.0, 0.0, 0.0, 0.0 }));
				item.type = json["type"];
			}
		}

		virtual ~GltfAccessors() override = default;

	private:
		std::vector<Item> m_AccessorsData;
		friend class GltfLoader;
		friend class GltfCollection;
	};
}
