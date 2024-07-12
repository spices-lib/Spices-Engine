/**
* @file Texture.h.
* @brief The Texture Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Resources/Loader/TextureLoader.h"

#include <memory>
#include <any>

namespace Spiecs {

	/**
	* @brief The enum of all Texture Type.
	*/
	enum class TextureType
	{
		Texture2D,
		Texture2DArray,
		Texture2DCube,
		MAX
	};

	/**
	* @brief Texture Class.
	* This class defines the basic behaver of texture.
	* When we add an new Texture, we need inherit from this.
	*/
	class Texture
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		Texture() {}

		/**
		* @brief Constructor Function.
		* Init class variable.
		* Usually call it.
		* @param[in] path Image Path in disk.
		*/
		Texture(const std::string& path) : m_ResourcePath(path) {};

		/**
		* @brief Destructor Function.
		*/
		virtual ~Texture() {}

		/**
		* @brief Get Specific resource, usually is a wrapper of VulkanImage.
		* @tparam T How to get the resource.
		* @return Returns the pointer of T.
		* @todo Only returns T.
		*/
		template<typename T>
		std::shared_ptr<T> GetResource();

	protected:

		/**
		* @brief Texture's resource, coule be any kind of type.
		*/
		std::any m_Resource;

		/**
		* @brief Texture's path in disk.
		* @todo Multiple path container.
		*/
		std::string m_ResourcePath;
	};

	template<typename T>
	inline std::shared_ptr<T> Texture::GetResource()
	{
		return std::any_cast<std::shared_ptr<T>>(m_Resource);
	}
}