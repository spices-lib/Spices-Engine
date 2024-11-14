#pragma once
#include "Core/Core.h"
#include "GltfObject.h"
#include "Resources/Texture/Texture2D.h"
#include "Resources/ResourcePool/ResourcePool.h"

namespace Spices {

	class GltfImages : public GltfObject
	{
	public:
		struct Item
		{
			std::string uri;
			std::shared_ptr<Texture> texture;
		};

	public:
		GltfImages(const Json& data) 
			: GltfObject(GltfObjectType::images, data) 
		{
			m_ImagesData.resize(data.size());

			for (int i = 0; i < data.size(); i++)
			{
				Item& item = m_ImagesData[i];
				const Json& json = data[i];

				item.uri = json["uri"];

				item.texture = ResourcePool<Texture>::Load<Texture2D>(item.uri, item.uri);
			}
		}

		virtual ~GltfImages() override = default;

	private:
		std::vector<Item> m_ImagesData;
	};
}
