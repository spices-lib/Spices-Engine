/**
* @file VulkanInstance.h.
* @brief The VulkanInstance Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

namespace Spiecs {

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
		* @param[in] enginename Engine name.
		*/
		VulkanInstance(VulkanState& vulkanState, const std::string& name, const std::string& enginename);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanInstance();

	private:

		/**
		* @brief Create VkApplicationInfo struct.
		* @param[in] name pApplicationName.
		* @param[in] enginename pEngineName.
		* @return Returns a filled VkApplicationInfo struct.
		*/
		VkApplicationInfo CreateApplicationInfo(const std::string& name, const std::string& enginename);

		/**
		* @brief Create VkInstanceCreateInfo struct.
		* @param[in] appInfo pApplicationInfo.
		* @return Returns a filled VkInstanceCreateInfo.
		*/
		VkInstanceCreateInfo CreateInstanceCreateInfo(const VkApplicationInfo& appInfo);

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
		* @brief Get all instance layer requirements our engine needede.
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
		void CreateVulkanSurface();

		/**
		* @brief Static function Called on a vulkan debug message transfered.
		* @param[in] messageSeverity VkDebugUtilsMessageSeverityFlagBitsEXT.
		* @param[in] messageType VkDebugUtilsMessageTypeFlagsEXT.
		* @param[in] pCallbackData VkDebugUtilsMessengerCallbackDataEXT.
		* @param[in] pUserData.
		* @todo Butter Log.
		*/
		static VKAPI_ATTR VkBool32 VKAPI_CALL InstanceDebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData
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
		* @brief Debug Utils Messagers used to execute message callback function.
		* Also debug vkinstance create.
		*/
		VkDebugUtilsMessengerCreateInfoEXT m_DebugMessengerCreateInfo{};
	};
}