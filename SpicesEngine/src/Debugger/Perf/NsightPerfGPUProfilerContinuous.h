/**
* @file NsightPerfGPUProfilerContinuous.h
* @brief The NsightPerfGPUProfilerContinuous Class Definitions.
* @author Spices
*/

#pragma once
#include "Core/Core.h"
#include "Render/Vulkan/VulkanUtils.h"

namespace Spices {

	/**
	* @brief Wapper of Nvidia Nsight Perfermance Metrics.
	*/
	class NsightPerfGPUProfilerContinuous
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] state VulkanState.
		*/
		NsightPerfGPUProfilerContinuous(VulkanState& state);

		/**
		* @brief Destructor Function.
		*/
		virtual ~NsightPerfGPUProfilerContinuous() = default;

		/**
		* @brief Create this Single Instance.
		* @param[in] state VulkanState.
		*/
		//static void CreateInstance(VulkanState& state);

		/**
		* @brief Get this Single Instance.
		* @return Returns this Single Instance.
		*/
		static NsightPerfGPUProfilerContinuous& Get() { return *m_NsightPerfGPUProfilerContinuous; }

	private:

		/**
		* @brief VulkanState.
		*/
		VulkanState& m_VulkanState;

		/**
		* @brief This Single Instance.
		*/
		static std::shared_ptr<NsightPerfGPUProfilerContinuous> m_NsightPerfGPUProfilerContinuous;
	};

}