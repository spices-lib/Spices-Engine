/**
* @file VulkanDebugUtils.cpp.
* @brief The VulkanDebugUtils Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "VulkanDebugUtils.h"

namespace Spiecs {

	PFN_vkCmdBeginDebugUtilsLabelEXT     VulkanDebugUtils::vkCmdBeginDebugUtilsLabelEXT;
	PFN_vkCmdEndDebugUtilsLabelEXT       VulkanDebugUtils::vkCmdEndDebugUtilsLabelEXT;
	PFN_vkCmdInsertDebugUtilsLabelEXT    VulkanDebugUtils::vkCmdInsertDebugUtilsLabelEXT;

	PFN_vkQueueBeginDebugUtilsLabelEXT   VulkanDebugUtils::vkQueueBeginDebugUtilsLabelEXT;
	PFN_vkQueueEndDebugUtilsLabelEXT     VulkanDebugUtils::vkQueueEndDebugUtilsLabelEXT;
	PFN_vkQueueInsertDebugUtilsLabelEXT  VulkanDebugUtils::vkQueueInsertDebugUtilsLabelEXT;

	PFN_vkSetDebugUtilsObjectNameEXT     VulkanDebugUtils::vkSetDebugUtilsObjectNameEXT;
	PFN_vkSetDebugUtilsObjectTagEXT      VulkanDebugUtils::vkSetDebugUtilsObjectTagEXT;

	void VulkanDebugUtils::Init(VkInstance instance)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Only Show Debug info with Debug Mode.
		*/
#ifdef SPIECS_DEBUG

		vkCmdBeginDebugUtilsLabelEXT    = reinterpret_cast<PFN_vkCmdBeginDebugUtilsLabelEXT>   (vkGetInstanceProcAddr(instance, "vkCmdBeginDebugUtilsLabelEXT"));
		vkCmdEndDebugUtilsLabelEXT      = reinterpret_cast<PFN_vkCmdEndDebugUtilsLabelEXT>     (vkGetInstanceProcAddr(instance, "vkCmdEndDebugUtilsLabelEXT"));
		vkCmdInsertDebugUtilsLabelEXT   = reinterpret_cast<PFN_vkCmdInsertDebugUtilsLabelEXT>  (vkGetInstanceProcAddr(instance, "vkCmdInsertDebugUtilsLabelEXT"));

		vkQueueBeginDebugUtilsLabelEXT  = reinterpret_cast<PFN_vkQueueBeginDebugUtilsLabelEXT> (vkGetInstanceProcAddr(instance, "vkQueueBeginDebugUtilsLabelEXT"));
		vkQueueEndDebugUtilsLabelEXT    = reinterpret_cast<PFN_vkQueueEndDebugUtilsLabelEXT>   (vkGetInstanceProcAddr(instance, "vkQueueEndDebugUtilsLabelEXT"));
		vkQueueInsertDebugUtilsLabelEXT = reinterpret_cast<PFN_vkQueueInsertDebugUtilsLabelEXT>(vkGetInstanceProcAddr(instance, "vkQueueInsertDebugUtilsLabelEXT"));

		vkSetDebugUtilsObjectNameEXT    = reinterpret_cast<PFN_vkSetDebugUtilsObjectNameEXT>   (vkGetInstanceProcAddr(instance, "vkSetDebugUtilsObjectNameEXT"));
		vkSetDebugUtilsObjectTagEXT     = reinterpret_cast<PFN_vkSetDebugUtilsObjectTagEXT>    (vkGetInstanceProcAddr(instance, "vkSetDebugUtilsObjectTagEXT"));

#endif // SPIECS_DEBUG

#ifdef SPIECS_RELEASE

		vkCmdBeginDebugUtilsLabelEXT    = nullptr;
		vkCmdEndDebugUtilsLabelEXT      = nullptr;
		vkCmdInsertDebugUtilsLabelEXT   = nullptr;

		vkQueueBeginDebugUtilsLabelEXT  = nullptr;
		vkQueueEndDebugUtilsLabelEXT    = nullptr;
		vkQueueInsertDebugUtilsLabelEXT = nullptr;

		vkSetDebugUtilsObjectNameEXT    = nullptr;
		vkSetDebugUtilsObjectTagEXT     = nullptr;

#endif // SPIECS_RELEASE

	}

	void VulkanDebugUtils::BeginLabel(
		VkCommandBuffer    cmdbuffer , 
		const std::string& caption   , 
		glm::vec4          color
	)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Only Show Debug info with Debug Mode.
		*/
#ifdef SPIECS_RELEASE
		return;
