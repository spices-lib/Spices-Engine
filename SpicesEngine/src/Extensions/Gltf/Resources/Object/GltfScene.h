/**
* @file GltfScene.h.
* @brief The GltfScene Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "GltfObject.h"

namespace Spices {

	/**
	* @brief Wapper of Gltf Json Scene.
	*/
	class GltfScene : public GltfObject
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] data Specific Json element.
		*/
		explicit GltfScene(const Json& data)
			: GltfObject(data) 
		{ 
			SPICES_PROFILE_ZONE;

			m_Scene = data.get<int>();
		}

		/**
		* @brief Destructor Function.
		*/
		virtual ~GltfScene() override = default;

	private:

		/**
		* @brief Data of Gltf Json Scene.
		*/
		uint32_t m_Scene;

		/**
		* @brief Allow GltfLoader and GltfCollection access all data this class.
		*/
		friend class GltfLoader;
		friend class GltfCollection;
	};
}
