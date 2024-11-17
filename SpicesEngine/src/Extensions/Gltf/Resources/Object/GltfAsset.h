/**
* @file GltfAsset.h.
* @brief The GltfAsset Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "GltfObject.h"

namespace Spices {

	/**
	* @brief Wapper of Gltf Json Asset.
	*/
	class GltfAsset : public GltfObject
	{
	public:

		/**
		* @brief Asset Item data.
		*/
		struct Item
		{
			std::string generator;     // gltf file creator.
			std::string extras;        // gltf extensions.
			std::string version;       // gltf version.
		};

	public:

		/**
		* @brief Constructor Function.
		* @param[in] data Specific Json element.
		*/
		explicit GltfAsset(const Json& data)
			: GltfObject(data) 
		{
			SPICES_PROFILE_ZONE;

			m_AssetData.generator = GltfHelper::GetElementString(data, "generator", "");
			//m_AssetData.extras = data["extras"];
			m_AssetData.version   = GltfHelper::GetElementString(data, "version", "");
		}

		/**
		* @brief Destructor Function.
		*/
		virtual ~GltfAsset() override = default;

	private:

		/**
		* @brief Data of Gltf Json Asset.
		*/
		Item m_AssetData;

		/**
		* @brief Allow GltfLoader and GltfCollection access all data this class.
		*/
		friend class GltfLoader;
		friend class GltfCollection;
	};
}
