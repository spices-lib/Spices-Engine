/**
* @file GltfCameras.h.
* @brief The GltfCameras Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "GltfObject.h"

namespace Spices {

	/**
	* @brief Wapper of Gltf Json Cameras.
	*/
	class GltfCameras : public GltfObject
	{
	public:

		/**
		* @brief Cameras Item data.
		*/
		struct Item
		{
			float yfov;           /* @brief camera fov.        */
			float znear;          /* @brief camera near plane. */
			float zfar;           /* @brief camera far plane.  */
			int nodeIndex;        /* @brief node index.        */
		};

	public:

		/**
		* @brief Constructor Function.
		* @param[in] data Specific Json element.
		*/
		explicit GltfCameras(const Json& data)
			: GltfObject(data)
		{
			SPICES_PROFILE_ZONE;

			m_CamerasData.resize(data.size());

			for (int i = 0; i < data.size(); i++)
			{
				Item& item = m_CamerasData[i];
				const Json& json = data[i];

				item.yfov      = GltfHelper::GetElementFloat(json, "perspective/yfov", 0.1f);
				item.znear     = GltfHelper::GetElementFloat(json, "perspective/znear", 0.1f);
				item.zfar      = GltfHelper::GetElementFloat(json, "perspective/zfar", 100.0f);
				item.nodeIndex = -1;
			}
		}

		/**
		* @brief Destructor Function.
		*/
		virtual ~GltfCameras() override = default;

	private:

		/**
		* @brief Data of Gltf Json Cameras
		*/
		std::vector<Item> m_CamerasData;

		/**
		* @brief Allow GltfLoader and GltfCollection access all data this class.
		*/
		friend class GltfLoader;
		friend class GltfCollection;
	};
}
