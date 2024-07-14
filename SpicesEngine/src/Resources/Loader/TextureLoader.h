/**
* @file TextureLoader.h.
* @brief The TextureLoader Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"

namespace Spices {

	/**
	* @brief Forward declare.
	*/
	class Texture2D;

	/**
	* @brief Forward declare.
	*/
	class Texture2DCube;

	/**
	* @brief TextureLoader Class.
	* This class only defines static function for load data from image file.
	*/
	class TextureLoader
	{
	public:

		/**
		* @brief Load image to a Texture2D object.
		* @param[in] fileName Image path.
		* @param[in out] outTexture Textrue2D pointer, only pass this to it.
		* Feel free for using row pointer here, it's very safe, for this API only called during Material.
		* @return Returns true if load data succssfully.
		*/
		static void Load(const std::string& filePath, Texture2D* outTexture);

		/**
		* @brief Load image to a Texture2DCube object.
		* @param[in] fileName Image path.
		* @param[in out] outTexture Texture2DCube pointer, only pass this to it.
		* Feel free for using row pointer here, it's very safe, for this API only called during Material.
		* @return Returns true if load data succssfully.
		* @todo finish it.
		*/
		static void Load(const std::string& filePath, Texture2DCube* outTexture);
	};
}