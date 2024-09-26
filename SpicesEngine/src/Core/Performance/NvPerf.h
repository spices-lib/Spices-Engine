#pragma once
#include "Core/Core.h"

#define NV_PERF_ENABLE_INSTRUMENTATION
#ifdef NV_PERF_ENABLE_INSTRUMENTATION
#include <windows-desktop-x64/nvperf_host_impl.h>
#include <NvPerfHudDataModel.h>
#include <NvPerfHudImPlotRenderer.h>
#include <NvPerfPeriodicSamplerVulkan.h>
#include <implot.h>

// Note:
//   Do this in exactly one source file to add rapidyaml's symbols.
//   If Windows.h is included before ryml_all.hpp, it needs to be included with NOMINMAX defined.
//   Otherwise min/max-related errors occur.
#define RYML_SINGLE_HDR_DEFINE_NOW
#include <ryml_all.hpp>
#endif

namespace Spices {

	nv::perf::sampler::PeriodicSamplerTimeHistoryVulkan m_Sampler;
	nv::perf::hud::HudDataModel m_HudDataModel;
	nv::perf::hud::HudImPlotRenderer m_HudRenderer;

	void init()
	{
		/**
		* @brief Initialize the sampler any time after VkDevice initialization.
		*/
		m_Sampler.Initialize(m_Instance, m_PhysicalDevice, m_Device);

		/**
		* @brief start a recording session and specify the sampling frequency, maximum decoding latency
        * (explained below) and the number of concurrently unfinished frames (maxFrameLatency).
		*/
		uint32_t samplingFrequencyInHz = 60;

		uint32_t samplingIntervalInNs = 1000000000 / samplingFrequencyInHz;
		uint32_t maxDecodeLatencyInNs = 1000000000;
		uint32_t maxFrameLatency = 5;
		m_Sampler.BeginSession(m_queue, m_queuindex, samplingIntervalInNs,
			maxDecodeLatencyInNs, maxFrameLatency);

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

		/**
		* @brief Initialize ImGui, ImPlot and pass the data model into HUD Renderer.
		*/
		//ImGui::CreateContext();
		//ImPlot::CreateContext();
		//ImGui_ImplVulkan_Init(...);
		//ImGui_ImplGlfw_InitForVulkan(...);
		nv::perf::hud::HudImPlotRenderer::SetStyle();
		m_HudRenderer.Initialize(m_HudDataModel);
	}

	/**
	* @brief Samples to be periodically fetched and processed by the sampler utility classes. Caveat: If this
    * is not done, the sampler can fall into an irrecoverable state. Choose maxDecodeLatency to
    * cover for a large-enough delay. Frame boundaries are recorded so that per-frame values, of e.g.
    * draw call counts, can be shown as well as per-sample values.
	*/
	void PeriodicOperations()
	{
		m_Sampler.DecodeCounters();
		m_Sampler.ConsumeSamples([&](
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
	}

	/**
	* @brief Inform the sampler about the frame end, and use the HUD renderer to visualize the populated
    * data model.
	*/
	void PerFrameOperations()
	{
		m_Sampler.OnFrameEnd();

		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowSize(ImVec2(400, -1), ImGuiCond_Appearing);
		ImGui::Begin("Graphics General Triage");
		m_hudRenderer.Render();
		ImGui::End();

		ImGui::Render();
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
	}

	/**
	* @brief HudDataModel, HudImPlotRenderer and PeriodicSamplerTimeHistoryVulkan can be destroyed
    * through their respective destructors. ImGui and ImPlot require the following tear-down:
	*/
	void CleanUpOperations()
	{
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImPlot::DestroyContext();
		ImGui::DestroyContext();
	}
}