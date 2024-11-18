/**
* @file GltfSkins.h.
* @brief The GltfSkins Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "GltfObject.h"

namespace Spices {

	/**
	* @brief Wrapper of Gltf Json Skins.
	*/
	class GltfSkins : public GltfObject
	{
	public:

		/**
		* @brief Scenes Item data.
		*/
		struct Item
		{

		};

	public:

		/**
		* @brief Constructor Function.
		* @param[in] data Specific Json element.
		*/
		explicit GltfSkins(const Json& data)
			: GltfObject(data)
		{
			SPICES_PROFILE_ZONE;

			m_SkinsData.resize(data.size());

			for (int i = 0; i < data.size(); i++)
			{
				Item& item       = m_SkinsData[i];
				const Json& json = data[i];


			}
		}

		/**
		* @brief Destructor Function.
		*/
		virtual ~GltfSkins() override = default;

	private:

		/**
		* @brief Data of Gltf Json Skins.
		*/
		std::vector<Item> m_SkinsData;

		/**
		* @brief Allow GltfLoader and GltfCollection access all data this class.
		*/
		friend class GltfLoader;
		friend class GltfCollection;
	};
}
