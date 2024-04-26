/**
* @file VulkanWindows.h.
* @brief The VulkanWindows Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

namespace Spiecs {

	struct WindowInfo
	{
		uint32_t width;
		uint32_t height;

		std::string name;
	};

	/**
	* @brief VulkanWindows Class.
	* This class defines the windows behaver.
	*/
	class VulkanWindows : public VulkanObject
	{
	public:

		/**
		* @brief Constructor Function.
		* Create Windows.
		* @param[in] vulkanState The global VulkanState.
		* @param[in] width Init viewport's width.
		* @param[in] height Init viewport's height.
		* @param[in] name Init viewport's title.
		*/
		VulkanWindows(VulkanState& vulkanState, const WindowInfo& initInfo);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanWindows();

		/**
		* @brief Get m_WindowsResized variable.
		* @return Returns the m_WindowsResized variable.
		*/
		inline bool IsResized() { return m_WindowsResized; };

		/**
		* @brief Set m_WindowsResized variable.
		* @param[in] isResized.
		*/
		void SetResized(bool isResized) { m_WindowsResized = isResized; };

	private:

		/**
		* @brief Set all needed GLFW events call back.
		*/
		void SetInternalCallBack();

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