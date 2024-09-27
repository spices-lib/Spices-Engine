#pragma once
#include "Core/Core.h"
#include "Render/Vulkan/VulkanUtils.h"

#define NV_PERF_ENABLE_INSTRUMENTATION
#ifdef NV_PERF_ENABLE_INSTRUMENTATION
#include <NvPerfHudDataModel.h>
#include <NvPerfHudImPlotRenderer.h>
#include <NvPerfPeriodicSamplerVulkan.h>
#include <NvPerfReportGeneratorVulkan.h>
#endif

namespace Spices {

	class NsightPerf
	{
	public:

		NsightPerf(VulkanState& state);

		virtual ~NsightPerf() = default;

		static void CreateInstance(VulkanState& state);

		static NsightPerf& Get() { return *m_NsightPerf; };

		/**
		* @brief Initialize ImGui, ImPlot and pass the data model into HUD Renderer.
		*/
		void InitHUDRenderer();

		/**
		* @brief Inform the sampler about the frame end, and use the HUD renderer to visualize the populated
		* data model.
		*/
		void RenderHUD();

	private:

		VulkanState& m_VulkanState;
		static std::shared_ptr<NsightPerf> m_NsightPerf;

		nv::perf::sampler::PeriodicSamplerTimeHistoryVulkan m_Sampler;
		nv::perf::hud::HudDataModel m_HudDataModel;
		nv::perf::hud::HudImPlotRenderer m_HudRenderer;
	};

#ifdef SPICES_DEBUG

#define PERF_CREATEINSTANCE(...)                                                   { ::Spices::NsightPerf::CreateInstance(__VA_ARGS__); }
#define PERF_INITHUDRENDERER                                                       { ::Spices::NsightPerf::Get().InitHUDRenderer(); }
#define PERF_RENDERHUD                                                             { ::Spices::NsightPerf::Get().RenderHUD(); }
#endif

#ifdef SPICES_RELEASE

#define PERF_INIT         


#endif

}