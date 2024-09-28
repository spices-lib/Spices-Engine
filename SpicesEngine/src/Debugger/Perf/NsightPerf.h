/**
* @file NsightPerf.h
* @brief The NsightPerf Class Definitions.
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

	class NsightPerf
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] state VulkanState.
		*/
		NsightPerf(VulkanState& state);

		/**
		* @brief Destructor Function.
		*/
		virtual ~NsightPerf() = default;

		/**
		* @brief Create this Single Instance.
		* @param[in] state VulkanState.
		*/
		static void CreateInstance(VulkanState& state);

		/**
		* @brief Get this Single Instance.
		* @return Returns this Single Instance.
		*/
		static NsightPerf& Get() { return *m_NsightPerf; }

		/**
		* @brief Initialize ImGui, ImPlot and pass the data model into HUD Renderer.
		*/
		void InitHUDRenderer();

		/**
		* @brief Inform the sampler about the frame end, and use the HUD renderer to visualize the populated
		* data model.
		*/
		void RenderHUD();

		/**
		* @brief Samples to be periodically fetched and processed by the sampler utility classes. Caveat: If this
		* is not done, the sampler can fall into an irrecoverable state. Choose maxDecodeLatency to
		* cover for a large-enough delay. Frame boundaries are recorded so that per-frame values, of e.g.
		* draw call counts, can be shown as well as per-sample values.
		*/
		void ConsumeSample();

		/***
		* @brief Query Device Extensions Requerments.
		* @param[in] vulkanApiVersion Vulkan API Version.
		* @param[in,out] deviceExtensionNames Extensions.
		*/
		void QueryDeviceExtensionRequerment(uint32_t vulkanApiVersion, std::vector<const char*>& deviceExtensionNames);

		/**
		* @brief Query Instance Extension Requerments.
		* @param[in,out] instanceExtensionNames Extensions.
		* @param[in] apiVersion Vulkan API Version.
		*/
		void QueryInstanceExtensionRequerment(std::vector<const char*>& instanceExtensionNames, uint32_t apiVersion);

		/**
		* @brief End Sampler Frame.
		*/
		void EndFrame();

		/**
		* @brief Reset Sampler.
		*/
		void Reset();

	private:

		VulkanState& m_VulkanState;
		static std::shared_ptr<NsightPerf> m_NsightPerf;

		nv::perf::sampler::PeriodicSamplerTimeHistoryVulkan m_Sampler;
		nv::perf::hud::HudDataModel m_HudDataModel;
		nv::perf::hud::HudImPlotRenderer m_HudRenderer;
	};

//#ifdef SPICES_DEBUG
//
//#define PERF_CREATEINSTANCE(...)                                                   { ::Spices::NsightPerf::CreateInstance(__VA_ARGS__); }
//#define PERF_INITHUDRENDERER                                                       { ::Spices::NsightPerf::Get().InitHUDRenderer(); }
//#define PERF_FRAMECONSUME                                                          { ::Spices::NsightPerf::Get().ConsumeSample(); }
//#define PERF_RENDERHUD                                                             { ::Spices::NsightPerf::Get().RenderHUD(); }
//#define PERF_QUERYDEVICEEXTENSION(...)                                             { ::Spices::NsightPerf::Get().QueryDeviceExtensionRequerment(__VA_ARGS__); }
//#define PERF_QUERYINSTANCEEXTENSION(...)                                           { ::Spices::NsightPerf::Get().QueryInstanceExtensionRequerment(__VA_ARGS__); }
//#define PERF_ENDFRAME                                                              { ::Spices::NsightPerf::Get().EndFrame(); }
//#define PERF_RESET                                                                 { ::Spices::NsightPerf::Get().Reset(); }
//
//#endif
//
//#ifdef SPICES_RELEASE

#define PERF_CREATEINSTANCE(...)              
#define PERF_INITHUDRENDERER            
#define PERF_FRAMECONSUME               
#define PERF_RENDERHUD 
#define PERF_QUERYDEVICEEXTENSION(...)  
#define PERF_QUERYINSTANCEEXTENSION(...)
#define PERF_ENDFRAME                   
#define PERF_RESET              

//#endif

}