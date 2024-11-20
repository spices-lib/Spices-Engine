/**
* @file GltfMaterials.h.
* @brief The GltfMaterials Class Definitions.
* @author Spices.
*/

#pragma once
#include <optional>

#include "Core/Core.h"
#include "GltfObject.h"

namespace Spices {

	/**
	* @brief Wrapper of Gltf Json Materials.
	*/
	class GltfMaterials : public GltfObject
	{
	public:

		/**
		* @brief Materials Item data.
		*/
		struct Item
		{
			std::optional<int> baseColorTexture;
			std::optional<int> metallicRoughnessTexture;
			std::optional<int> normalTexture;
			std::optional<int> emissiveTexture;
			std::optional<int> occlusionTexture;
			
			glm::vec4  baseColorFactor;
			glm::vec4  emissiveFactor;
			
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
				Json::object_t json = data[i];

				if(json.find("pbrMetallicRoughness") != json.end())
				{
					Json::object_t factor = json["pbrMetallicRoughness"];
					if(factor.find("baseColorTexture") != factor.end())
					{
						item.baseColorTexture = factor["baseColorTexture"]["index"];
					}

					item.baseColorFactor  = GltfHelper::GetElementVector(factor, "baseColorFactor", glm::vec4(1.0f));

					if(factor.find("metallicRoughnessTexture") != factor.end())
					{
						item.metallicRoughnessTexture = factor["metallicRoughnessTexture"]["index"];
					}
				}

				if(json.find("normalTexture") != json.end())
				{
					item.normalTexture = json["normalTexture"]["index"];
				}

				if(json.find("emissiveTexture") != json.end())
				{
					item.emissiveTexture = json["emissiveTexture"]["index"];
				}

				if(json.find("occlusionTexture") != json.end())
				{
					item.occlusionTexture = json["occlusionTexture"]["index"];
				}

				item.emissiveFactor = GltfHelper::GetElementVector(json, "emissiveFactor", glm::vec4(0.0f));
				
				item.name = GltfHelper::GetElementString(json, "name", "");
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
