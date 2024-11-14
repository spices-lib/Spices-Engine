#pragma once
#include "Core/Core.h"
#include "GltfObject.h"

namespace Spices {

	class GltfBufferViews : public GltfObject
	{
	public:

		struct Item
		{
			uint32_t buffer;
			uint32_t byteLength;
			uint32_t byteOffset;
			uint32_t byteStride;
			uint32_t target;
		};

	public:
		GltfBufferViews(const Json& data)
			: GltfObject(GltfObjectType::bufferViews, data)
		{
			m_BufferViewsData.resize(data.size());

			for (int i = 0; i < data.size(); i++)
			{
				Item& item = m_BufferViewsData[i];
				const Json& json = data[i];

				item.buffer     = json["buffer"];
				item.byteLength = json["byteLength"];
				item.byteOffset = json["byteOffset"];
				item.byteStride = json["byteStride"];
				item.target     = json["target"];
			}
		}

		virtual ~GltfBufferViews() override = default;

	private:
		std::vector<Item> m_BufferViewsData;
	};
}
