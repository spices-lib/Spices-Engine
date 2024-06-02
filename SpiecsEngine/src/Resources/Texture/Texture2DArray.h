/**
* @file Texture2DArray.h.
* @brief The Texture2DArray Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Texture2D.h"

namespace Spiecs {

	/**
	* @brief Texture2D Class.
	* This class defines the basic behaver of Texture2DArray.
	* @todo finish it.
	*/
	class Texture2DArray : public Texture2D
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		Texture2DArray() {};

		/**
		* @brief Constructor Function.
		* Init class variable, load date immediatelly.
		* Usually call it.
		* @param[in] path Image Path in disk.
		* @noto Do not exectue Texture::Texture(const std::string& path).
		*/
		Texture2DArray(const std::string& path);

		/**
		* @brief Destructor Function.
		*/
		virtual ~Texture2DArray() {};

	private:

		/**
		* Allow TextureLoader access all data.
		*/
		friend class TextureLoader;
	};
}