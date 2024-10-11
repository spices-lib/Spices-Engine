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
		* @brief Get this Single Instance.
		* @return Returns this Single Instance.
		*/
		static NsightPerfGPUProfilerOneshotCollection& Get() { return *m_NsightPerfGPUProfilerOneshotCollection; }

		/**
		* @brief Clear Frame Data.
		*/
		void ClearFrame();

		/**
		* @brief Begin recording a Oneshot Collection Range.
		* @param[in] cmd VkCommandBuffer.
		* @param[in] name Range name.
		* @param[in] nestingLevel
		* @param[in] index Range index.
		*/
		void BeginRange(
			VkCommandBuffer cmd          , 
			const char*     name         , 
			size_t          nestingLevel , 
			size_t&         index
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
		* @brief Reset on quit application.
		*/
		void Reset();

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

		nv::perf::sampler::PeriodicSamplerOneShotVulkan m_PeriodicSamplerOneShot;
		std::vector<nv::perf::mini_trace::APITracerVulkan> m_ApiTracers;
		std::vector<size_t> m_FrameLevelTraceIndice;
		std::string m_OutputDirectory;
	};

//#ifdef SPICES_DEBUG
//
//#define NSIGHTPERF_GPUPROFILERONESHOT_CREATEINSTANCE(...)                                                  { ::Spices::NsightPerfGPUProfilerOneshotCollection::CreateInstance(__VA_ARGS__); }
//#define NSIGHTPERF_GPUPROFILERONESHOT_ENDFRAME(...)                                                        { ::Spices::NsightPerfGPUProfilerOneshotCollection::Get().EndFrame(__VA_ARGS__); }
//#define NSIGHTPERF_GPUPROFILERONESHOT_BEGINRANGE(...)                                                      { ::Spices::NsightPerfGPUProfilerOneshotCollection::Get().BeginRange(__VA_ARGS__); }
//#define NSIGHTPERF_GPUPROFILERONESHOT_ENDRANGE(...)                                                        { ::Spices::NsightPerfGPUProfilerOneshotCollection::Get().EndRange(__VA_ARGS__); }
//#define NSIGHTPERF_GPUPROFILERONESHOT_RESET(...)                                                           { ::Spices::NsightPerfGPUProfilerOneshotCollection::Get().Reset(__VA_ARGS__); }
//#define NSIGHTPERF_GPUPROFILERONESHOT_CAPTUREFRAME                                                         { ::Spices::NsightPerfGPUProfilerOneshotCollection::Get().CaptureFrame(); }
//
//#endif
//
//#ifdef SPICES_RELEASE

#define NSIGHTPERF_GPUPROFILERONESHOT_CREATEINSTANCE(...)
#define NSIGHTPERF_GPUPROFILERONESHOT_ENDFRAME(...)      
#define NSIGHTPERF_GPUPROFILERONESHOT_BEGINRANGE(...)    
#define NSIGHTPERF_GPUPROFILERONESHOT_ENDRANGE(...)      
#define NSIGHTPERF_GPUPROFILERONESHOT_RESET(...)         
#define NSIGHTPERF_GPUPROFILERONESHOT_CAPTUREFRAME          

//#endif

}