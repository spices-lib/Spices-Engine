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
		static void Load(const std::string& fileName, Texture2D* outTexture);

		/**
		* @brief Load image to a Texture2DCube object.
		* @param[in] fileName Image path.
		* @param[in out] outTexture Texture2DCube pointer, only pass this to it.
		* Feel free for using row pointer here, it's very safe, for this API only called during Material.
		* @return Returns true if load data succssfully.
		* @todo finish it.
		*/
		static void Load(const std::string& fileName, Texture2DCube* outTexture);

	private:

		/**
		* @brief Search texture file and load.
		* @param[in] fileName .
		* @param[in] binF Load ktx file function.
		* @param[in] srcF Load src file function.
		* @return Returns true if load file succeed.
		*/
		static bool SearchFile(
			const std::string& fileName, 
			std::function<void(const std::string&)> binF, 
			std::function<void(const std::string&)> srcF
		);

		/**
		* @brief Function of load a ktx file.
		* @param[in] fileName ktx filenme.
		* @param[in] it file directfolder.
		* @param[in] outTexture Pointer of texture.
		* @return Returns true if load file succeed.
		*/
		static bool LoadBin(const std::string& fileName, const std::string& it, Texture2D* outTexture);

		/**
		* @brief Function of load a src file.
		* @param[in] fileName src filenme.
		* @param[in] it file directfolder.
		* @param[in] outTexture Pointer of texture.
		* @return Returns true if load file succeed.
		*/
		static bool LoadSrc(const std::string& fileName, const std::string& it, Texture2D* outTexture);

	};
}