/**
* @file GltfScenes.h.
* @brief The GltfScenes Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "GltfObject.h"

namespace Spices {

	/**
	* @brief Wrapper of Gltf Json Scenes.
	*/
	class GltfScenes : public GltfObject
	{
	public:

		/**
		* @brief Scenes Item data.
		*/
		struct Item
		{
			std::string name;                /* @brief Scenes name. */
			std::vector<uint32_t> nodes;     /* @brief Scene nodes. */
		};

	public:

		/**
		* @brief Constructor Function.
		* @param[in] data Specific Json element.
		*/
		explicit GltfScenes(const Json data)
			: GltfObject(data) 
		{
			SPICES_PROFILE_ZONE;

			m_ScenesData.resize(data.size());

			for (int i = 0; i < data.size(); i++)
			{
				Item& item       = m_ScenesData[i];
				const Json& json = data[i];

				item.name        = GltfHelper::GetElementString(json, "name", "");

				if (json.find("nodes") != json.end())
				{
					for (int n = 0; n < json["nodes"].size(); n++)
					{
						item.nodes.push_back(json["nodes"][n]);
					}
				}
			}
		}

		/**
		* @brief Destructor Function.
		*/
		virtual ~GltfScenes() override = default;

	private:

		/**
		* @brief Data of Gltf Json Scenes.
		*/
		std::vector<Item> m_ScenesData;

		/**
		* @brief Allow GltfLoader and GltfCollection access all data this class.
		*/
		friend class GltfLoader;
		friend class GltfCollection;
	};
}
