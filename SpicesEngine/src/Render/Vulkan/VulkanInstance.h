/**
* @file VulkanInstance.h.
* @brief The VulkanInstance Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

namespace Spices {

	/**
	* @brief VulkanInstance Class.
	* This class defines the VulkanInstance behaver.
	* This class is just a wapper of vkinstance and vksurface.
	*/
	class VulkanInstance : public VulkanObject
	{
	public:

		/**
		* @brief Constructor Function.
		* Create vkinstance and vksurface.
		* @param[in] vulkanState The global VulkanState.
		* @param[in] name Application name.
		* @param[in] engineName Engine name.
		*/
		VulkanInstance(
			VulkanState&       vulkanState , 
			const std::string& name        , 
			const std::string& engineName
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanInstance() override;

	private:

		/**
		* @brief Get all instance extension requirements our engine needede.
		* Source 1 : glfw requirements.
		* Source 2 : user Setting.
		* Source 3 : validation layer requirements.
		* @todo Configurable
		*/
		void GetExtensionRequirements();

		/**
		* @brief Iter all our extensions, check whether all satisfied or not.
		* @return Returns true if all Requirements satisfied.
		*/
		bool CheckExtensionRequirementsSatisfied();

		/**
		* @brief Get all instance layer requirements our engine needed.
		* Source 1 : user Setting.
		* @todo Configurable
		*/
		void GetLayerRequirements();

		/**
		* @brief Iter all our layers, check whether all satisfied or not.
		* @return Returns true if all layers satisfied.
		*/
		bool ChecklayerRequirementsSatisfied();

		/**
		* @brief Set Vulkan's debug message callback function pointer.
		* Working with DEBUG mode.
		*/
		void SetVulkanDebugCallbackFuncPointer();

		/**
		* @brief Create a Surface Object.
		*/
		void CreateVulkanSurface() const;

		/**
		* @brief Static function Called on a vulkan debug message transfered.
		* @param[in] messageSeverity VkDebugUtilsMessageSeverityFlagBitsEXT.
		* @param[in] messageType VkDebugUtilsMessageTypeFlagsEXT.
		* @param[in] pCallbackData VkDebugUtilsMessengerCallbackDataEXT.
		* @param[in] pUserData .
		*/
		static VKAPI_ATTR VkBool32 VKAPI_CALL InstanceDebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT          messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT                 messageType,
			const VkDebugUtilsMessengerCallbackDataEXT*     pCallbackData,
			void*                                           pUserData
		);

		/**
		* @brief Set m_DebugMessengerCreateInfo variable.
		*/
		void FillDebugMessengerCreateInfo();

	private:

		/**
		* @brief Instance Extension Properties.
		*/
		std::vector<const char*> m_ExtensionProperties;

		/**
		* @brief Instance Layer Properties.
		*/
		std::vector<const char*> m_LayerProperties;

		/**
		* @brief Parameter for Create/Destroy DebugUtilsMessengerEXT.
		*/
		VkDebugUtilsMessengerEXT m_DebugMessenger{};

		/**
		* @brief Debug Utils Messages used to execute message callback function.
		* Also debug vkinstance create.
		*/
		VkDebugUtilsMessengerCreateInfoEXT m_DebugMessengerCreateInfo{};
	};
}