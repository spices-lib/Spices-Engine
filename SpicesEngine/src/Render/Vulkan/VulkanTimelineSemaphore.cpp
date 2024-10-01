/**
* @file VulkanTimelineSemaphore.cpp.
* @brief The VulkanTimelineSemaphore Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "VulkanTimelineSemaphore.h"

namespace Spices {

	VulkanTimelineSemaphore::VulkanTimelineSemaphore(VulkanState& vulkanState)
		: VulkanObject(vulkanState)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Instance a VkSemaphoreTypeCreateInfo.
		*/
		VkSemaphoreTypeCreateInfo           timelineCreateInfo{};
		timelineCreateInfo.sType           = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO;
		timelineCreateInfo.pNext           = nullptr;
		timelineCreateInfo.semaphoreType   = VK_SEMAPHORE_TYPE_TIMELINE;
		timelineCreateInfo.initialValue    = 0;

		/**
		* @brief Instance a VkSemaphoreCreateInfo.
		*/
		VkSemaphoreCreateInfo                createInfo{};
		createInfo.sType                   = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		createInfo.pNext                   = &timelineCreateInfo;
		createInfo.flags                   = 0;

		/**
		* @brief Create Timeline Semaphore.
		*/
		vkCreateSemaphore(vulkanState.m_Device, &createInfo, nullptr, &m_TimelineSemaphore);
	}

	VulkanTimelineSemaphore::~VulkanTimelineSemaphore()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Destroy Timeline Semaphore.
		*/
		vkDestroySemaphore(m_VulkanState.m_Device, m_TimelineSemaphore, nullptr);
	}

	void VulkanTimelineSemaphore::Wait(uint64_t waitValue)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Instance a VkSemaphoreWaitInfo.
		*/
		VkSemaphoreWaitInfo                  waitInfo{};
		waitInfo.sType                     = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO;
		waitInfo.pNext                     = nullptr;
		waitInfo.flags                     = 0;
		waitInfo.semaphoreCount            = 1;
		waitInfo.pSemaphores               = &m_TimelineSemaphore;
		waitInfo.pValues                   = &waitValue;

		/**
		* @brief Call Wait.
		*/
		vkWaitSemaphores(m_VulkanState.m_Device, &waitInfo, UINT64_MAX);
	}

	void VulkanTimelineSemaphore::Singal(uint64_t signalValue)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Instance a VkSemaphoreSignalInfo.
		*/
		VkSemaphoreSignalInfo               signalInfo{};
		signalInfo.sType                   = VK_STRUCTURE_TYPE_SEMAPHORE_SIGNAL_INFO;
		signalInfo.pNext                   = nullptr;
		signalInfo.semaphore               = m_TimelineSemaphore;
		signalInfo.value                   = signalValue;

		/**
		* @brief Call Signal.
		*/
		vkSignalSemaphore(m_VulkanState.m_Device, &signalInfo);
	}

	uint64_t VulkanTimelineSemaphore::GetValue()
	{
		SPICES_PROFILE_ZONE;

		uint64_t value;
		vkGetSemaphoreCounterValue(m_VulkanState.m_Device, m_TimelineSemaphore, &value);

		return value;
	}
}