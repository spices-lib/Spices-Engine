#pragma once
#include "Core/Core.h"
#include "GltfObject.h"

namespace Spices {

	class GltfTextures : public GltfObject
	{
	public:

		struct Item
		{
			uint32_t format;
			uint32_t internalFormat;
			uint32_t sampler;
			uint32_t source;
			uint32_t target;
			uint32_t type;
		};

	public:

		GltfTextures(const Json& data) 
			: GltfObject(GltfObjectType::textures, data) 
		{
			m_TexturesData.resize(data.size());

			for (int i = 0; i < data.size(); i++)
			{
				Item& item = m_TexturesData[i];
				const Json& json = data[i];

				item.format = json["format"];
				item.internalFormat = json["internalFormat"];
				item.sampler = json["sampler"];
				item.source = json["source"];
				item.target = json["target"];
				item.type = json["type"];
			}
		}

		virtual ~GltfTextures() override = default;

	private:
		std::vector<Item> m_TexturesData;
	};
}
