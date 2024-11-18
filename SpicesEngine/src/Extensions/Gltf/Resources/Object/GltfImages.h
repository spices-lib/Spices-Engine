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
	* @brief Wrapper of Gltf Json Images.
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

				std::string folder = json["uri"];

				std::stringstream file;
				file << path.string() << "/" << folder;

				item.uri = file.str();

				if (!FileLibrary::FileLibrary_Exists(item.uri.c_str()))
				{
					std::stringstream ss;
					ss << "Image Files: " << item.uri << "  is not finded.";

					SPICES_CORE_ERROR(ss.str())
					continue;
				}
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
