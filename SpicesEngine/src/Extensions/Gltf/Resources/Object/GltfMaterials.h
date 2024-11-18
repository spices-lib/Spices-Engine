/**
* @file GltfMaterials.h.
* @brief The GltfMaterials Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "GltfObject.h"

namespace Spices {

	/**
	* @brief Wapper of Gltf Json Materials.
	*/
	class GltfMaterials : public GltfObject
	{
	public:

		/**
		* @brief Materials Item data.
		*/
		struct Item
		{
			uint32_t   baseColorTexture;
			glm::vec4  baseColorFactor;
			uint32_t   metallicRoughnessTexture;
			uint32_t   normalTexture;

			std::string name;
		};

	public:

		/**
		* @brief Constructor Function.
		* @param[in] data Specific Json element.
		*/
		explicit GltfMaterials(const Json& data)
			: GltfObject(data)
		{
			SPICES_PROFILE_ZONE;

			m_MaterialsData.resize(data.size());

			for (int i = 0; i < data.size(); i++)
			{
				Item& item       = m_MaterialsData[i];
				const Json& json = data[i];

				item.baseColorTexture = json["pbrMetallicRoughness"]["baseColorTexture"]["index"];
				Json::object_t factor = json["pbrMetallicRoughness"];
				item.baseColorFactor  = GltfHelper::GetElementVector(factor, "baseColorFactor", glm::vec4(1.0f));

				item.metallicRoughnessTexture = json["pbrMetallicRoughness"]["metallicRoughnessTexture"]["index"];
				item.normalTexture = json["normalTexture"]["index"];

				item.name = json["name"];
			}
		}
		
		/**
		* @brief Destructor Function.
		*/
		virtual ~GltfMaterials() override = default;

	private:

		/**
		* @brief Data of Gltf Json Materials.
		*/
		std::vector<Item> m_MaterialsData;

		/**
		* @brief Allow GltfLoader and GltfCollection access all data this class.
		*/
		friend class GltfLoader;
		friend class GltfCollection;
	};
}
