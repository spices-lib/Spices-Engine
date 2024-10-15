#include "Pchheader.h"
#include "TracyProfilerWapper.h"
#include "Render/Vulkan/VulkanUtils.h"

namespace Spices {

	std::shared_ptr<TracyGPUContext> TracyGPUContext::m_TracyGPUContext;

	TracyGPUContext::TracyGPUContext(VulkanState& state)
		: m_VulkanState(state)
		, m_Context(nullptr)
	{
		SPICES_PROFILE_ZONE;

		uint32_t count = 0;
		VK_CHECK(state.m_VkFunc.vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(state.m_PhysicalDevice, &count, nullptr))

		std::vector<VkTimeDomainEXT> timeDomains;
		timeDomains.resize(count);
		VK_CHECK(state.m_VkFunc.vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(state.m_PhysicalDevice, &count, timeDomains.data()))

		bool hasDomain = false;
		if (count > 0)
		{
			/**
			* @brief This vector is essential to vkGetCalibratedTimestampsEXT, and only need to be registered once.
			*/
			std::vector<VkCalibratedTimestampInfoEXT> timestampsInfo;

			for (auto& domain : timeDomains)
			{
				/**
				* @brief Initialize in-scope time stamp info variable.
				*/
				VkCalibratedTimestampInfoEXT    info{};
				info.sType                    = VK_STRUCTURE_TYPE_CALIBRATED_TIMESTAMP_INFO_EXT;
				info.pNext                    = nullptr;
				info.timeDomain               = domain;

				/**
				* @brief Push-back timestamp info to timestamps info vector.
				*/
				timestampsInfo.push_back(info);
			}

			/**
			* @brief timestamps vector.
			*/
			std::vector<uint64_t> timestamps;
			timestamps.resize(count);

			/**
			* @brief max deviations vector.
			*/
			std::vector<uint64_t> maxDeviations;
			maxDeviations.resize(count);

			/**
			* @brief Get calibrated timestamps.
			*/
			VK_CHECK(state.m_VkFunc.vkGetCalibratedTimestampsEXT(state.m_Device, timestamps.size(), timestampsInfo.data(), timestamps.data(), maxDeviations.data()))

			std::unordered_map<VkTimeDomainEXT, uint32_t> maps;
			for (auto& domain : timeDomains)
			{
				maps[domain] = 1;
			}

			hasDomain = maps[VK_TIME_DOMAIN_DEVICE_EXT] && maps[VK_TIME_DOMAIN_QUERY_PERFORMANCE_COUNTER_EXT];
		}

		if (!hasDomain)
		{
			std::stringstream ss;
			ss << "This Device not support VK_TIME_DOMAIN_DEVICE_EXT.";

			SPICES_CORE_ERROR(ss.str());
		}

		/**
		* @brief Create Context and set name.
		*/
		m_Context = SPICES_PROFILE_VK_CONTEXHOSTCALIBRATED(state.m_PhysicalDevice, state.m_Device, state.m_VkFunc.vkResetQueryPool, state.m_VkFunc.vkGetPhysicalDeviceCalibrateableTimeDomainsEXT, state.m_VkFunc.vkGetCalibratedTimestampsEXT);
		SPICES_PROFILE_VK_CONTEXTNAME(m_Context, "SpicesEngineVulkanContext", 50);
	}

	void TracyGPUContext::CreateInstance(VulkanState& state)
	{
		SPICES_PROFILE_ZONE;

		if (!m_TracyGPUContext)
		{
			m_TracyGPUContext = std::make_shared<TracyGPUContext>(state);
		}
	}
}