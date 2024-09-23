/**
* @file VulkanDebugUtils.h.
* @brief The VulkanDebugUtils Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"

namespace Spices {

	/**
	* @brief This Class Defines static function for helping vulkan debug.	
	*/
	class VulkanDebugUtils
	{
	public:

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
		static void BeginQueueLabel(
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
		static void SetObjectName(
			VkObjectType       type    , 
			uint64_t           handle  , 
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
		static void SetObjectTag(
			VkObjectType       type     , 
			uint64_t           handle   , 
			VkDevice&          device   , 
			std::vector<char*> captions
		);
	};

#ifdef SPICES_DEBUG

#define DEBUGUTILS_BEGINLABEL(...)                 { ::Spices::VulkanDebugUtils::BeginLabel       (__VA_ARGS__)	; }
#define DEBUGUTILS_ENDLABEL(...)                   { ::Spices::VulkanDebugUtils::EndLabel         (__VA_ARGS__)	; }
#define DEBUGUTILS_INSERTLABEL(...)                { ::Spices::VulkanDebugUtils::InsertLabel      (__VA_ARGS__)	; }
#define DEBUGUTILS_BEGINQUEUELABEL(...)            { ::Spices::VulkanDebugUtils::BeginQueueLabel  (__VA_ARGS__)	; }
#define DEBUGUTILS_ENDQUEUELABEL(...)              { ::Spices::VulkanDebugUtils::EndQueueLabel    (__VA_ARGS__)	; }
#define DEBUGUTILS_INSERTQUEUELABEL(...)           { ::Spices::VulkanDebugUtils::InsertQueueLabel (__VA_ARGS__)	; }
#define DEBUGUTILS_SETOBJECTNAME(...)              { ::Spices::VulkanDebugUtils::SetObjectName    (__VA_ARGS__)	; }
#define DEBUGUTILS_SETOBJECTTAG(...)               { ::Spices::VulkanDebugUtils::SetObjectTag     (__VA_ARGS__)	; }

#endif

#ifdef SPICES_RELEASE

#define DEBUGUTILS_BEGINLABEL(...)      
#define DEBUGUTILS_ENDLABEL(...)        
#define DEBUGUTILS_INSERTLABEL(...)     
#define DEBUGUTILS_BEGINQUEUELABEL(...) 
#define DEBUGUTILS_ENDQUEUELABEL(...)   
#define DEBUGUTILS_INSERTQUEUELABEL(...)
#define DEBUGUTILS_SETOBJECTNAME(...)   
#define DEBUGUTILS_SETOBJECTTAG(...)                 

#endif

}