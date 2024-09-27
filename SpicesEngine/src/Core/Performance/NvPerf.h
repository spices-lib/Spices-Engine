//#pragma once
//#include "Core/Core.h"
//
//#define NV_PERF_ENABLE_INSTRUMENTATION
//#ifdef NV_PERF_ENABLE_INSTRUMENTATION
//#include <windows-desktop-x64/nvperf_host_impl.h>
//#include <NvPerfHudDataModel.h>
//#include <NvPerfHudImPlotRenderer.h>
//#include <NvPerfPeriodicSamplerVulkan.h>
//#include <NvPerfReportGeneratorVulkan.h>
//#include <implot.h>
//
//// Note:
////   Do this in exactly one source file to add rapidyaml's symbols.
////   If Windows.h is included before ryml_all.hpp, it needs to be included with NOMINMAX defined.
////   Otherwise min/max-related errors occur.
//#define RYML_SINGLE_HDR_DEFINE_NOW
//#include <ryml_all.hpp>
//#endif
//
//namespace Spices {
//
//	nv::perf::sampler::PeriodicSamplerTimeHistoryVulkan m_Sampler;
//	nv::perf::hud::HudDataModel m_HudDataModel;
//	nv::perf::hud::HudImPlotRenderer m_HudRenderer;
//
//	/**
//	* @brief Samples to be periodically fetched and processed by the sampler utility classes. Caveat: If this
//    * is not done, the sampler can fall into an irrecoverable state. Choose maxDecodeLatency to
//    * cover for a large-enough delay. Frame boundaries are recorded so that per-frame values, of e.g.
//    * draw call counts, can be shown as well as per-sample values.
//	*/
//	void PeriodicOperations()
//	{
//		m_Sampler.DecodeCounters();
//		m_Sampler.ConsumeSamples([&](
//		const uint8_t* pCounterDataImage    , 
//		size_t         counterDataImageSize , 
//		uint32_t       rangeIndex           , 
//		bool&          stop
//		) {
//			stop = false;
//			return m_HudDataModel.AddSample(pCounterDataImage, counterDataImageSize, rangeIndex);
//		});
//
//		for (auto& frameDelimiter : m_Sampler.GetFrameDelimiters())
//		{
//			m_HudDataModel.AddFrameDelimiter(frameDelimiter.frameEndTime);
//		}
//	}
//
//	// Range Profiler
//
//	nv::perf::profiler::ReportGeneratorVulkan m_nvperf;
//
//	/**
//	* @brief Initialize the report generator any time after VkDevice initialization. This step determines the list
//    * of counters. Specify additionalMetrics before calling InitializeReportGenerator. This is also a
//    * good time to decide whether a frame-level range is desirable.
//	*/
//	void ini()
//	{
//		m_nvperf.additionalMetrics = { "crop_write_throughput" };
//		m_nvperf.InitializeReportGenerator(instance, physicaldevidce, device);
//		m_nvperf.SetFrameLevelRangeName("Frame");
//		m_nvperf.SetNumNestingLevels(10)
//	}
//
//	void pf()
//	{
//		m_nvperf.OnFrameEnd();
//		vkQueueWaitIdle(queue);
//		vkQueuePresentKHR(queue, ...);
//		m_nvperf.OnFrameStart(queue);
//	}
//
//	void initreportcol()
//	{
//		std::string outDir = ...;
//		bool b = m_nvperf.StartCollectionOnNextFrame(outDir, AppendDateTime::yes);
//	}
//
//	void pp()
//	{
//		vkCmdBeginDebugUtilsLabelEXT();
//
//		m_nvperf.rangeCommands.PushRange();
//
//		vkCmdDrawIndexed();
//
//		m_nvperf.rangeCommands.PopRange();
//
//		vkCmdEndDebugUtilsLabelEXT();
//	}
//}