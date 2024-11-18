/**
* @file GltfAccessors.h.
* @brief The GltfAccessors Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "GltfObject.h"

namespace Spices {

	/**
	* @brief Wrapper of Gltf Json Accessors.
	*/
	class GltfAccessors : public GltfObject
	{
	public:

		/**
		* @brief Accessors Item data.
		*/
		struct Item
		{
			int         bufferView;         /* @brief Buffer View Index.                  */
			int         byteOffset;         /* @brief Byte offset in indexed buffer view. */
			int         componentType;      /* @brief Buffer data bytes type.             */
			int         count;              /* @brief Buffer data count.                  */
			glm::vec4   max;                /* @brief Buffer data max vale.               */
			glm::vec4   min;                /* @brief Buffer data min vale.               */
			std::string type;               /* @brief Buffer data channel type.           */
		};

	public:

		/**
		* @brief Constructor Function.
		* @param[in] data Specific Json element.
		*/
		explicit GltfAccessors(const Json& data)
			: GltfObject(data) 
		{
			SPICES_PROFILE_ZONE;

			m_AccessorsData.resize(data.size());

			for (int i = 0; i < data.size(); i++)
			{
				Item& item = m_AccessorsData[i];
				const Json& json = data[i];

				item.bufferView    = GltfHelper::GetElementInt(json, "bufferView", -1);
				item.byteOffset    = GltfHelper::GetElementInt(json, "byteOffset", -1);
				item.componentType = GltfHelper::GetElementInt(json, "componentType", -1);
				item.count         = GltfHelper::GetElementInt(json, "count", -1);
				item.max           = GltfHelper::GetVector(GltfHelper::GetElementJsonArray(json, "max", { 0.0, 0.0, 0.0, 0.0 }));
				item.min           = GltfHelper::GetVector(GltfHelper::GetElementJsonArray(json, "min", { 0.0, 0.0, 0.0, 0.0 }));
				item.type          = GltfHelper::GetElementString(json, "type", "");
			}
		}

		/**
		* @brief Destructor Function.
		*/
		virtual ~GltfAccessors() override = default;

	private:

		/**
		* @brief Data of Gltf Json Accessors
		*/
		std::vector<Item> m_AccessorsData;

		/**
		* @brief Allow GltfLoader and GltfCollection access all data this class.
		*/
		friend class GltfLoader;
		friend class GltfCollection;
	};
}
