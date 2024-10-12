/**
* @file NsightPerfGPUProfilerOneshotCollection.h
* @brief The NsightPerfGPUProfilerOneshotCollection Class Definitions.
* @author Spices
*/

#pragma once
#include "Core/Core.h"
#include "Render/Vulkan/VulkanUtils.h"

#include "NvPerfPeriodicSamplerVulkan.h"
#include "NvPerfMiniTraceVulkan.h"

namespace Spices {

	/**
	* @brief Wapper of Nvidia Nsight Perfermance OneshotCollection.
	*/
	class NsightPerfGPUProfilerOneshotCollection
	{
	public:

		using SamplerStatus = nv::perf::sampler::PeriodicSamplerOneShotVulkan::SamplerStatus;

	public:

		/**
		* @brief Constructor Function.
		* @param[in] state VulkanState.
		*/
		NsightPerfGPUProfilerOneshotCollection(VulkanState& state);

		/**
		* @brief Destructor Function.
		*/
		virtual ~NsightPerfGPUProfilerOneshotCollection() = default;

		/**
		* @brief Create this Single Instance.
		* @param[in] state VulkanState.
		*/
		static void CreateInstance(VulkanState& state);

		/**
		* @brief Create this.
		*/
		void Create(VulkanState& state);

		/**
		* @brief Get this Single Instance.
		* @return Returns this Single Instance.
		*/
		static NsightPerfGPUProfilerOneshotCollection& Get() { return *m_NsightPerfGPUProfilerOneshotCollection; }

		/**
		* @brief Begin a Frame.
		* @param[in] state VulkanState.
		* @param[in] commandBuffer VkCommandBuffer.
		*/
		void BeginFrame(VulkanState& state, VkCommandBuffer commandBuffer);

		/**
		* @brief Begin recording a Oneshot Collection Range.
		* @param[in] cmd VkCommandBuffer.
		* @param[in] name Range name.
		* @param[in] nestingLevel
		* @param[in] index Range index.
		*/
		void BeginRange(
			VkCommandBuffer      cmd          , 
			const std::string&   name         ,
			size_t               nestingLevel , 
			uint32_t             index
		);

		/**
		* @brief End recording a Oneshot Collection Range.
		* @param[in] cmd VkCommandBuffer.
		* @param[in] index Range index.
		*/
		void EndRange(VkCommandBuffer cmd, size_t index);

		/**
		* @brief End a Frmae.
		*/
		void EndFrame();

		/**
		* @brief Reset Session.
		*/
		void Reset();

		/**
		* @brief Quit on quit application.
		*/
		void Quit();

		/**
		* @brief Capture this frame.
		*/
		void CaptureFrame();

	private:

		/**
		* @brief VulkanState.
		*/
		VulkanState& m_VulkanState;

		/**
		* @brief This Single Instance.
		*/
		static std::shared_ptr<NsightPerfGPUProfilerOneshotCollection> m_NsightPerfGPUProfilerOneshotCollection;

		/**
		* @brief PeriodicSamplerOneShotVulkan.
		*/
		nv::perf::sampler::PeriodicSamplerOneShotVulkan m_PeriodicSamplerOneShot;

		/**
		* @brief APITracerVulkan Per FlightFrame.
		*/
		std::vector<nv::perf::mini_trace::APITracerVulkan> m_ApiTracers;

		/**
		* @brief FrameLevelTraceIndice.
		*/
		std::vector<size_t> m_FrameLevelTraceIndice;

		/**
		* @brief OutFile Directory.
		*/
		std::string m_OutputDirectory;

		/**
		* @brief True if in Session.
		*/
		bool m_IsInSession;

		/**
		* @brief True if want capture next frame.
		*/
		bool m_EnableCaptureNextFrame;
	};

#ifdef SPICES_DEBUG

#define NSIGHTPERF_GPUPROFILERONESHOT_CREATEINSTANCE(...)                                                  { ::Spices::NsightPerfGPUProfilerOneshotCollection::CreateInstance(__VA_ARGS__); }
#define NSIGHTPERF_GPUPROFILERONESHOT_BEGINFRAME(...)                                                      { ::Spices::NsightPerfGPUProfilerOneshotCollection::Get().BeginFrame(__VA_ARGS__); }
#define NSIGHTPERF_GPUPROFILERONESHOT_ENDFRAME                                                             { ::Spices::NsightPerfGPUProfilerOneshotCollection::Get().EndFrame(); }
#define NSIGHTPERF_GPUPROFILERONESHOT_BEGINRANGE(...)                                                      { ::Spices::NsightPerfGPUProfilerOneshotCollection::Get().BeginRange(__VA_ARGS__); }
#define NSIGHTPERF_GPUPROFILERONESHOT_ENDRANGE(...)                                                        { ::Spices::NsightPerfGPUProfilerOneshotCollection::Get().EndRange(__VA_ARGS__); }
#define NSIGHTPERF_GPUPROFILERONESHOT_QUIT                                                                 { ::Spices::NsightPerfGPUProfilerOneshotCollection::Get().Quit(); }
#define NSIGHTPERF_GPUPROFILERONESHOT_CAPTUREFRAME                                                         { ::Spices::NsightPerfGPUProfilerOneshotCollection::Get().CaptureFrame(); }

#endif

#ifdef SPICES_RELEASE

#define NSIGHTPERF_GPUPROFILERONESHOT_CREATEINSTANCE(...)
#define NSIGHTPERF_GPUPROFILERONESHOT_BEGINFRAME(...)
#define NSIGHTPERF_GPUPROFILERONESHOT_ENDFRAME     
#define NSIGHTPERF_GPUPROFILERONESHOT_BEGINRANGE(...)    
#define NSIGHTPERF_GPUPROFILERONESHOT_ENDRANGE(...)      
#define NSIGHTPERF_GPUPROFILERONESHOT_QUIT        
#define NSIGHTPERF_GPUPROFILERONESHOT_CAPTUREFRAME          

#endif

}