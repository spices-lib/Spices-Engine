#pragma once
#include "Core/Core.h"
#include "GltfObject.h"

namespace Spices {

	class GltfSkins : public GltfObject
	{
	public:

		struct Item
		{

		};

		GltfSkins(const Json& data)
			: GltfObject(GltfObjectType::asset, data)
		{
			m_SkinsData.resize(data.size());

			for (int i = 0; i < data.size(); i++)
			{
				Item& item = m_SkinsData[i];
				const Json& json = data[i];


			}
		}

		virtual ~GltfSkins() override = default;

	private:

		std::vector<Item> m_SkinsData;
	};
}
