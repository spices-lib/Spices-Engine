/**
* @file VulkanFunctions.h.
* @brief The VulkanFunctions Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"

namespace Spices {

/**
* @brief Macro for declear function pointer variable in VulkanFunctions.
*/
#define VK_FUNCTION_POINTER(function)          \
	PFN_##function function;

/**
* @brief Macro for explain function pointer variable in VulkanFunctions::Init.
*/
#define EXPLAIN_VK_FUNCTION_POINTER(function)  \
	function = reinterpret_cast<PFN_##function>(vkGetInstanceProcAddr(instance, #function));

	/**
	* @brief Vulkan Function Pointers Collection.
	* It holds all function pointer which need get manually.
	*/
	struct VulkanFunctions
	{
		/**
		* @brief Init All Vulkan Function Pointer.
		* @param[in] instance VkInstance.
		*/
		void Init(VkInstance instance);

		/** 
		* @brief Debug Functions.
		*/
		VK_FUNCTION_POINTER(vkCreateDebugUtilsMessengerEXT                  )
		VK_FUNCTION_POINTER(vkDestroyDebugUtilsMessengerEXT                 )

		/**
		* @brief DebugUtils Function. 
		*/
		VK_FUNCTION_POINTER(vkCmdBeginDebugUtilsLabelEXT                    )
		VK_FUNCTION_POINTER(vkCmdEndDebugUtilsLabelEXT                      )
		VK_FUNCTION_POINTER(vkCmdInsertDebugUtilsLabelEXT                   )
														                    
		VK_FUNCTION_POINTER(vkQueueBeginDebugUtilsLabelEXT                  )
		VK_FUNCTION_POINTER(vkQueueEndDebugUtilsLabelEXT                    )
		VK_FUNCTION_POINTER(vkQueueInsertDebugUtilsLabelEXT                 )
														                    
		VK_FUNCTION_POINTER(vkSetDebugUtilsObjectNameEXT                    )
		VK_FUNCTION_POINTER(vkSetDebugUtilsObjectTagEXT                     )
																            
		/**														            
		* @brief RayTracing Function.					            
		*/														            
		VK_FUNCTION_POINTER(vkGetRayTracingShaderGroupHandlesKHR            )
		VK_FUNCTION_POINTER(vkCmdTraceRaysKHR                               )
		VK_FUNCTION_POINTER(vkCreateRayTracingPipelinesKHR                  )
		VK_FUNCTION_POINTER(vkCreateAccelerationStructureKHR                )
		VK_FUNCTION_POINTER(vkDestroyAccelerationStructureKHR               )
		VK_FUNCTION_POINTER(vkCmdBuildAccelerationStructuresKHR             )
		VK_FUNCTION_POINTER(vkCopyAccelerationStructureKHR                  )
		VK_FUNCTION_POINTER(vkGetAccelerationStructureDeviceAddressKHR      )
		VK_FUNCTION_POINTER(vkCmdWriteAccelerationStructuresPropertiesKHR   )
		VK_FUNCTION_POINTER(vkGetAccelerationStructureBuildSizesKHR         )
		VK_FUNCTION_POINTER(vkCmdCopyAccelerationStructureKHR               )

		/**
		* @brief Mesh Shader Function.
		*/
		VK_FUNCTION_POINTER(vkCmdDrawMeshTasksEXT                           )

		/**
		* @brief DGC Function.
		*/
		VK_FUNCTION_POINTER(vkCreateIndirectCommandsLayoutNV                )
		VK_FUNCTION_POINTER(vkDestroyIndirectCommandsLayoutNV               )
		VK_FUNCTION_POINTER(vkCmdExecuteGeneratedCommandsNV                 )
		VK_FUNCTION_POINTER(vkGetGeneratedCommandsMemoryRequirementsNV      )
		VK_FUNCTION_POINTER(vkCmdPreprocessGeneratedCommandsNV              )
	};
}