#endif

		/**
		* @brief Return with a warn info if not find function pointer.
		*/
		if (!vkCmdBeginDebugUtilsLabelEXT) 
		{
			std::stringstream ss;
			ss << "VulkanDebugUtils:: Function vkCmdBeginDebugUtilsLabelEXT address was not found.";

			SPIECS_CORE_WARN(ss.str());

			return;
		}

		/**
		* @brief Instance a VkDebugUtilsObjectNameInfoEXT.
		*/
		VkDebugUtilsLabelEXT labelInfo{};
		labelInfo.sType       = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
		labelInfo.pLabelName  = caption.c_str();
		memcpy(labelInfo.color, &color[0], sizeof(float) * 4);

		/**
		* @brief Execute the function pointer.
		*/
		vkCmdBeginDebugUtilsLabelEXT(cmdbuffer, &labelInfo);
	}

	void VulkanDebugUtils::InsertLabel(
		VkCommandBuffer    cmdbuffer , 
		const std::string& caption   , 
		glm::vec4          color
	)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Only Show Debug info with Debug Mode.
		*/
#ifdef SPIECS_RELEASE
		return;
#endif

		/**
		* @brief Return with a warn info if not find function pointer.
		*/
		if (!vkCmdInsertDebugUtilsLabelEXT)
		{
			std::stringstream ss;
			ss << "VulkanDebugUtils:: Function vkCmdInsertDebugUtilsLabelEXT address was not found.";

			SPIECS_CORE_WARN(ss.str());

			return;
		}

		/**
		* @brief Instance a VkDebugUtilsObjectNameInfoEXT.
		*/
		VkDebugUtilsLabelEXT labelInfo{};
		labelInfo.sType      = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
		labelInfo.pLabelName = caption.c_str();
		memcpy(labelInfo.color, &color[0], sizeof(float) * 4);

		/**
		* @brief Execute the function pointer.
		*/
		vkCmdInsertDebugUtilsLabelEXT(cmdbuffer, &labelInfo);
	}

	void VulkanDebugUtils::EndLabel(VkCommandBuffer cmdbuffer)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Only Show Debug info with Debug Mode.
		*/
#ifdef SPIECS_RELEASE
		return;
#endif

		/**
		* @brief Return with a warn info if not find function pointer.
		*/
		if (!vkCmdEndDebugUtilsLabelEXT) 
		{
			std::stringstream ss;
			ss << "VulkanDebugUtils:: Function vkCmdEndDebugUtilsLabelEXT address was not found.";

			SPIECS_CORE_WARN(ss.str());

			return;
		}

		/**
		* @brief Execute the function pointer.
		*/
		vkCmdEndDebugUtilsLabelEXT(cmdbuffer);
	}

	void VulkanDebugUtils::BeginQueueLabel(
		VkQueue            queue   , 
		const std::string& caption , 
		glm::vec4          color
	)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Only Show Debug info with Debug Mode.
		*/
#ifdef SPIECS_RELEASE
		return;
#endif

		/**
		* @brief Return with a warn info if not find function pointer.
		*/
		if (!vkQueueBeginDebugUtilsLabelEXT)
		{
			std::stringstream ss;
			ss << "VulkanDebugUtils:: Function vkQueueBeginDebugUtilsLabelEXT address was not found.";

			SPIECS_CORE_WARN(ss.str());

			return;
		}

		/**
		* @brief Instance a VkDebugUtilsObjectNameInfoEXT.
		*/
		VkDebugUtilsLabelEXT labelInfo{};
		labelInfo.sType      = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
		labelInfo.pLabelName = caption.c_str();
		memcpy(labelInfo.color, &color[0], sizeof(float) * 4);

		/**
		* @brief Execute the function pointer.
		*/
		vkQueueBeginDebugUtilsLabelEXT(queue, &labelInfo);
	}

	void VulkanDebugUtils::EndQueueLabel(VkQueue queue)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Only Show Debug info with Debug Mode.
		*/
#ifdef SPIECS_RELEAS
		return;
#endif

		/**
		* @brief Return with a warn info if not find function pointer.
		*/
		if (!vkQueueEndDebugUtilsLabelEXT)
		{
			std::stringstream ss;
			ss << "VulkanDebugUtils:: Function vkQueueEndDebugUtilsLabelEXT address was not found.";

			SPIECS_CORE_WARN(ss.str());

			return;
		}

		/**
		* @brief Execute the function pointer.
		*/
		vkQueueEndDebugUtilsLabelEXT(queue);
	}

	void VulkanDebugUtils::InsertQueueLabel(VkQueue queue, const std::string& caption, glm::vec4 color)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Only Show Debug info with Debug Mode.
		*/
#ifdef SPIECS_RELEASE
		return;
#endif

		/**
		* @brief Return with a warn info if not find function pointer.
		*/
		if (!vkQueueInsertDebugUtilsLabelEXT)
		{
			std::stringstream ss;
			ss << "VulkanDebugUtils:: Function vkQueueInsertDebugUtilsLabelEXT address was not found.";

			SPIECS_CORE_WARN(ss.str());

			return;
		}

		/**
		* @brief Instance a VkDebugUtilsObjectNameInfoEXT.
		*/
		VkDebugUtilsLabelEXT labelInfo{};
		labelInfo.sType      = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
		labelInfo.pLabelName = caption.c_str();
		memcpy(labelInfo.color, &color[0], sizeof(float) * 4);

		/**
		* @brief Execute the function pointer.
		*/
		vkQueueInsertDebugUtilsLabelEXT(queue, &labelInfo);
	}
}