/**
* @file Texture2D.h.
* @brief The Texture2D Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Texture.h"

namespace Spiecs {

	struct RendererResourceCreateInfo;

	/**
	* @brief Texture2D Class.
	* This class defines the basic behaver of texture2D.
	*/
	class Texture2D : public Texture
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		Texture2D() {};

		/**
		* @brief Constructor Function.
		* Used for create render resource.
		* @param[in] info RendererResourceCreateInfo.
		*/
		Texture2D(const RendererResourceCreateInfo& info);

		/**
		* @brief Constructor Function.
		* Init class variable, load date immediatelly.
		* Usually call it.
		* @param[in] path Image Path in disk.
		* @noto Do not exectue Texture::Texture(const std::string& path).
		*/
		Texture2D(const std::string& path);

		/**
		* @brief Destructor Function.
		*/
		virtual ~Texture2D() {};

	private:

		/**
		* Allow TextureLoader access all data.
		*/
		friend class TextureLoader;
	};
}