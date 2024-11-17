/**
* @file GltfSamplers.h.
* @brief The GltfSamplers Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "GltfObject.h"

namespace Spices {

	/**
	* @brief Wapper of Gltf Json Samplers.
	*/
	class GltfSamplers : public GltfObject
	{
	public:

		/**
		* @brief Samplers Item data.
		*/
		struct Item
		{
			int magFilter;      /* @brief */
			int minFilter;      /* @brief */
			int wrapS;          /* @brief */
			int wrapT;          /* @brief */
		};

	public:

		/**
		* @brief Constructor Function.
		* @param[in] data Specific Json element.
		*/
		explicit GltfSamplers(const Json& data)
			: GltfObject(data)
		{
			SPICES_PROFILE_ZONE;

			m_SamplersData.resize(data.size());

			for (int i = 0; i < data.size(); i++)
			{
				Item& item       = m_SamplersData[i];
				const Json& json = data[i];

				item.magFilter   = GltfHelper::GetElementInt(json, "magFilter", -1);
				item.minFilter   = GltfHelper::GetElementInt(json, "minFilter", -1);
				item.wrapS       = GltfHelper::GetElementInt(json, "wrapS", -1);
				item.wrapT       = GltfHelper::GetElementInt(json, "wrapT", -1);
			}
		}

		/**
		* @brief Destructor Function.
		*/
		virtual ~GltfSamplers() override = default;

	private:

		/**
		* @brief Data of Gltf Json Samplers.
		*/
		std::vector<Item> m_SamplersData;

		/**
		* @brief Allow GltfLoader and GltfCollection access all data this class.
		*/
		friend class GltfLoader;
		friend class GltfCollection;
	};
}
