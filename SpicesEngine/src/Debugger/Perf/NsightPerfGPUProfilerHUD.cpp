/**
* @file NsightPerfGPUProfilerHUD.cpp
* @brief The NsightPerfGPUProfilerHUD Class Implementation.
* @author Spices
*/

#include "Pchheader.h"
#include "NsightPerfGPUProfilerHUD.h"
#include "Render/FrameInfo.h"

#include <NvPerfMiniTraceVulkan.h>

// Note:
//   Do this in exactly one source file to add rapidyaml's symbols.
//   If Windows.h is included before ryml_all.hpp, it needs to be included with NOMINMAX defined.
//   Otherwise min/max-related errors occur.
#define RYML_SINGLE_HDR_DEFINE_NOW
#include <ryml_all.hpp>

#include <windows-desktop-x64/nvperf_host_impl.h>

namespace Spices {

	std::shared_ptr<NsightPerfGPUProfilerHUD> NsightPerfGPUProfilerHUD::m_NsightPerfGPUProfilerHUD;

	NsightPerfGPUProfilerHUD::NsightPerfGPUProfilerHUD(VulkanState& state)
		: m_VulkanState(state)
		, m_IsReachBufferBound(false)
		, m_IsInSession(false)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Create this in construct.
		*/
		Create(state);
	}

	void NsightPerfGPUProfilerHUD::Create(VulkanState& state)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Initialize the sampler any time after VkDevice initialization.
		*/
		m_Sampler.Initialize(state.m_Instance, state.m_PhysicalDevice, state.m_Device);

		/**
		* @brief start a recording session and specify the sampling frequency, maximum decoding latency
		* (explained below) and the number of concurrently unfinished frames (maxFrameLatency).
		*/
		uint32_t samplingFrequencyInHz = 60;
		
		/**
		* @todo There remains a bug about off screen frames still fill in buffer,
		* That will cause buffer is fulled useage.
		* @attention we set Buffer size can hold 1 h data with 60 HZ.
		*/
		uint32_t samplingIntervalInNs = 1000 * 1000 * 1000 / samplingFrequencyInHz; // 1 / 60 s.
		uint32_t maxDecodeLatencyInNs = 1000 * 1000 * 1000 * 10;                    // 10 s.
		uint32_t maxFrameLatency = MaxFrameInFlight + 1;  // requires +1 due to this sample's synchronization model
		m_Sampler.BeginSession(
			state.m_GraphicQueue          ,
			state.m_GraphicQueueFamily    , 
			samplingIntervalInNs          ,
			maxDecodeLatencyInNs          , 
			maxFrameLatency
		);

		/**
		* @brief Select a HUD configuration to record via the HudPresets class¡£
		*/
		nv::perf::hud::HudPresets hudPressets;
		auto deviceIdentifiers = m_Sampler.GetGpuDeviceIdentifiers();
		hudPressets.Initialize(deviceIdentifiers.pChipName);
		m_HudDataModel.Load(hudPressets.GetPreset("SpicesEngineDefault"));
		
		/**
		* @brief Initialize the data model, choose a window of time to store in the TimePlots, and specify the
		* sampling interval.
		*/
		double plotTimeWidthInSeconds = 4.0;
		m_HudDataModel.Initialize(1.0 / samplingFrequencyInHz, plotTimeWidthInSeconds);
		
		/**
		* @brief Pass the newly created counter configuration to the sampler, and prepare the sample-to-frame
		* data processing pipeline.
		*/
		m_Sampler.SetConfig(&m_HudDataModel.GetCounterConfiguration());
		m_HudDataModel.PrepareSampleProcessing(m_Sampler.GetCounterData());
	}

	void NsightPerfGPUProfilerHUD::CreateInstance(VulkanState& state)
	{
		SPICES_PROFILE_ZONE;

		if (!m_NsightPerfGPUProfilerHUD)
		{
			m_NsightPerfGPUProfilerHUD = std::make_shared<NsightPerfGPUProfilerHUD>(state);
		}
	}

	void NsightPerfGPUProfilerHUD::InitHUDRenderer()
	{
		SPICES_PROFILE_ZONE;

		m_HudRenderer.Initialize(m_HudDataModel);

		/**
		* @brief End Session after HudRenderer initalized,
		* for Session can not be owned by multiple instance.
		*/
		Reset();
	}

	void NsightPerfGPUProfilerHUD::RenderHUD()
	{
		SPICES_PROFILE_ZONE;

		m_HudRenderer.Render();
	}

	void NsightPerfGPUProfilerHUD::ConsumeSample()
	{
		SPICES_PROFILE_ZONE;

		if (!m_IsInSession) return;

		bool decoded  = m_Sampler.DecodeCounters();
		bool consumed = m_Sampler.ConsumeSamples([&](
		const uint8_t* pCounterDataImage    , 
		size_t         counterDataImageSize , 
		uint32_t       rangeIndex           , 
		bool&          stop
		) {
			stop = false;
			return m_HudDataModel.AddSample(pCounterDataImage, counterDataImageSize, rangeIndex);
		});

		for (auto& frameDelimiter : m_Sampler.GetFrameDelimiters())
		{
			m_HudDataModel.AddFrameDelimiter(frameDelimiter.frameEndTime);
		}

		m_IsReachBufferBound = !decoded || !consumed;
	}

	void NsightPerfGPUProfilerHUD::QueryDeviceExtensionRequerment(VkInstance instance, VkPhysicalDevice physicalDevice, std::vector<const char*>& deviceExtensionNames)
	{
		SPICES_PROFILE_ZONE;

		std::vector<const char*> deviceExtensionNamesTemp;
		nv::perf::VulkanAppendDeviceRequiredExtensions(instance, physicalDevice, (void*)vkGetInstanceProcAddr, deviceExtensionNamesTemp);

		/**
		* @brief This method returens null extensions back, so do not use it.
		*/
		//nv::perf::mini_trace::MiniTracerVulkan::AppendDeviceRequiredExtensions(vulkanApiVersion, deviceExtensions);

		for (const char* e : deviceExtensionNamesTemp)
		{
			deviceExtensionNames.push_back(e);
		}
	}

	void NsightPerfGPUProfilerHUD::QueryInstanceExtensionRequerment(std::vector<const char*>& instanceExtensionNames, uint32_t apiVersion)
	{
		SPICES_PROFILE_ZONE;

#if 0

		/**
		* @brief Enable this if want define a specific folder for dll search.
		*/
		const char* paths[] = {"G:/NvPerf/lib"};
		NVPW_SetLibraryLoadPaths_Params params{ NVPW_SetLibraryLoadPaths_Params_STRUCT_SIZE };
		params.numPaths = sizeof(paths) / sizeof(paths[0]);
		params.ppPaths = paths;
		NVPW_SetLibraryLoadPaths(&params);

#endif

		std::vector<const char*> instanceExtensions;
		nv::perf::InitializeNvPerf();
		nv::perf::VulkanAppendInstanceRequiredExtensions(instanceExtensions, apiVersion);

		for (const char* e : instanceExtensions)
		{
			instanceExtensionNames.push_back(e);
		}
	}

	void NsightPerfGPUProfilerHUD::EndFrame(VulkanState& state)
	{
		SPICES_PROFILE_ZONE;
		
		if (m_IsInSession)
		{
			m_Sampler.OnFrameEnd();
		}

		/**
		* @brief Last time state.
		*/
		static bool lastInSession = false;

		/**
		* @brief false -> true => BeginSession.
		* true -> false => EndSession.
		*/
		if (!lastInSession && m_IsInSession)
		{
			Create(state);
		}
		if (lastInSession && !m_IsInSession)
		{
			Reset();
		}
		lastInSession = m_IsInSession;
	
		/**
		* @brief Reset and Create.
		*/
		if (m_IsReachBufferBound && m_IsInSession)
		{
			Reset();
			Create(state);
		}
	}

	void NsightPerfGPUProfilerHUD::Reset()
	{
		SPICES_PROFILE_ZONE;

		m_Sampler.Reset();
	}
}