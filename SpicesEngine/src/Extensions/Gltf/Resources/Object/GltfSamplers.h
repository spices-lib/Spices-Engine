#pragma once
#include "Core/Core.h"
#include "GltfObject.h"

namespace Spices {

	class GltfSamplers : public GltfObject
	{
	public:
		struct Item
		{
			uint32_t magFilter;
			uint32_t minFilter;
			uint32_t wrapS;
			uint32_t wrapT;
		};

	public:

		GltfSamplers(const Json& data) 
			: GltfObject(data)
		{
			m_SamplersData.resize(data.size());

			for (int i = 0; i < data.size(); i++)
			{
				Item& item = m_SamplersData[i];
				const Json& json = data[i];

				item.magFilter = json["magFilter"];
				item.minFilter = json["minFilter"];
				item.wrapS = json["wrapS"];
				item.wrapT = json["wrapT"];
			}
		}

		virtual ~GltfSamplers() override = default;

	private:
		std::vector<Item> m_SamplersData;
	};
}
