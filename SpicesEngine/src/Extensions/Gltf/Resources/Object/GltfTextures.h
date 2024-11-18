/**
* @file GltfTextures.h.
* @brief The GltfTextures Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "GltfObject.h"

namespace Spices {

	/**
	* @brief Wrapper of Gltf Json Textures.
	*/
	class GltfTextures : public GltfObject
	{
	public:

		/**
		* @brief Textures Item data.
		*/
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

		/**
		* @brief Constructor Function.
		* @param[in] data Specific Json element.
		*/
		explicit GltfTextures(const Json& data)
			: GltfObject(data) 
		{
			SPICES_PROFILE_ZONE;

			m_TexturesData.resize(data.size());

			for (int i = 0; i < data.size(); i++)
			{
				Item& item          = m_TexturesData[i];
				const Json& json    = data[i];

				item.format         = json["format"];
				item.internalFormat = json["internalFormat"];
				item.sampler        = json["sampler"];
				item.source         = json["source"];
				item.target         = json["target"];
				item.type           = json["type"];
			}
		}

		/**
		* @brief Destructor Function.
		*/
		virtual ~GltfTextures() override = default;

	private:

		/**
		* @brief Data of Gltf Json Textures.
		*/
		std::vector<Item> m_TexturesData;

		/**
		* @brief Allow GltfLoader and GltfCollection access all data this class.
		*/
		friend class GltfLoader;
		friend class GltfCollection;
	};
}
