/**
* @file Texture2DCube.h.
* @brief The Texture2DCube Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Texture2D.h"

namespace Spices {

	/**
	* @brief Texture2DCube Class.
	* This class defines the basic behaves of Texture2DCube.
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
		* Used for create render resource.
		* @param[in] info RendererResourceCreateInfo.
		*/
		Texture2DCube(const RendererResourceCreateInfo& info);

		/**
		* @brief Constructor Function.
		* Init class variable, load date immediately.
		* Usually call it.
		* @param[in] path Image Path in disk.
		* @noto Do not execute Texture::Texture(const std::string& path).
		*/
		Texture2DCube(const std::string& path);

		/**
		* @brief Destructor Function.
		*/
		virtual ~Texture2DCube() override = default;

	private:

		/**
		* Allow TextureLoader access all data.
		*/
		friend class TextureLoader;
	};
}