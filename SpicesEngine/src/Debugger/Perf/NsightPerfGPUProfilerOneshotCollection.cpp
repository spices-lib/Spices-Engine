/**
* @file NsightPerfGPUProfilerOneshotCollection.cpp
* @brief The NsightPerfGPUProfilerOneshotCollection Class Implementation.
* @author Spices
*/

#include "Pchheader.h"
#include "NsightPerfGPUProfilerOneshotCollection.h"
#include "NsightPerfHelpers.h"
#include "Render/FrameInfo.h"
#include "Core/Library/ProcessLibrary.h"

#include <NvPerfCounterConfiguration.h>
#include <NvPerfCpuMarkerTrace.h>
#include <NvPerfVulkan.h>
#include <NvPerfReportGeneratorVulkan.h>

namespace Spices {

    std::shared_ptr<NsightPerfGPUProfilerOneshotCollection> NsightPerfGPUProfilerOneshotCollection::m_NsightPerfGPUProfilerOneshotCollection;

    NsightPerfGPUProfilerOneshotCollection::NsightPerfGPUProfilerOneshotCollection(VulkanState& state)
        : m_VulkanState(state)
        , m_IsInSession(false)
        , m_EnableCaptureNextFrame(false)
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Create this in construct.
        */
        Create(state);

