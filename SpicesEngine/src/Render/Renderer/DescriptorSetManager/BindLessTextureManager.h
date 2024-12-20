/**
* @file BindLessTextureManager.h
* @brief The BindLessTextureManager Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Render/Vulkan/VulkanDescriptor.h"

namespace Spices {
	
	/**
	* @brief This Class Helps registry a texture to bind less texture descriptor set.
	*/
	class BindLessTextureManager
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		BindLessTextureManager() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~BindLessTextureManager() = default;

		/**
		* @brief Registry a texture to bind less texture descriptor set array index.
		*/
		static uint32_t Registry(const std::string& name);

		/**
		* @brief UnRegistry a texture from bind less texture descriptor set array index.
		*/
		static void UnRegistry(const std::string& name);

	private:
		
		/**
		* @brief Hashmap of texture path and array index.
		*/
		static std::unordered_map<std::string, uint32_t> m_TextureIDMap;

		/**
		* @brief Hashmap of array index and "".
		* @todo might be fill with texture's information.
		*/
		static std::unordered_map<uint32_t, std::string> m_TextureInfoMap;

		/**
		* @brief Mutex of this BindLessTextureManager.
		*/
		static std::mutex m_Mutex;
	};
}