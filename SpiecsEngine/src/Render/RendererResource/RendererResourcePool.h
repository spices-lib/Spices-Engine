/**
* @file RendererResourcePool.h.
* @brief The RendererResourcePool Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "RendererResource.h"

#include <unordered_map>
#include <memory>

namespace Spiecs {

	/**
	* @brief RendererResourcePool Class.
	* This class is a pool of all framebuffer's attachment.
	*/
	class RendererResourcePool
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		RendererResourcePool() {};

		/**
		* @brief Destructor Function.
		*/
		virtual ~RendererResourcePool() {};

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaver.
		*/
		RendererResourcePool(const RendererResourcePool&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaver.
		*/
		RendererResourcePool& operator=(const RendererResourcePool&) = delete;

		/**
		* @brief Called On SwapChian Resized.
		* Event received from VulkanRenderBackeEnd.
		* @param[in] width	The new width of this resource.
		* @param[in] height	The new height of this resource.
		*/
		void OnWindowResized(uint32_t width, uint32_t height);

		/**
		* @brief Get Resource with specific name, create it if it have not been created.
		* @param[in] name The name of the resource.
		* @param[in] info The info used for create resource.
		* @return Returns the view of the resource.
		*/
		VkImageView& AccessResource(const std::string& name, const RendererResourceCreateInfo& info);

		/**
		* @brief Get Depth Resource, create it if it have not been created.
		* @param[in] info The info used for create depth resource.
		* @return Returns the view of the resource.
		*/
		VkImageView& AccessDepthResource(const RendererResourceCreateInfo& info);

	private:

		/**
		* @brief The hashmap of all RendererResource.
		*/
		std::unordered_map<std::string, std::unique_ptr<RendererResource>> m_RendererResource;
	};
}