/**
* @file GltfBufferViews.h.
* @brief The GltfBufferViews Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "GltfObject.h"

namespace Spices {

	/**
	* @brief Wapper of Gltf Json BufferViews.
	*/
	class GltfBufferViews : public GltfObject
	{
	public:

		/**
		* @brief BufferViews Item data.
		*/
		struct Item
		{
			int buffer;             /* @brief Buffer index. */
			int byteLength;         /* @brief BufferView length. */
			int byteOffset;         /* @brief Bytes offset in this view. */
			int byteStride;         /* @brief Buffer data bytes stride. */
			int target;             /* @todo */
		};

	public:

		/**
		* @brief Constructor Function.
		* @param[in] data Specific Json element.
		*/
		explicit GltfBufferViews(const Json& data)
			: GltfObject(data)
		{
			SPICES_PROFILE_ZONE;

			m_BufferViewsData.resize(data.size());

			for (int i = 0; i < data.size(); i++)
			{
				Item& item = m_BufferViewsData[i];
				const Json& json = data[i];

				item.buffer     = GltfHelper::GetElementInt(json, "buffer", -1);
				item.byteLength = GltfHelper::GetElementInt(json, "byteLength", -1);
				item.byteOffset = GltfHelper::GetElementInt(json, "byteOffset", -1);
				item.byteStride = GltfHelper::GetElementInt(json, "byteStride", -1);
				item.target     = GltfHelper::GetElementInt(json, "target", -1);
			}
		}

		/**
		* @brief Destructor Function.
		*/
		virtual ~GltfBufferViews() override = default;

	private:

		/**
		* @brief Data of Gltf Json BufferViews
		*/
		std::vector<Item> m_BufferViewsData;

		/**
		* @brief Allow GltfLoader and GltfCollection access all data this class.
		*/
		friend class GltfLoader;
		friend class GltfCollection;
	};
}
