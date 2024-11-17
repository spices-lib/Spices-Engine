/**
* @file GltfAnimations.h.
* @brief The GltfAnimations Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "GltfObject.h"

namespace Spices {

	/**
	* @brief Wapper of Gltf Json Animations.
	*/
	class GltfAnimations : public GltfObject
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] data Specific Json element.
		*/
		explicit GltfAnimations(const Json& data) : GltfObject(data) {}

		/**
		* @brief Destructor Function.
		*/
		virtual ~GltfAnimations() override = default;

	private:

		/**
		* @brief Allow GltfLoader and GltfCollection access all data this class.
		*/
		friend class GltfLoader;
		friend class GltfCollection;
	};
}
