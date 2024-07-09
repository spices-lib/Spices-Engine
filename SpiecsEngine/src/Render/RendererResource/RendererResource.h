/**
* @file RendererResource.h.
* @brief The RendererResource Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Resources/Texture/Texture2D.h"
#include "Resources/Texture/Texture2DArray.h"
#include "Resources/Texture/Texture2DCube.h"

namespace Spiecs{

	/**
	* @brief Forward Declare
	*/
	struct VulkanState;

	/**
	* @brief This struct defines the data used to create a texture2d.
	* From render pass.
	* @todo More type of texture support.
	*/
	struct RendererResourceCreateInfo
	{
		/**
		* @brief Constructor Function.
		*/
		RendererResourceCreateInfo() = default;

		/**
		* @brief Constructor Function.
		*/
		RendererResourceCreateInfo(const std::string& name) : name(name) {}

		/**
		* @brief Destructor Function.
		*/
		virtual ~RendererResourceCreateInfo() = default;

		/**
		* @brief Texture's description info.
		*/
		VkAttachmentDescription description {};
		
		/**
		* @brief Texture's name.
		*/
		std::string name;

		/**
		* @brief Texture's Class Type.
		*/
		TextureType type = TextureType::Texture2D;

		/**
		* @brief Texture' width.
		*/
		uint32_t width = 1920;

		/**
		* @brief Texture' height..
		*/
		uint32_t height = 1080;

		/**
		* @brief Texture's layer(1 for Texture2D, 6 for TextureCube, any for TextureArray).
		*/
		uint32_t layer = 1;

		/**
		* @brief Image Usage.
		*/
		VkImageUsageFlagBits usage {};

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
	* This class is a wrapper of framebuffer attachment.
	*/
	class RendererResource
	{
	public:

		/**
		* @brief Constructor Function.
		* Init member variables.
		* @param[in] info RendererResourceCreateInfo.
		*/
		RendererResource(const RendererResourceCreateInfo& info);

		/**
		* @brief Destructor Function.
		*/
		virtual ~RendererResource() = default;

		/**
		* @brief Called On SwapChain Resized.
		* Event received from VulkanRenderBackEnd.
		* @param[in] width	The new width of this resource.
		* @param[in] height	The new height of this resource.
		*/
		void OnResized(uint32_t width, uint32_t height);

		/**
		* @brief Get Texture this class wrapped.
		* @return Returns the reference of unique pointer  of the Texture.
		*/
		std::unique_ptr<Texture2D>& GetTexture() { return m_Texture; }

		/**
		* @brief Get isResizeable variable.
		* @return Returns true if can resize.
		*/
		bool IsResizeable() const { return m_IsResizeable; }

	private:

		/**
		* @brief True if this resource can resize.
		*/
		bool m_IsResizeable = true;

		/**
		* @brief The information of this resource.
		*/
		RendererResourceCreateInfo m_Info {};

		/**
		* @brief The texture this class Wrapped.
		*/
		std::unique_ptr<Texture2D> m_Texture;
	};
}