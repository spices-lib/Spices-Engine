/**
* @file VulkanDebugUtils.h.
* @brief The VulkanDebugUtils Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"

namespace Spiecs {

	/**
	* @brief This Class Defines static function for helping vulkan debug.	
	*/
	class VulkanDebugUtils
	{
	public:

		/**
		* @brief Init all vulkan debug function pointer.
		* @param[in] instance VkInstance.
		*/
		static void Init(VkInstance instance);

		/**
		* @brief Start Record Commands with a Label.
		* @param[in] cmdbuffer The VkCommandBuffer in use this frame.
		* @param[in] caption The label be captured.
		* @param[in] color The label color.
		*/
		static void BeginLabel(
			VkCommandBuffer    cmdbuffer              , 
			const std::string& caption                , 
			glm::vec4          color = glm::vec4(1.0f)
		);

		/**
		* @brief End Record Commands with a Label.
		* @param[in] cmdbuffer The VkCommandBuffer in use this frame.
		*/
		static void EndLabel(VkCommandBuffer cmdbuffer);

		/**
		* @brief Insert Record Commands with a Label.
		* @param[in] cmdbuffer The VkCommandBuffer in use this frame.
		* @param[in] caption The label be captured.
		* @param[in] color The label color.
		*/
		static void InsertLabel(
			VkCommandBuffer    cmdbuffer               , 
			const std::string& caption                 , 
			glm::vec4          color = glm::vec4(1.0f)
		);

		/**
		* @brief Start Record Queue with a Label.
		* @param[in] queue The VkQueue in use.
		* @param[in] caption The label be captured.
		* @param[in] color The label color.
		*/
		static void BeginQueueLable(
			VkQueue            queue                  , 
			const std::string& caption                , 
			glm::vec4          color = glm::vec4(1.0f)
		);

		/**
		* @brief End Record Queue with a Label.
		* @param[in] queue The VkQueue in use.
		*/
		static void EndQueueLabel(VkQueue queue);

		/**
		* @brief Insert Record Queue with a Label.
		* @param[in] queue The VkQueue in use.
		* @param[in] caption The label be captured.
		* @param[in] color The label color.
		*/
		static void InsertQueueLabel(
			VkQueue            queue                   , 
			const std::string& caption                 , 
			glm::vec4          color = glm::vec4(1.0f)
		);

		/**
		* @brief Set Vulkan Object with a name, which can be captured.
		* @param[in] type The VkObjectType of Specific Vulkan Object.
		* @param[in] handle The handle of Specific Vulkan Object.
		* @param[in] device VkDevice.
		* @param[in] caption The Vulkan Object name.
		*/
		template<typename T>
		static void SetObjectName(
			VkObjectType       type    , 
			T&                 handle  , 
			VkDevice&          device  , 
			const std::string& caption
		);

		/**
		* @brief Set Vulkan Object with tags, which can be captured.
		* @param[in] type The VkObjectType of Specific Vulkan Object.
		* @param[in] handle The handle of Specific Vulkan Object.
		* @param[in] device VkDevice.
		* @param[in] captions The Vulkan Object tags.
		*/
		template<typename T>
		static void SetObjectTag(
			VkObjectType       type     , 
			T&                 handle   , 
			VkDevice&          device   , 
			std::vector<char*> captions
		);

	private:

		/**
		* @brief The function pointer of vkCmdBeginDebugUtilsLabelEXT.
		*/
		static PFN_vkCmdBeginDebugUtilsLabelEXT     vkCmdBeginDebugUtilsLabelEXT;

		/**
		* @brief The function pointer of vkCmdEndDebugUtilsLabelEXT.
		*/
		static PFN_vkCmdEndDebugUtilsLabelEXT       vkCmdEndDebugUtilsLabelEXT;

		/**
		* @brief The function pointer of vkCmdInsertDebugUtilsLabelEXT.
		*/
		static PFN_vkCmdInsertDebugUtilsLabelEXT    vkCmdInsertDebugUtilsLabelEXT;

		/**
		* @brief The function pointer of vkQueueBeginDebugUtilsLabelEXT.
		*/
		static PFN_vkQueueBeginDebugUtilsLabelEXT   vkQueueBeginDebugUtilsLabelEXT;

		/**
		* @brief The function pointer of vkQueueEndDebugUtilsLabelEXT.
		*/
		static PFN_vkQueueEndDebugUtilsLabelEXT     vkQueueEndDebugUtilsLabelEXT;

		/**
		* @brief The function pointer of vkQueueInsertDebugUtilsLabelEXT.
		*/
		static PFN_vkQueueInsertDebugUtilsLabelEXT  vkQueueInsertDebugUtilsLabelEXT;

		/**
		* @brief The function pointer of vkSetDebugUtilsObjectNameEXT.
		*/
		static PFN_vkSetDebugUtilsObjectNameEXT     vkSetDebugUtilsObjectNameEXT;

		/**
		* @brief The function pointer of vkSetDebugUtilsObjectTagEXT.
		*/
		static PFN_vkSetDebugUtilsObjectTagEXT      vkSetDebugUtilsObjectTagEXT;
	};

	template<typename T>
	inline void VulkanDebugUtils::SetObjectName(
		VkObjectType       type    , 
		T&                 handle  , 
		VkDevice&          device  , 
		const std::string& caption
	)
	{
		/**
		* @brief Only Show Debug info with Debug Mode.
		*/
#ifdef SPIECS_RELEASE
		return;
#endif

		/**
		* @brief Return with a warn info if not find function pointer.
		*/
		if (!vkSetDebugUtilsObjectNameEXT)
		{
			std::stringstream ss;
			ss << "VulkanDebugUtils:: Function vkSetDebugUtilsObjectNameEXT address was not found.";

			SPIECS_CORE_WARN(ss.str());

			return;
		}

		/**
		* @brief Instance a VkDebugUtilsObjectNameInfoEXT.
		*/
		VkDebugUtilsObjectNameInfoEXT name_info{};
		name_info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
		name_info.objectType   = type;
		name_info.objectHandle = (uint64_t)handle;
		name_info.pObjectName  = caption.c_str();

		/**
		* @brief Execute the function pointer.
		*/
		vkSetDebugUtilsObjectNameEXT(device, &name_info);
	}

	template<typename T>
	inline void VulkanDebugUtils::SetObjectTag(VkObjectType type, T& handle, VkDevice& device, std::vector<char*> captions)
	{
		/**
		* @brief Only Show Debug info with Debug Mode.
		*/
#ifdef SPIECS_RELEASE
		return;
#endif

		/**
		* @brief Return with a warn info if not find function pointer.
		*/
		if (!vkSetDebugUtilsObjectTagEXT)
		{
			std::stringstream ss;
			ss << "VulkanDebugUtils:: Function vkSetDebugUtilsObjectTagEXT address was not found.";

			SPIECS_CORE_WARN(ss.str());

			return;
		}

		/**
		* @brief Instance a VkDebugUtilsObjectNameInfoEXT.
		*/
		VkDebugUtilsObjectTagInfoEXT tag_info{};
		tag_info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_TAG_INFO_EXT;
		tag_info.objectType   = type;
		tag_info.objectHandle = (uint64_t)handle;
		tag_info.tagName      = 0;
		tag_info.tagSize      = scaptions.size();
		tag_info.pTag         = captions.data();

		/**
		* @brief Execute the function pointer.
		*/
		vkSetDebugUtilsObjectTagEXT(device, &info);
	}
}