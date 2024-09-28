#include "Pchheader.h"
#include "NsightPerfReportGenerator.h"

namespace Spices {

	std::shared_ptr<NsightPerfReportGenerator> NsightPerfReportGenerator::m_NsightPerfReportGenerator;

	NsightPerfReportGenerator::NsightPerfReportGenerator(VulkanState& state)
		: m_ClockStatus(NVPW_Device_ClockStatus::NVPW_DEVICE_CLOCK_STATUS_UNKNOWN)
		, m_CapturedThisFrame(false)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Initialize the report generator any time after VkDevice initialization. This step determines the list
        * of counters. Specify additionalMetrics before calling InitializeReportGenerator. This is also a
        * good time to decide whether a frame-level range is desirable.
		*/
		m_NvPerf.additionalMetrics = { "crop__write_throughput" };
		m_NvPerf.InitializeReportGenerator(state.m_Instance, state.m_PhysicalDevice, state.m_Device);
		m_NvPerf.SetFrameLevelRangeName("Frame");
		m_NvPerf.SetNumNestingLevels(10);
		m_NvPerf.SetMaxNumRanges(10);

		/**
		* @brief Initiate collection with the following call.
		*/
		std::string outDir = "";
		m_NvPerf.outputOptions.directoryName = "D:/OpenGLProjects/Spices-Engine/SpicesGame";
		//bool b = m_NvPerf.StartCollectionOnNextFrame();

		/**
		* @brief VulkanLoadDriver() must be called first, which is taken care of by InitializeReportGenerator().
		*/
		m_ClockStatus = nv::perf::VulkanGetDeviceClockState(state.m_Instance, state.m_PhysicalDevice, state.m_Device);
		nv::perf::VulkanSetDeviceClockState(state.m_Instance, state.m_PhysicalDevice, state.m_Device, NVPW_DEVICE_CLOCK_SETTING_LOCK_TO_RATED_TDP);
	}

	void NsightPerfReportGenerator::CreateInstance(VulkanState& state)
	{
		SPICES_PROFILE_ZONE;

		if (!m_NsightPerfReportGenerator)
		{
			m_NsightPerfReportGenerator = std::make_shared<NsightPerfReportGenerator>(state);
		}
	}

	void NsightPerfReportGenerator::EndFrame(VkQueue queue)
	{
		SPICES_PROFILE_ZONE;

		m_NvPerf.OnFrameEnd();

		if (m_NvPerf.IsCollectingReport())
		{
			SPICES_CORE_INFO("Nsight Perf: Currently profiling the frame. HTML Report will be written to: ");
		}
		else if (m_NvPerf.GetInitStatus() == nv::perf::profiler::ReportGeneratorInitStatus::Succeeded)
		{
			//SPICES_CORE_INFO("Nsight Perf: Hit <space-bar> to begin profiling.");
		}
		else
		{
			SPICES_CORE_INFO("Nsight Perf: Initialization failed. Please check the logs.");
		}

		vkQueueWaitIdle(queue);     // workaround to avoid hang
	}

	void NsightPerfReportGenerator::BeginFrame(VkQueue queue, uint32_t queueFamilyIndex)
	{
		SPICES_PROFILE_ZONE;

		m_NvPerf.OnFrameStart(queue, queueFamilyIndex);
	}

	void NsightPerfReportGenerator::PushRange(VkCommandBuffer commandBuffer, const std::string& pRangeName)
	{
		SPICES_PROFILE_ZONE;

		m_NvPerf.rangeCommands.PushRange(commandBuffer, pRangeName.c_str());
	}

	void NsightPerfReportGenerator::PopRange(VkCommandBuffer commandBuffer)
	{
		SPICES_PROFILE_ZONE;

		m_NvPerf.rangeCommands.PopRange(commandBuffer);
	}

	void NsightPerfReportGenerator::Reset(VulkanState& state)
	{
		SPICES_PROFILE_ZONE;

		m_NvPerf.Reset();
		nv::perf::VulkanSetDeviceClockState(state.m_Instance, state.m_PhysicalDevice, state.m_Device, m_ClockStatus);
	}

	void NsightPerfReportGenerator::CollectionNextFrame()
	{
		SPICES_PROFILE_ZONE;

		if (m_CapturedThisFrame)
		{
			m_NvPerf.StartCollectionOnNextFrame();
			m_CapturedThisFrame = false;
		}
	}
}