/**
* @file NsightPerfReportGenerator.h
* @brief The NsightPerfReportGenerator Class Definitions.
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
	* @brief Wapper of Nvidia Nsight Perfermance ReportGenerator.
	*/
	class NsightPerfReportGenerator
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] state VulkanState.
		*/
		NsightPerfReportGenerator(VulkanState& state);

		/**
		* @brief Destructor Function.
		*/
		virtual ~NsightPerfReportGenerator() = default;

		/**
		* @brief Create this Single Instance.
		* @param[in] state VulkanState.
		*/
		static void CreateInstance(VulkanState& state);

		/**
		* @brief Get this Single Instance.
		* @return Returns this Single Instance.
		*/
		static NsightPerfReportGenerator& Get() { return *m_NsightPerfReportGenerator; }

		/**
		* @brief End a Frmae.
		* The vkQueueWaitIdle() call in the code sequence above is a workaround for a driver/OS issue.
		*/
		void EndFrame();

		/**
		* @brief Begin a Frmae.
		* OnFrameStart and OnFrameEnd will not perform any operation until collection is initiated.
		* @param[in] queue VkQueue.
		* @param[in] queueFamilyIndex .
		*/
		void BeginFrame(VkQueue queue, uint32_t queueFamilyIndex);

		/**
		* @brief Struct VulkanRangeCommands provides a reliable set of function pointers, that are safe to call
        * on any device (supported, unsupported, or non-NVIDIA). The ReportGeneratorVulkan has a
        * member rangeCommands.
		* @param[in] commandBuffer VkCommandBuffer.
		* @param[in] pRangeName .
		*/
		void PushRange(VkCommandBuffer commandBuffer, const std::string& pRangeName);

		/**
		* @brief Pop Range.
		* @param[in] commandBuffer VkCommandBuffer.
		*/
		void PopRange(VkCommandBuffer commandBuffer);

		/**
		* @brief Reset Reporter.
		* @param[in] state VulkanState.
		*/
		void Reset(VulkanState& state);

		/**
		* @brief Capture next frame and generate report.
		*/
		void CollectionNextFrame();

		/**
		* @brief EnableCapture.
		*/
		void EnableCapture() { m_CapturedThisFrame = true; }

	private:

		/**
		* @brief This Single Instance.
		*/
		static std::shared_ptr<NsightPerfReportGenerator> m_NsightPerfReportGenerator;

		/**
		* @brief Nvidia Nsight Perfermance ReportGenerator.
		*/
		nv::perf::profiler::ReportGeneratorVulkan m_NvPerf;

		/**
		* @brief Used to restore clock state when exiting.
		*/
		NVPW_Device_ClockStatus m_ClockStatus;

		/**
		* @brief true if captured this frame.
		*/
		bool m_CapturedThisFrame;
	};

#ifdef SPICES_DEBUG

#define PERFREPORT_CREATEINSTANCE(...)                                                  { ::Spices::NsightPerfReportGenerator::CreateInstance(__VA_ARGS__); }
#define PERFREPORT_BEGINFRAME(...)                                                      { ::Spices::NsightPerfReportGenerator::Get().BeginFrame(__VA_ARGS__); }
#define PERFREPORT_ENDFRAME                                                             { ::Spices::NsightPerfReportGenerator::Get().EndFrame(); }
#define PERFREPORF_PUSHRANGE(...)                                                       { ::Spices::NsightPerfReportGenerator::Get().PushRange(__VA_ARGS__); }
#define PERFREPORT_POPRANGE(...)                                                        { ::Spices::NsightPerfReportGenerator::Get().PopRange(__VA_ARGS__); }
#define PERFREPORT_RESET(...)                                                           { ::Spices::NsightPerfReportGenerator::Get().Reset(__VA_ARGS__); }
#define PERFREPORT_CAPTUREFRAME                                                         { ::Spices::NsightPerfReportGenerator::Get().EnableCapture(); }

#endif

#ifdef SPICES_RELEASE

#define PERFREPORT_CREATEINSTANCE(...)  
#define PERFREPORT_BEGINFRAME(...)      
#define PERFREPORT_ENDFRAME        
#define PERFREPORF_PUSHRANGE(...)       
#define PERFREPORT_POPRANGE(...)                  
#define PERFREPORT_RESET(...)
#define PERFREPORT_CAPTUREFRAME

#endif

}