/**
* @file VulkanWindows.h.
* @brief The VulkanWindows Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

#include <stb_image.h>

namespace Spices {

	/**
	* @brief This struct helps load the icon of window.
	*/
	struct WindowIcon
	{
		/**
		* @brief Constructor Function.
		* Load the icon file immdeiatelly.
		* @param[in] iconPath The file path of icon.
		*/
		WindowIcon(const std::string& iconPath)
		{
			if (image.pixels)
			{
				stbi_image_free(image.pixels);
			}

			int texChannels, width, height;
			image.pixels = stbi_load(iconPath.c_str(), &image.width, &image.height, &texChannels, STBI_rgb_alpha);
			if (!image.pixels)
			{
				SPICES_CORE_ERROR("Failed to load texture image!");
			}
		}

		/**
		* @brief Destructor Function.
		*/
		virtual ~WindowIcon()
		{
			stbi_image_free(image.pixels);
		}

		/**
		* @brief GLFWimage.
		*/
		GLFWimage image { 0, 0, nullptr};
	};

	/**
	* @brief This struct defines the basic information of window.
	*/
	struct WindowInfo
	{
		/**
		* @brief Window's width.
		*/
		int width;

		/**
		* @brief Window's height.
		*/
		int height;

		/**
		* @brief Window's name.
		*/
		std::string name;

		/**
		* @brief Window's icon.
		*/
		std::shared_ptr<WindowIcon> icon;
	};

	/**
	* @brief VulkanWindows Class.
	* This class defines the windows behaves.
	*/
	class VulkanWindows : public VulkanObject
	{
	public:

		/**
		* @brief Constructor Function.
		* Create Windows.
		* @param[in] vulkanState The global VulkanState.
		* @param[in] initInfo WindowInfo.
		*/
		VulkanWindows(
			VulkanState&      vulkanState , 
			const WindowInfo& initInfo
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanWindows() override;

		/**
		* @brief Get m_WindowsResized variable.
		* @return Returns the m_WindowsResized variable.
		*/
		bool IsResized() const { return m_WindowsResized; }

		/**
		* @brief Set m_WindowsResized variable.
		* @param[in] isResized .
		*/
		void SetResized(bool isResized) { m_WindowsResized = isResized; }

	private:

		/**
		* @brief Set all needed GLFW events call back.
		*/
		void SetInternalCallBack() const;

	private:

		/**
		* @brief Window's info.
		* not viewport's info.
		*/
		WindowInfo m_WindowInfo;

		/**
		* @brief True if viewPort is resized.
		*/
		bool m_WindowsResized = false;
	};
}