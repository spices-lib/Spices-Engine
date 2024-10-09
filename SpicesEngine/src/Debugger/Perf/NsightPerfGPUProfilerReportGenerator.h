/**
* @file NsightPerfGPUProfilerReportGenerator.h
* @brief The NsightPerfGPUProfilerReportGenerator Class Definitions.
* @author Spices
*/

#pragma once
#include "Core/Core.h"
#include "Render/Vulkan/VulkanUtils.h"

#include <NvPerfHudDataModel.h>
#include <NvPerfHudImPlotRenderer.h>
#include <NvPerfPeriodicSamplerVulkan.h>
#include <NvPerfReportGeneratorVulkan.h>

namespace Spices {

	/**
	* @brief Wapper of Nvidia NsightPerf GPU Perfermance ReportGenerator.
	*/
	class NsightPerfGPUProfilerReportGenerator
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] state VulkanState.
		*/
		NsightPerfGPUProfilerReportGenerator(VulkanState& state);

		/**
		* @brief Destructor Function.
		*/
		virtual ~NsightPerfGPUProfilerReportGenerator() = default;

		/**
		* @brief Create this Single Instance.
		* @param[in] state VulkanState.
		*/
		static void CreateInstance(VulkanState& state);

		/**
		* @brief Get this Single Instance.
		* @return Returns this Single Instance.
		*/
		static NsightPerfGPUProfilerReportGenerator& Get() { return *m_NsightPerfGPUProfilerReportGenerator; }

		/**
		* @brief End a Frmae.
		* The vkQueueWaitIdle() call in the code sequence above is a workaround for a driver/OS issue.
		* @param[in] state VulkanState.
		*/
		void EndFrame(VulkanState& state);

		/**
		* @brief Begin a Frmae.
		* OnFrameStart and OnFrameEnd will not perform any operation until collection is initiated.
		* @param[in] queue VkQueue.
		* @param[in] queueFamilyIndex .
		*/
		void BeginFrame(VkQueue queue, uint32_t queueFamilyIndex);

		/**
		* @brief Struct VulkanRangeCommands provides a reliable set of function pointers, that are safe to call
        * on any device (supported, unsupported, or non-NVIDIA). The ReportGeneratorVulkan has a
        * member rangeCommands.
		* @param[in] commandBuffer VkCommandBuffer.
		* @param[in] pRangeName .
		*/
		void PushRange(VkCommandBuffer commandBuffer, const std::string& pRangeName);

		/**
		* @brief Pop Range.
		* @param[in] commandBuffer VkCommandBuffer.
		*/
		void PopRange(VkCommandBuffer commandBuffer);

		/**
		* @brief Reset Reporter.
		* @param[in] state VulkanState.
		*/
		void Reset(VulkanState& state);

		/**
		* @brief Capture next frame and generate report.
		*/
		void CollectionNextFrame();

		/**
		* @brief EnableCapture.
		*/
		void EnableCapture() { m_CapturedThisFrame = true; }

	private:

		/**
		* @brief This Single Instance.
		*/
		static std::shared_ptr<NsightPerfGPUProfilerReportGenerator> m_NsightPerfGPUProfilerReportGenerator;

		/**
		* @brief Nvidia Nsight Perfermance ReportGenerator.
		*/
		nv::perf::profiler::ReportGeneratorVulkan m_NvPerf;

		/**
		* @brief Used to restore clock state when exiting.
		*/
		NVPW_Device_ClockStatus m_ClockStatus;

		/**
		* @brief true if captured this frame.
		*/
		bool m_CapturedThisFrame;
	};

#ifdef SPICES_DEBUG

#define NSIGHTPERF_GPUPROFILERREPORT_CREATEINSTANCE(...)                                                  { ::Spices::NsightPerfGPUProfilerReportGenerator::CreateInstance(__VA_ARGS__); }
#define NSIGHTPERF_GPUPROFILERREPORT_BEGINFRAME(...)                                                      { ::Spices::NsightPerfGPUProfilerReportGenerator::Get().BeginFrame(__VA_ARGS__); }
#define NSIGHTPERF_GPUPROFILERREPORT_ENDFRAME(...)                                                        { ::Spices::NsightPerfGPUProfilerReportGenerator::Get().EndFrame(__VA_ARGS__); }
#define NSIGHTPERF_GPUPROFILERREPORF_PUSHRANGE(...)                                                       { ::Spices::NsightPerfGPUProfilerReportGenerator::Get().PushRange(__VA_ARGS__); }
#define NSIGHTPERF_GPUPROFILERREPORT_POPRANGE(...)                                                        { ::Spices::NsightPerfGPUProfilerReportGenerator::Get().PopRange(__VA_ARGS__); }
#define NSIGHTPERF_GPUPROFILERREPORT_RESET(...)                                                           { ::Spices::NsightPerfGPUProfilerReportGenerator::Get().Reset(__VA_ARGS__); }
#define NSIGHTPERF_GPUPROFILERREPORT_CAPTUREFRAME                                                         { ::Spices::NsightPerfGPUProfilerReportGenerator::Get().EnableCapture(); }

#endif

#ifdef SPICES_RELEASE

#define NSIGHTPERF_GPUPROFILERREPORT_CREATEINSTANCE(...)
#define NSIGHTPERF_GPUPROFILERREPORT_BEGINFRAME(...)    
#define NSIGHTPERF_GPUPROFILERREPORT_ENDFRAME(...)      
#define NSIGHTPERF_GPUPROFILERREPORF_PUSHRANGE(...)     
#define NSIGHTPERF_GPUPROFILERREPORT_POPRANGE(...)      
#define NSIGHTPERF_GPUPROFILERREPORT_RESET(...)         
#define NSIGHTPERF_GPUPROFILERREPORT_CAPTUREFRAME       

#endif

}