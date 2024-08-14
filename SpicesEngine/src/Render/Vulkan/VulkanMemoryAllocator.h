/**
* @file VulkanMemoryAllocator.h.
* @brief The VulkanMemoryAllocator Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"
#include "Render/FrameInfo.h"

namespace Spices {

	class VulkanMemoryAllocator : public VulkanObject
	{
	public:

		/**
		* @brief Constructor Function.
		* Create Specific ThreadPool.
		* @param[in] vulkanState The global VulkanState.
		*/
		VulkanMemoryAllocator(VulkanState& vulkanState);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanMemoryAllocator() override;

	};
}
