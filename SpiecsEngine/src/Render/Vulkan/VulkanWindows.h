/**
* @file VulkanWindows.h.
* @brief The VulkanWindows Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

namespace Spiecs {

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
		VulkanWindows(VulkanState& vulkanState, uint32_t width, uint32_t height, const std::string& name);

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
		* @brief ViewPort's width.
		*/
		uint32_t m_Width;

		/**
		* @brief ViewPort's height.
		*/
		uint32_t m_Height;

		/**
		* @brief ViewPort's name.
		*/
		std::string m_WindowsName;

		/**
		* @brief True if viewPort is resized.
		*/
		bool m_WindowsResized = false;
	};
}