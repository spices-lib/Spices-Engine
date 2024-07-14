/**
* @file Texture2D.h.
* @brief The Texture2D Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Texture.h"

namespace Spices {

	struct RendererResourceCreateInfo;

	/**
	* @brief Texture2D Class.
	* This class defines the basic behaves of texture2D.
	*/
	class Texture2D : public Texture
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		Texture2D() = default;

		/**
		* @brief Constructor Function.
		* Used for create render resource.
		* @param[in] info RendererResourceCreateInfo.
		*/
		Texture2D(const RendererResourceCreateInfo& info);

		/**
		* @brief Constructor Function.
		* Init class variable, load date immediately.
		* Usually call it.
		* @param[in] path Image Path in disk.
		* @noto Do not execute Texture::Texture(const std::string& path).
		*/
		Texture2D(const std::string& path);

		/**
		* @brief Destructor Function.
		*/
		virtual ~Texture2D() override = default;

	private:

		/**
		* Allow TextureLoader access all data.
		*/
		friend class TextureLoader;
	};
}