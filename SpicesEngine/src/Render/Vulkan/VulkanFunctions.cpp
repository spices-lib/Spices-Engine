/**
* @file VulkanFunctions.cpp.
* @brief The VulkanFunctions Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "VulkanFunctions.h"

namespace Spices {

	void VulkanFunctions::Init(VkInstance instance)
	{
		EXPLAIN_VK_FUNCTION_POINTER(vkCreateDebugUtilsMessengerEXT                  )
		EXPLAIN_VK_FUNCTION_POINTER(vkDestroyDebugUtilsMessengerEXT                 )
																                    
		EXPLAIN_VK_FUNCTION_POINTER(vkCmdBeginDebugUtilsLabelEXT                    )
		EXPLAIN_VK_FUNCTION_POINTER(vkCmdEndDebugUtilsLabelEXT                      )
		EXPLAIN_VK_FUNCTION_POINTER(vkCmdInsertDebugUtilsLabelEXT                   )
																                    
		EXPLAIN_VK_FUNCTION_POINTER(vkQueueBeginDebugUtilsLabelEXT                  )
		EXPLAIN_VK_FUNCTION_POINTER(vkQueueEndDebugUtilsLabelEXT                    )
		EXPLAIN_VK_FUNCTION_POINTER(vkQueueInsertDebugUtilsLabelEXT                 )
																			       
		EXPLAIN_VK_FUNCTION_POINTER(vkSetDebugUtilsObjectNameEXT                    )
		EXPLAIN_VK_FUNCTION_POINTER(vkSetDebugUtilsObjectTagEXT                     )
																			       
		EXPLAIN_VK_FUNCTION_POINTER(vkGetRayTracingShaderGroupHandlesKHR            )
		EXPLAIN_VK_FUNCTION_POINTER(vkCmdTraceRaysKHR                               )
		EXPLAIN_VK_FUNCTION_POINTER(vkCreateRayTracingPipelinesKHR                  )
		EXPLAIN_VK_FUNCTION_POINTER(vkCreateAccelerationStructureKHR                )
		EXPLAIN_VK_FUNCTION_POINTER(vkDestroyAccelerationStructureKHR               )
		EXPLAIN_VK_FUNCTION_POINTER(vkCmdBuildAccelerationStructuresKHR             )
		EXPLAIN_VK_FUNCTION_POINTER(vkCopyAccelerationStructureKHR                  )
		EXPLAIN_VK_FUNCTION_POINTER(vkGetAccelerationStructureDeviceAddressKHR      )
		EXPLAIN_VK_FUNCTION_POINTER(vkCmdWriteAccelerationStructuresPropertiesKHR   )
		EXPLAIN_VK_FUNCTION_POINTER(vkGetAccelerationStructureBuildSizesKHR         )
		EXPLAIN_VK_FUNCTION_POINTER(vkCmdCopyAccelerationStructureKHR               )

		EXPLAIN_VK_FUNCTION_POINTER(vkCmdDrawMeshTasksEXT                           )

		EXPLAIN_VK_FUNCTION_POINTER(vkCreateIndirectCommandsLayoutNV                )
		EXPLAIN_VK_FUNCTION_POINTER(vkDestroyIndirectCommandsLayoutNV               )
		EXPLAIN_VK_FUNCTION_POINTER(vkCmdExecuteGeneratedCommandsNV                 )
		EXPLAIN_VK_FUNCTION_POINTER(vkGetGeneratedCommandsMemoryRequirementsNV      )
		EXPLAIN_VK_FUNCTION_POINTER(vkCmdPreprocessGeneratedCommandsNV              )

		EXPLAIN_VK_FUNCTION_POINTER(vkCmdSetCheckpointNV                            )
	}
}