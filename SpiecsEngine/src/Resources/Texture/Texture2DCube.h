/**
* @file Texture2DCube.h.
* @brief The Texture2DCube Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Texture2D.h"

namespace Spiecs {

	/**
	* @brief Texture2DCube Class.
	* This class defines the basic behaver of Texture2DCube.
	*/
	class Texture2DCube : public Texture2D
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		Texture2DCube() {};

		/**
		* @brief Constructor Function.
		* Init class variable, load date immediatelly.
		* Usually call it.
		* @param[in] path Image Path in disk.
		* @noto Do not exectue Texture::Texture(const std::string& path).
		*/
		Texture2DCube(const std::string& path);

		/**
		* @brief Destructor Function.
		*/
		virtual ~Texture2DCube() {};

	private:

		/**
		* Allow TextureLoader access all data.
		*/
		friend class TextureLoader;
	};
}