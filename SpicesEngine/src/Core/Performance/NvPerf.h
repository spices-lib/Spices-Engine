
//namespace Spices {
//

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