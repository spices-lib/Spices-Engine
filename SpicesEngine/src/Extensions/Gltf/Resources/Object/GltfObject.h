/**
* @file GltfObject.h.
* @brief The GltfObject Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "../../GltfHelper.h"

#include <nlohmann/json.hpp>

namespace Spices {

	/**
	* @brief Wapper of Gltf Json element
	*/
	class GltfObject
	{
	public:

		/**
		* @brief using Json instand of nlohmann::json
		*/
		using Json = nlohmann::json;

	public:

		/**
		* @brief Constructor Function.
		* @param[in] data Specific Json element.
		*/
		explicit GltfObject(const Json& data) : m_Data(data) {}

		/**
		* @brief Destructor Function.
		*/
		virtual ~GltfObject() = default;

	protected:

		/**
		* @brief Specific Json element.
		*/
		Json m_Data;
	};

}