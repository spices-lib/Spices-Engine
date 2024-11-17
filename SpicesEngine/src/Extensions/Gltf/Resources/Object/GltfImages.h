/**
* @file GltfImages.h.
* @brief The GltfImages Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "GltfObject.h"
#include "Resources/Texture/Texture2D.h"
#include "Resources/ResourcePool/ResourcePool.h"
#include "Core/Library/FileLibrary.h"
#include "Render/Renderer/DescriptorSetManager/BindLessTextureManager.h"

namespace Spices {

	/**
	* @brief Wapper of Gltf Json Images.
	*/
	class GltfImages : public GltfObject
	{
	public:

		/**
		* @brief Images Item data.
		*/
		struct Item
		{
			std::string uri;                         /* @brief Image file url.                        */
			std::shared_ptr<Texture> texture;        /* @brief Image pointer.                         */
			uint32_t index;                          /* @brief Image index in BindLess DescriptorSet. */
		};

	public:

		/**
		* @brief Constructor Function.
		* @param[in] data Specific Json element.
		* @param[in] path gltf file directory path.
		*/
		GltfImages(const Json& data, const std::filesystem::path& path)
			: GltfObject(data) 
		{
			SPICES_PROFILE_ZONE;

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

		/**
		* @brief Destructor Function.
		*/
		virtual ~GltfImages() override = default;

	private:

		/**
		* @brief gltf file directory path.
		*/
		std::filesystem::path m_Path;

		/**
		* @brief Data of Gltf Json Cameras
		*/
		std::vector<Item> m_ImagesData;

		/**
		* @brief Allow GltfLoader and GltfCollection access all data this class.
		*/
		friend class GltfLoader;
		friend class GltfCollection;
	};
}
