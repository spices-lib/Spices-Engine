#pragma once
#include "Core/Core.h"
#include "GltfObject.h"
#include "../../../assets/Shaders/src/Header/ShaderCommon.h"

namespace Spices {

	class GltfLights : public GltfObject
	{
	public:
		struct Item
		{
			glm::vec4 color;
			float range;
			float intensity;
			float innerConeAngle;
			float outerConeAngle;

			std::string name;
			std::string type;
		};

	public:
		GltfLights(const Json& data)
			: GltfObject(GltfObjectType::images, data)
		{
			m_LightsData.resize(data.size());

			for (int i = 0; i < data.size(); i++)
			{
				Item& item = m_LightsData[i];
				Json::object_t json = data[i];

				item.color = GltfHelper::GetElementVector(json, "color", glm::vec4(1, 1, 1, 0));
				item.range = GltfHelper::GetElementFloat(json, "range", 105);
				item.intensity = GltfHelper::GetElementFloat(json, "intensity", 1);
				item.innerConeAngle = GltfHelper::GetElementFloat(json, "spot/innerConeAngle", 0);
				item.outerConeAngle = GltfHelper::GetElementFloat(json, "spot/outerConeAngle", PI);

				item.name = GltfHelper::GetElementString(json, "name", "");
				item.type = GltfHelper::GetElementString(json, "type", "");

				// @todo: another data.
			}
		}

		virtual ~GltfLights() override = default;

	private:
		std::vector<Item> m_LightsData;
	};
}
