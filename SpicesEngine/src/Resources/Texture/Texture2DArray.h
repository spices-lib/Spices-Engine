/**
* @file Texture2DArray.h.
* @brief The Texture2DArray Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Texture2D.h"

namespace Spices {

	/**
	* @brief Texture2D Class.
	* This class defines the basic behaves of Texture2DArray.
	* @todo finish it.
	*/
	class Texture2DArray : public Texture2D
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		Texture2DArray() {}

		/**
		* @brief Constructor Function.
		* Used for create render resource.
		* @param[in] info RendererResourceCreateInfo.
		*/
		Texture2DArray(const RendererResourceCreateInfo& info);

		/**
		* @brief Constructor Function.
		* Init class variable, load date immediately.
		* Usually call it.
		* @param[in] path Image Path in disk.
		* @noto Do not execute Texture::Texture(const std::string& path).
		*/
		Texture2DArray(const std::string& path);

		/**
		* @brief Destructor Function.
		*/
		virtual ~Texture2DArray() override = default;

	private:

		/**
		* Allow TextureLoader access all data.
		*/
		friend class TextureLoader;
	};
}