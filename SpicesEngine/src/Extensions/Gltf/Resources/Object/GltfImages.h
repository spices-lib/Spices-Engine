#pragma once
#include "Core/Core.h"
#include "GltfObject.h"
#include "Resources/Texture/Texture2D.h"
#include "Resources/ResourcePool/ResourcePool.h"
#include "Core/Library/FileLibrary.h"
#include "Render/Renderer/DescriptorSetManager/BindLessTextureManager.h"

namespace Spices {

	class GltfImages : public GltfObject
	{
	public:
		struct Item
		{
			std::string uri;
			std::shared_ptr<Texture> texture;
			uint32_t index;
		};

	public:
		GltfImages(const Json& data, const std::filesystem::path& path)
			: GltfObject(data) 
		{
			m_ImagesData.resize(data.size());

			for (int i = 0; i < data.size(); i++)
			{
				Item& item = m_ImagesData[i];
				const Json& json = data[i];

				item.uri = json["uri"];

				std::stringstream file;
				file << path.string() << "/" << item.uri;

				if (!FileLibrary::FileLibrary_Exists(file.str().c_str()))
				{
					std::stringstream ss;
					ss << "Image Files: " << file.str() << "  is not finded.";

					SPICES_CORE_ERROR(ss.str())
					continue;
				}

				item.texture = ResourcePool<Texture>::Load<Texture2D>(file.str(), file.str());
				item.index   = BindLessTextureManager::Registry(file.str());
			}  
		}

		virtual ~GltfImages() override = default;

	private:
		std::filesystem::path m_Path;
		std::vector<Item> m_ImagesData;
	};
}