        /**
        * @brief End Session after initalized,
        * for Session can not be owned by multiple instance.
        */
        Reset();
    }

    void NsightPerfGPUProfilerOneshotCollection::CreateInstance(VulkanState& state)
    {
        SPICES_PROFILE_ZONE;

        if (!m_NsightPerfGPUProfilerOneshotCollection)
        {
            m_NsightPerfGPUProfilerOneshotCollection = std::make_shared<NsightPerfGPUProfilerOneshotCollection>(state);
        }
    }

    void NsightPerfGPUProfilerOneshotCollection::Create(VulkanState& state)
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Sampling Frequence.
        */
        const size_t samplingIntervalInNanoSeconds = 1024 * 16;
        const size_t maxIntervalPerFrameInNanoSeconds = 100 * 1000 * 1000; // 100ms
        const size_t numFramesToSample = MaxFrameInFlight;

        /**
        * @brief Sampling max level depth.
        */
        const size_t numRangesPerFrame = 50;

        /**
        * @brief Get traces.yaml path on sampling finished.
        */
        auto onStopSampling = [this](const char* outputDirectory) {
            m_OutputDirectory = outputDirectory;
        };

        /**
        * @brief Initialize.
        */
        NSPERF_CHECK(m_PeriodicSamplerOneShot.Initialize(
            state.m_Instance                 , 
            state.m_PhysicalDevice           , 
            state.m_Device                   , 
            samplingIntervalInNanoSeconds    , 
            maxIntervalPerFrameInNanoSeconds , 
            numRangesPerFrame                , 
            onStopSampling
        ))

        /**
        * @brief Set Output folder.
        */
        m_PeriodicSamplerOneShot.m_outputOption.directoryName  = SPICES_GPUPROFILEONESHOT_PATH;
        m_PeriodicSamplerOneShot.m_outputOption.appendDateTime = nv::perf::sampler::PeriodicSamplerOneShotVulkan::AppendDateTime::yes;

        /**
        * @brief Initialize apiTracer.
        */
        m_FrameLevelTraceIndice.resize(numFramesToSample, 0);
        m_ApiTracers.resize(numFramesToSample);
        for (auto& apiTracer : m_ApiTracers)
        {
            NSPERF_CHECK(apiTracer.Initialize(state.m_Instance, state.m_PhysicalDevice, state.m_Device, numRangesPerFrame))
        }

        /**
        * @brief Set InSession true.
        */
        m_IsInSession = true;
    }

    void NsightPerfGPUProfilerOneshotCollection::BeginFrame(VulkanState& state, VkCommandBuffer commandBuffer)
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Begin Session if want capture this frame.
        */
        if (m_EnableCaptureNextFrame)
        {
            Create(state);
            m_PeriodicSamplerOneShot.StartCollectionOnFrameEnd();
        }

        /**
        * @brief Clear apiTracer.
        */
        auto& apiTracer = m_ApiTracers[FrameInfo::Get().m_FrameIndex];
        apiTracer.ClearData();
        apiTracer.ResetQueries(commandBuffer);
    }

    void NsightPerfGPUProfilerOneshotCollection::BeginRange(
        VkCommandBuffer        cmd          , 
        const std::string&     name         , 
        size_t                 nestingLevel , 
        uint32_t               index
    )
    {
        SPICES_PROFILE_ZONE;

        auto& apiTracer = m_ApiTracers[FrameInfo::Get().m_FrameIndex];
        NSPERF_CHECK(apiTracer.BeginRange(cmd, name.c_str(), nestingLevel, m_FrameLevelTraceIndice[index]))
    }

    void NsightPerfGPUProfilerOneshotCollection::EndRange(VkCommandBuffer cmd, size_t index)
    {
        SPICES_PROFILE_ZONE;

        auto& apiTracer = m_ApiTracers[FrameInfo::Get().m_FrameIndex];
        NSPERF_CHECK(apiTracer.EndRange(cmd, index))
    }

    void NsightPerfGPUProfilerOneshotCollection::EndFrame()
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief End this frame.
        */
        NSPERF_CHECK(m_PeriodicSamplerOneShot.OnFrameEnd())

        if (!m_OutputDirectory.empty())
        {
            /**
            * @brief ResolveQueries.
            */
            std::vector<nv::perf::mini_trace::APITraceData> apiTraceData;
            for (auto& apiTracer : m_ApiTracers)
            {
                NSPERF_CHECK(apiTracer.ResolveQueries(apiTraceData))
            }

            /**
            * @brief Write traces to files.
            */
            const std::string yamlText = SerializeAPITraceDataToYaml(apiTraceData);
            const std::string filename = std::string(m_OutputDirectory) + NV_PERF_PATH_SEPARATOR + "traces.yaml";
            std::ofstream file(filename);
            if (file.is_open())
            {
                file << yamlText;
            }
            else
            {
                NV_PERF_LOG_ERR(10, "Failed to open file: %s\n", filename.c_str());
            }

            /**
            * @brief Open timelineview
            */
            std::stringstream ss;
            ss << SPICES_EXTENT_PROCESS_PATH << "TimelineViewer/TimelineViewer.exe";
            ProcessLibrary::OpenProcess(ss.str().c_str());

            /**
            * @brief Clear and Reset.
            */
            m_OutputDirectory = "";
            Reset();
            SPICES_CORE_INFO("Nsight Perf: One-Shot sampling Finished.");
        }

        /**
        * @brief Do not want to capture next frame.
        */
        if (m_IsInSession && m_EnableCaptureNextFrame)
        {
            m_EnableCaptureNextFrame = false;
        }

        /**
        * @brief Debug Status.
        */
        SamplerStatus samplerStatus = m_PeriodicSamplerOneShot.GetSamplerStatus();
        if (samplerStatus == SamplerStatus::Failed)
        {
            SPICES_CORE_ERROR("Nsight Perf: Failed to initialize");
        }
        else if (samplerStatus == SamplerStatus::Sampling)
        {
            SPICES_CORE_INFO("Nsight Perf: One-Shot is sampling.");
        }
    }

    void NsightPerfGPUProfilerOneshotCollection::Reset()
    {
        SPICES_PROFILE_ZONE;

        NSPERF_CHECK(m_PeriodicSamplerOneShot.Reset())

        m_IsInSession = false;
    }

    void NsightPerfGPUProfilerOneshotCollection::CaptureFrame()
    {
        SPICES_PROFILE_ZONE;

        m_EnableCaptureNextFrame = true;
    }

}