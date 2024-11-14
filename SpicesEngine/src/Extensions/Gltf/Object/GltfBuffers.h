#pragma once
#include "Core/Core.h"
#include "GltfObject.h"

namespace Spices {

	class GltfBuffers : public GltfObject
	{
	public:
		struct Item
		{
			uint32_t byteLength;
			std::string uri;
			std::unique_ptr<std::vector<char>> buffer;
		};

	public:
		GltfBuffers(const Json& data)
			: GltfObject(GltfObjectType::buffers, data)
		{
			m_BuffersData.resize(data.size());

			for (int i = 0; i < data.size(); i++)
			{
				Item& item = m_BuffersData[i];
				const Json& json = data[i];

				item.byteLength = json["byteLength"];
				item.uri = json["uri"];

				std::ifstream ff(item.uri, std::ios::in | std::ios::binary);

				ff.seekg(0, ff.end);
				std::streamoff length = ff.tellg();
				ff.seekg(0, ff.beg);

				item.buffer = std::make_unique<std::vector<char>>(0, length);
				ff.read(item.buffer->data(), length);
			}
		}

		virtual ~GltfBuffers() override = default;

	private:
		std::vector<Item> m_BuffersData;
	};
}
