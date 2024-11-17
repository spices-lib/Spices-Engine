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
		GltfBuffers(const Json& data, const std::filesystem::path& path)
			: GltfObject(data)
			, m_Path(path)
		{
			m_BuffersData.resize(data.size());

			for (int i = 0; i < data.size(); i++)
			{
				Item& item = m_BuffersData[i];
				const Json& json = data[i];

				item.byteLength = json["byteLength"];
				item.uri = json["uri"];

				std::stringstream file;
				file << m_Path.string() << "/" << item.uri;
				
				std::ifstream f(file.str(), std::ios::in | std::ios::binary);
				if (!f)
				{
					std::stringstream ss;
					ss << "Buffer Files: " << file.str() << "  is not finded.";

					SPICES_CORE_ERROR(ss.str())
					continue;
				}

				f.seekg(0, f.end);
				std::streamoff length = f.tellg();
				f.seekg(0, f.beg);

				item.buffer = std::make_unique<std::vector<char>>(length, 0);
				f.read(item.buffer->data(), length);
			}
		}

		virtual ~GltfBuffers() override = default;

	private:
		std::filesystem::path m_Path;
		std::vector<Item> m_BuffersData;
		friend class GltfLoader;
		friend class GltfCollection;
	};
}
