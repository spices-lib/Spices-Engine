/**
* @file GltfLights.h.
* @brief The GltfLights Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "GltfObject.h"
#include "../../../assets/Shaders/src/Header/ShaderCommon.h"

namespace Spices {

	/**
	* @brief Wrapper of Gltf Json Lights.
	*/
	class GltfLights : public GltfObject
	{
	public:

		/**
		* @brief Lights Item data.
		*/
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

		/**
		* @brief Constructor Function.
		* @param[in] data Specific Json element.
		*/
		explicit GltfLights(const Json& data)
			: GltfObject(data)
		{
			SPICES_PROFILE_ZONE;

			m_LightsData.resize(data.size());

			for (int i = 0; i < data.size(); i++)
			{
				Item& item = m_LightsData[i];
				Json::object_t json = data[i];

				item.color          = GltfHelper::GetElementVector(json, "color", glm::vec4(1, 1, 1, 0));
				item.range          = GltfHelper::GetElementFloat(json, "range", 105);
				item.intensity      = GltfHelper::GetElementFloat(json, "intensity", 1);
				item.innerConeAngle = GltfHelper::GetElementFloat(json, "spot/innerConeAngle", 0);
				item.outerConeAngle = GltfHelper::GetElementFloat(json, "spot/outerConeAngle", SpicesShader::PI);

				item.name           = GltfHelper::GetElementString(json, "name", "");
				item.type           = GltfHelper::GetElementString(json, "type", "");

				// @todo: another data.
			}
		}

		/**
		* @brief Destructor Function.
		*/
		virtual ~GltfLights() override = default;

	private:

		/**
		* @brief Data of Gltf Json Lights.
		*/
		std::vector<Item> m_LightsData;

		/**
		* @brief Allow GltfLoader and GltfCollection access all data this class.
		*/
		friend class GltfLoader;
		friend class GltfCollection;
	};
}
