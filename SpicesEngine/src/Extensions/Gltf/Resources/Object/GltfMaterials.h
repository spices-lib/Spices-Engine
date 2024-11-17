/**
* @file GltfMaterials.h.
* @brief The GltfMaterials Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "GltfObject.h"

namespace Spices {

	/**
	* @brief Wapper of Gltf Json Materials.
	*/
	class GltfMaterials : public GltfObject
	{
	public:

		/**
		* @brief Materials Item data.
		*/
		struct Item
		{

		};

	public:

		/**
		* @brief Constructor Function.
		* @param[in] data Specific Json element.
		*/
		explicit GltfMaterials(const Json& data)
			: GltfObject(data)
		{
			
		}
		 
		/**
		* @brief Destructor Function.
		*/
		virtual ~GltfMaterials() override = default;

	private:

		/**
		* @brief Data of Gltf Json Materials.
		*/
		std::vector<Item> m_MaterialsData;

		/**
		* @brief Allow GltfLoader and GltfCollection access all data this class.
		*/
		friend class GltfLoader;
		friend class GltfCollection;
	};
}
