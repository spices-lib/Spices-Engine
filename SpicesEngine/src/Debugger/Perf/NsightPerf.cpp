#include "Pchheader.h"
#include "NsightPerf.h"

// Note:
//   Do this in exactly one source file to add rapidyaml's symbols.
//   If Windows.h is included before ryml_all.hpp, it needs to be included with NOMINMAX defined.
//   Otherwise min/max-related errors occur.
#define RYML_SINGLE_HDR_DEFINE_NOW
#include <ryml_all.hpp>

#include <windows-desktop-x64/nvperf_host_impl.h>

namespace Spices {

	std::shared_ptr<NsightPerf> NsightPerf::m_NsightPerf;

	NsightPerf::NsightPerf(VulkanState& state)
		: m_VulkanState(state)
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
		
		uint32_t samplingIntervalInNs = 1000000000 / samplingFrequencyInHz;
		uint32_t maxDecodeLatencyInNs = 1000000000;
		uint32_t maxFrameLatency = 5;
		m_Sampler.BeginSession(
			state.m_GraphicQueue       , 
			state.m_GraphicQueueFamily , 
			samplingIntervalInNs       ,
			maxDecodeLatencyInNs       , 
			maxFrameLatency
		);
		
		/**
		* @brief Select a HUD configuration to record via the HudPresets class. Here we select Graphics
		* General Triage.
		*/
		nv::perf::hud::HudPresets hudPressets;
		auto deviceIdentifiers = m_Sampler.GetGpuDeviceIdentifiers();
		hudPressets.Initialize(deviceIdentifiers.pChipName);
		m_HudDataModel.Load(hudPressets.GetPreset("Graphic General Triangle"));
		
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

	void NsightPerf::CreateInstance(VulkanState& state)
	{
		SPICES_PROFILE_ZONE;

		if (!m_NsightPerf)
		{
			m_NsightPerf = std::make_shared<NsightPerf>(state);
		}
	}

	void NsightPerf::InitHUDRenderer()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Thouse API needs be called before this.
		* ImGui::CreateContext();
		* ImPlot::CreateContext();
		* ImGui_ImplGlfw_InitForVulkan(...);
		* ImGui_ImplVulkan_Init(...);
		*/
		nv::perf::hud::HudImPlotRenderer::SetStyle();
		m_HudRenderer.Initialize(m_HudDataModel);
	}

	void NsightPerf::RenderHUD()
	{
		SPICES_PROFILE_ZONE;

		m_HudRenderer.Render();
	}
}