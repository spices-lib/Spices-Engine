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
			int format;
			int internalFormat;
			int sampler;
			int source;
			int target;
			int type;
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

				item.format         = GltfHelper::GetElementInt(json, "format", -1);
				item.internalFormat = GltfHelper::GetElementInt(json, "internalFormat", -1);
				item.sampler        = GltfHelper::GetElementInt(json, "sampler", -1);
				item.source         = GltfHelper::GetElementInt(json, "source", -1);
				item.target         = GltfHelper::GetElementInt(json, "target", -1);
				item.type           = GltfHelper::GetElementInt(json, "type", -1);
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
