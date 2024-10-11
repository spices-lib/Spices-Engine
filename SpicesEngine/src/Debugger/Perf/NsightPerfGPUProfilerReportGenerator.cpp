#include "Pchheader.h"
#include "NsightPerfHelpers.h"
#include "NsightPerfGPUProfilerReportGenerator.h"

namespace Spices {

	std::shared_ptr<NsightPerfGPUProfilerReportGenerator> NsightPerfGPUProfilerReportGenerator::m_NsightPerfGPUProfilerReportGenerator;

	NsightPerfGPUProfilerReportGenerator::NsightPerfGPUProfilerReportGenerator(VulkanState& state)
		: m_ClockStatus(NVPW_Device_ClockStatus::NVPW_DEVICE_CLOCK_STATUS_UNKNOWN)
		, m_CapturedThisFrame(false)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Initiate collection with the following call.
		*/
		m_NvPerf.outputOptions.directoryName            = SPICES_GPUPROFILEREPORT_PATH;
		m_NvPerf.outputOptions.writeCounterConfigImage  = true;
		m_NvPerf.outputOptions.writeCounterDataImage    = true;
		m_NvPerf.outputOptions.appendDateTimeToDirName  = nv::perf::AppendDateTime::yes;
		m_NvPerf.outputOptions.enableCsvReport          = false;

		/**
		* @brief Initialize the report generator any time after VkDevice initialization. This step determines the list
        * of counters. Specify additionalMetrics before calling InitializeReportGenerator. This is also a
        * good time to decide whether a frame-level range is desirable.
		*/
		m_NvPerf.additionalMetrics = { 
			"zrop__cycles_elapsed",
			"lts__t_sector_hit_rate",
			"crop__write_throughput" 
		};
		NSPERF_CHECK(m_NvPerf.InitializeReportGenerator(state.m_Instance, state.m_PhysicalDevice, state.m_Device))
		m_NvPerf.SetFrameLevelRangeName("Frame");
		m_NvPerf.SetNumNestingLevels(10);
		m_NvPerf.SetMaxNumRanges(100);
		m_NvPerf.SetOpenReportDirectoryAfterCollection(true);

		/**
		* @brief VulkanLoadDriver() must be called first, which is taken care of by InitializeReportGenerator().
		*/
		m_ClockStatus = nv::perf::VulkanGetDeviceClockState(
			state.m_Instance       , 
			state.m_PhysicalDevice , 
			state.m_Device
		);

		NSPERF_CHECK(nv::perf::VulkanSetDeviceClockState(
			state.m_Instance       , 
			state.m_PhysicalDevice , 
			state.m_Device         , 
			NVPW_DEVICE_CLOCK_SETTING_LOCK_TO_RATED_TDP
		))
	}

	void NsightPerfGPUProfilerReportGenerator::CreateInstance(VulkanState& state)
	{
		SPICES_PROFILE_ZONE;

		if (!m_NsightPerfGPUProfilerReportGenerator)
		{
			m_NsightPerfGPUProfilerReportGenerator = std::make_shared<NsightPerfGPUProfilerReportGenerator>(state);
		}
	}

	void NsightPerfGPUProfilerReportGenerator::EndFrame(VulkanState& state)
	{
		SPICES_PROFILE_ZONE;

		NSPERF_CHECK(m_NvPerf.OnFrameEnd())

		if (m_NvPerf.IsCollectingReport())
		{
			/**
			* @brief Wait for all device task over.
			* @note take frmame counts: 98 * SetNumNestingLevels().
			* @todo slate infobar.
			*/
			VK_CHECK(vkDeviceWaitIdle(state.m_Device))
		}
		else if (m_NvPerf.GetInitStatus() != nv::perf::profiler::ReportGeneratorInitStatus::Succeeded)
		{
			SPICES_CORE_ERROR("Nsight Perf: UnKnown failed.");
		}
	}

	void NsightPerfGPUProfilerReportGenerator::BeginFrame(VkQueue queue, uint32_t queueFamilyIndex)
	{
		SPICES_PROFILE_ZONE;

		CollectionNextFrame();
		NSPERF_CHECK(m_NvPerf.OnFrameStart(queue, queueFamilyIndex))
	}

	void NsightPerfGPUProfilerReportGenerator::PushRange(VkCommandBuffer commandBuffer, const std::string& pRangeName)
	{
		SPICES_PROFILE_ZONE;

		NSPERF_CHECK(m_NvPerf.rangeCommands.PushRange(commandBuffer, pRangeName.c_str()))
	}

	void NsightPerfGPUProfilerReportGenerator::PopRange(VkCommandBuffer commandBuffer)
	{
		SPICES_PROFILE_ZONE;

		NSPERF_CHECK(m_NvPerf.rangeCommands.PopRange(commandBuffer))
	}

	void NsightPerfGPUProfilerReportGenerator::Reset(VulkanState& state)
	{
		SPICES_PROFILE_ZONE;

		m_NvPerf.Reset();
		NSPERF_CHECK(nv::perf::VulkanSetDeviceClockState(state.m_Instance, state.m_PhysicalDevice, state.m_Device, m_ClockStatus))
	}

	void NsightPerfGPUProfilerReportGenerator::CollectionNextFrame()
	{
		SPICES_PROFILE_ZONE;

		if (m_CapturedThisFrame)
		{
			NSPERF_CHECK(m_NvPerf.StartCollectionOnNextFrame())
			m_CapturedThisFrame = false;
		}
	}
}