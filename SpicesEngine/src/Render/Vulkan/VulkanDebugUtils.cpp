/**
* @file VulkanDebugUtils.cpp.
* @brief The VulkanDebugUtils Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "VulkanDebugUtils.h"
#include "Render/Vulkan/VulkanRenderBackend.h"

namespace Spices {

	void VulkanDebugUtils::BeginLabel(
		VkCommandBuffer    cmdbuffer , 
		const std::string& caption   , 
		glm::vec4          color
	)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Only Show Debug info with Debug Mode.
		*/
#ifdef SPICES_RELEASE

		return;

#endif

		/**
		* @brief Instance a VkDebugUtilsObjectNameInfoEXT.
		*/
		VkDebugUtilsLabelEXT             labelInfo{};
		labelInfo.sType                = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
		labelInfo.pLabelName           = caption.c_str();

		memcpy(labelInfo.color, &color[0], sizeof(float) * 4);

		/**
		* @brief Execute the function pointer.
		*/
		VulkanRenderBackend::GetState().m_VkFunc.vkCmdBeginDebugUtilsLabelEXT(cmdbuffer, &labelInfo);
	}

	void VulkanDebugUtils::InsertLabel(
		VkCommandBuffer    cmdbuffer , 
		const std::string& caption   , 
		glm::vec4          color
	)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Only Show Debug info with Debug Mode.
		*/
#ifdef SPICES_RELEASE

		return;

#endif

		/**
		* @brief Instance a VkDebugUtilsObjectNameInfoEXT.
		*/
		VkDebugUtilsLabelEXT         labelInfo{};
		labelInfo.sType            = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
		labelInfo.pLabelName       = caption.c_str();

		memcpy(labelInfo.color, &color[0], sizeof(float) * 4);

		/**
		* @brief Execute the function pointer.
		*/
		VulkanRenderBackend::GetState().m_VkFunc.vkCmdInsertDebugUtilsLabelEXT(cmdbuffer, &labelInfo);
	}

	void VulkanDebugUtils::EndLabel(VkCommandBuffer cmdbuffer)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Only Show Debug info with Debug Mode.
		*/
#ifdef SPICES_RELEASE

		return;

#endif

		/**
		* @brief Execute the function pointer.
		*/
		VulkanRenderBackend::GetState().m_VkFunc.vkCmdEndDebugUtilsLabelEXT(cmdbuffer);
	}

	void VulkanDebugUtils::BeginQueueLabel(
		VkQueue            queue   , 
		const std::string& caption , 
		glm::vec4          color
	)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Only Show Debug info with Debug Mode.
		*/
#ifdef SPICES_RELEASE

		return;

#endif

		/**
		* @brief Instance a VkDebugUtilsObjectNameInfoEXT.
		*/
		VkDebugUtilsLabelEXT        labelInfo{};
		labelInfo.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
		labelInfo.pLabelName      = caption.c_str();

		memcpy(labelInfo.color, &color[0], sizeof(float) * 4);

		/**
		* @brief Execute the function pointer.
		*/
		VulkanRenderBackend::GetState().m_VkFunc.vkQueueBeginDebugUtilsLabelEXT(queue, &labelInfo);
	}

	void VulkanDebugUtils::EndQueueLabel(VkQueue queue)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Only Show Debug info with Debug Mode.
		*/
#ifdef SPICES_RELEAS

		return;

#endif

		/**
		* @brief Execute the function pointer.
		*/
		VulkanRenderBackend::GetState().m_VkFunc.vkQueueEndDebugUtilsLabelEXT(queue);
	}

	void VulkanDebugUtils::InsertQueueLabel(VkQueue queue, const std::string& caption, glm::vec4 color)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Only Show Debug info with Debug Mode.
		*/
#ifdef SPICES_RELEASE

		return;

#endif

		/**
		* @brief Instance a VkDebugUtilsObjectNameInfoEXT.
		*/
		VkDebugUtilsLabelEXT        labelInfo{};
		labelInfo.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
		labelInfo.pLabelName      = caption.c_str();

		memcpy(labelInfo.color, &color[0], sizeof(float) * 4);

		/**
		* @brief Execute the function pointer.
		*/
		VulkanRenderBackend::GetState().m_VkFunc.vkQueueInsertDebugUtilsLabelEXT(queue, &labelInfo);
	}

	void VulkanDebugUtils::SetObjectName(
		VkObjectType       type    , 
		uint64_t           handle  , 
		VkDevice&          device  , 
		const std::string& caption
	)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Only Show Debug info with Debug Mode.
		*/
#ifdef SPICES_RELEASE

		return;

#endif

		/**
		* @brief Instance a VkDebugUtilsObjectNameInfoEXT.
		*/
		VkDebugUtilsObjectNameInfoEXT       name_info{};
		name_info.sType                   = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
		name_info.objectType              = type;
		name_info.objectHandle            = handle;
		name_info.pObjectName             = caption.c_str();

		/**
		* @brief Execute the function pointer.
		*/
		VulkanRenderBackend::GetState().m_VkFunc.vkSetDebugUtilsObjectNameEXT(device, &name_info);
	}

	void VulkanDebugUtils::SetObjectTag(
		VkObjectType       type      , 
		uint64_t           handle    , 
		VkDevice&          device    , 
		std::vector<char*> captions
	)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Only Show Debug info with Debug Mode.
		*/
#ifdef SPICES_RELEASE

		return;

#endif

		/**
		* @brief Instance a VkDebugUtilsObjectNameInfoEXT.
		*/
		VkDebugUtilsObjectTagInfoEXT    tag_info{};
		tag_info.sType                = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_TAG_INFO_EXT;
		tag_info.objectType           = type;
		tag_info.objectHandle         = handle;
		tag_info.tagName              = 0;
		tag_info.tagSize              = captions.size();
		tag_info.pTag                 = captions.data();

		/**
		* @brief Execute the function pointer.
		*/
		VulkanRenderBackend::GetState().m_VkFunc.vkSetDebugUtilsObjectTagEXT(device, &tag_info);
	}
}