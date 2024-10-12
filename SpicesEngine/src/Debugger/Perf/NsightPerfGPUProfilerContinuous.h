/**
* @file NsightPerfGPUProfilerContinuous.h
* @brief The NsightPerfGPUProfilerContinuous Class Definitions.
* @author Spices
*/

#pragma once
#include "Core/Core.h"
#include "Render/Vulkan/VulkanUtils.h"

#include "NvPerfPeriodicSamplerGpu.h"
#include "NvPerfCounterData.h"
#include "NvPerfMetricsEvaluator.h"

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
		static void CreateInstance(VulkanState& state);

		/**
		* @brief Begin this Session.
		*/
		void Create(VulkanState& state);

		/**
		* @brief Get this Single Instance.
		* @return Returns this Single Instance.
		*/
		static NsightPerfGPUProfilerContinuous& Get() { return *m_NsightPerfGPUProfilerContinuous; }

		/**
		* @brief Begin a frame.
		* @param[in] state VulkanState.
		*/
		void BeginFrame(VulkanState& state);

		/**
		* @brief ConsumeSample each frame.
		*/
		void EndFrame();

		/**
		* @brief Capture this frame.
		*/
		void CaptureFrame();

		/**
		* @brief Reset on quit application.
		*/
		void Reset();

	private:

		/**
		* @brief VulkanState.
		*/
		VulkanState& m_VulkanState;

		/**
		* @brief This Single Instance.
		*/
		static std::shared_ptr<NsightPerfGPUProfilerContinuous> m_NsightPerfGPUProfilerContinuous;

		/**
		* @brief the periodic sampler.
		*/
		nv::perf::sampler::GpuPeriodicSampler sampler;

		/**
		* @brief This is used to store the counter values collected during profiling.
		*/
		nv::perf::sampler::RingBufferCounterData counterData;

		/**
		* @brief Metrics requests.
		*/
		std::vector<NVPW_MetricEvalRequest> metricEvalRequests;

		/**
		* @brief MetricsEvaluator.
		*/
		nv::perf::MetricsEvaluator metricsEvaluator;

		/**
		* @brief True if in session.
		*/
		bool m_IsInSession;

		/**
		* @brief True if want capture next frame.
		*/
		bool m_EnableCaptureNextFrame;
	};

#ifdef SPICES_DEBUG

#define NSIGHTPERF_GPUPROFILERCONTINUOUS_CREATEINSTANCE(...)                                                   { ::Spices::NsightPerfGPUProfilerContinuous::CreateInstance(__VA_ARGS__); }
#define NSIGHTPERF_GPUPROFILERCONTINUOUS_BEGINFRAME(...)                                                       { ::Spices::NsightPerfGPUProfilerContinuous::Get().BeginFrame(__VA_ARGS__); }
#define NSIGHTPERF_GPUPROFILERCONTINUOUS_ENDFRAME                                                              { ::Spices::NsightPerfGPUProfilerContinuous::Get().EndFrame(); }
#define NSIGHTPERF_GPUPROFILERCONTINUOUS_CAPTUREFRAME                                                          //{ ::Spices::NsightPerfGPUProfilerContinuous::Get().CaptureFrame(); }
#define NSIGHTPERF_GPUPROFILERCONTINUOUS_RESET                                                                 { ::Spices::NsightPerfGPUProfilerContinuous::Get().Reset(); }

#endif

#ifdef SPICES_RELEASE

#define NSIGHTPERF_GPUPROFILERCONTINUOUS_CREATEINSTANCE(...)
#define NSIGHTPERF_GPUPROFILERCONTINUOUS_BEGINFRAME(...)    
#define NSIGHTPERF_GPUPROFILERCONTINUOUS_ENDFRAME   
#define NSIGHTPERF_GPUPROFILERCONTINUOUS_CAPTUREFRAME
#define NSIGHTPERF_GPUPROFILERCONTINUOUS_RESET              

#endif

}