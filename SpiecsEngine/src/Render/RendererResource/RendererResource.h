/**
* @file RendererResource.h.
* @brief The RendererResource Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Resources/Texture/Texture2D.h"

namespace Spiecs{

	/**
	* @brief Forward Declear
	*/
	struct VulkanState;

	/**
	* @brief This struct defines the data used to create a texture2d.
	* From renderpass.
	* @todo More type of texture support.
	*/
	struct RendererResourceCreateInfo
	{
		/**
		* @brief Constructor Function.
		*/
		RendererResourceCreateInfo() {};

		/**
		* @brief Destructor Function.
		*/
		virtual ~RendererResourceCreateInfo() {};

		/**
		* @brief Texture's description info.
		*/
		VkAttachmentDescription description{};

		/**
		* @brief Texture'width.
		*/
		uint32_t width = 1920;

		/**
		* @brief Texture'height..
		*/
		uint32_t height = 1080;

		/**
		* @brief True if this resource is a DepthResource.
		*/
		bool isDepthResource = false;

		/**
		* @brief True if this resource needs resize(sync with viewport's size).
		*/
		bool isResizeable = true;
	};

	/**
	* @brief RendererResource Class.
	* This class is a wapper of framebuffer attachment.
	*/
	class RendererResource
	{
	public:

		/**
		* @brief Constructor Function.
		* Init member veriables.
		* @param[in] info RendererResourceCreateInfo.
		*/
		RendererResource(const RendererResourceCreateInfo& info);

		/**
		* @brief Destructor Function.
		*/
		virtual ~RendererResource() {};

		/**
		* @brief Called On SwapChian Resized.
		* Event received from VulkanRenderBackeEnd.
		* @param[in] width	The new width of this resource.
		* @param[in] height	The new height of this resource.
		*/
		void OnResized(uint32_t width, uint32_t height);

		/**
		* @brief Get Texture this class wappered.
		* @return Returns the reference of unique pointer  of the Texture.
		*/
		inline std::unique_ptr<Texture2D>& GetTexture() { return m_Texture; };

		/**
		* @brief Get isResizeable variable.
		* @return Returns true if can resize.
		*/
		inline const bool IsResizeable() { return m_IsResizeable; };

	private:

		/**
		* @brief True if this resource can resize.
		*/
		bool m_IsResizeable = true;

		/**
		* @brief The information of this resource.
		*/
		RendererResourceCreateInfo m_Info{};

		/**
		* @brief The testure this calss Wappered.
		*/
		std::unique_ptr<Texture2D> m_Texture;
	};
}