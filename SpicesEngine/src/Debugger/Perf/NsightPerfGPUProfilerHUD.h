/**
* @file NsightPerfGPUProfilerHUD.h
* @brief The NsightPerfGPUProfilerHUD Class Definitions.
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
	* @brief Wapper of Nvidia Nsight Perfermance HUD.
	*/
	class NsightPerfGPUProfilerHUD
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] state VulkanState.
		*/
		NsightPerfGPUProfilerHUD(VulkanState& state);

		/**
		* @brief Destructor Function.
		*/
		virtual ~NsightPerfGPUProfilerHUD() = default;

		/**
		* @brief Create this.
		*/
		void Create(VulkanState& state);

		/**
		* @brief Create this Single Instance.
		* @param[in] state VulkanState.
		*/
		static void CreateInstance(VulkanState& state);

		/**
		* @brief Get this Single Instance.
		* @return Returns this Single Instance.
		*/
		static NsightPerfGPUProfilerHUD& Get() { return *m_NsightPerfGPUProfilerHUD; }

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
		* @brief Set if is needed in session.
		* @param[in] inSession True if is needed in session.
		*/
		void SetInSession(bool inSession) { m_IsInSession = inSession; }

		/**
		* @brief Samples to be periodically fetched and processed by the sampler utility classes. Caveat: If this
		* is not done, the sampler can fall into an irrecoverable state. Choose maxDecodeLatency to
		* cover for a large-enough delay. Frame boundaries are recorded so that per-frame values, of e.g.
		* draw call counts, can be shown as well as per-sample values.
		*/
		void ConsumeSample();

		/***
		* @brief Query Device Extensions Requerments.
		* @param[in] instance VkInstance.
		* @param[in] physicalDevice VkPhysicalDevice.
		* @param[in,out] deviceExtensionNames Extensions.
		*/
		static void QueryDeviceExtensionRequerment(VkInstance instance, VkPhysicalDevice physicalDevice, std::vector<const char*>& deviceExtensionNames);

		/**
		* @brief Query Instance Extension Requerments.
		* @param[in,out] instanceExtensionNames Extensions.
		* @param[in] apiVersion Vulkan API Version.
		*/
		static void QueryInstanceExtensionRequerment(std::vector<const char*>& instanceExtensionNames, uint32_t apiVersion);

		/**
		* @brief End Sampler Frame.
		*/
		void EndFrame(VulkanState& state);

		/**
		* @brief Reset Sampler.
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
		static std::shared_ptr<NsightPerfGPUProfilerHUD> m_NsightPerfGPUProfilerHUD;

		/**
		* @brief Sampler.
		*/
		nv::perf::sampler::PeriodicSamplerTimeHistoryVulkan m_Sampler;

		/**
		* @brief HUD data.
		*/
		nv::perf::hud::HudDataModel m_HudDataModel;

		/**
		* @brief HUD imgui renderer.
		*/
		nv::perf::hud::HudImPlotRenderer m_HudRenderer;

		/**
		* @brief True if Decode and Consume failed.
		*/
		bool m_IsReachBufferBound;

		/**
		* @brief True if in a Session.
		*/
		bool m_IsInSession;
	};

#define NSIGHTPERF_GPUPROFILERHUD_QUERYDEVICEEXTENSION(...)                                             { ::Spices::NsightPerfGPUProfilerHUD::QueryDeviceExtensionRequerment(__VA_ARGS__); }
#define NSIGHTPERF_GPUPROFILERHUD_QUERYINSTANCEEXTENSION(...)                                           { ::Spices::NsightPerfGPUProfilerHUD::QueryInstanceExtensionRequerment(__VA_ARGS__); }

#ifdef SPICES_DEBUG

#define NSIGHTPERF_GPUPROFILERHUD_CREATEINSTANCE(...)                                                   { ::Spices::NsightPerfGPUProfilerHUD::CreateInstance(__VA_ARGS__); }
#define NSIGHTPERF_GPUPROFILERHUD_INITHUDRENDERER                                                       { ::Spices::NsightPerfGPUProfilerHUD::Get().InitHUDRenderer(); }
#define NSIGHTPERF_GPUPROFILERHUD_FRAMECONSUME                                                          { ::Spices::NsightPerfGPUProfilerHUD::Get().ConsumeSample(); }
#define NSIGHTPERF_GPUPROFILERHUD_RENDERHUD                                                             { ::Spices::NsightPerfGPUProfilerHUD::Get().RenderHUD(); }
#define NSIGHTPERF_GPUPROFILERHUD_SETINSESSION(...)                                                     { ::Spices::NsightPerfGPUProfilerHUD::Get().SetInSession(__VA_ARGS__); }
#define NSIGHTPERF_GPUPROFILERHUD_ENDFRAME(...)                                                         { ::Spices::NsightPerfGPUProfilerHUD::Get().EndFrame(__VA_ARGS__); }
#define NSIGHTPERF_GPUPROFILERHUD_RESET                                                                 { ::Spices::NsightPerfGPUProfilerHUD::Get().Reset(); }

#endif

#ifdef SPICES_RELEASE

#define NSIGHTPERF_GPUPROFILERHUD_CREATEINSTANCE(...) 
#define NSIGHTPERF_GPUPROFILERHUD_INITHUDRENDERER     
#define NSIGHTPERF_GPUPROFILERHUD_FRAMECONSUME        
#define NSIGHTPERF_GPUPROFILERHUD_RENDERHUD           
#define NSIGHTPERF_GPUPROFILERHUD_SETINSESSION(...)   
#define NSIGHTPERF_GPUPROFILERHUD_ENDFRAME(...)       
#define NSIGHTPERF_GPUPROFILERHUD_RESET                                         

#endif

}