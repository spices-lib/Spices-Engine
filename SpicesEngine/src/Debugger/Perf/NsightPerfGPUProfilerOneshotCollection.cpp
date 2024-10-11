/**
* @file NsightPerfGPUProfilerOneshotCollection.cpp
* @brief The NsightPerfGPUProfilerOneshotCollection Class Implementation.
* @author Spices
*/

#include "Pchheader.h"
#include "NsightPerfGPUProfilerOneshotCollection.h"
#include "NsightPerfHelpers.h"
#include "Render/FrameInfo.h"

#include <NvPerfCounterConfiguration.h>
#include <NvPerfCpuMarkerTrace.h>
#include <NvPerfVulkan.h>
#include <NvPerfReportGeneratorVulkan.h>

namespace Spices {

    std::shared_ptr<NsightPerfGPUProfilerOneshotCollection> NsightPerfGPUProfilerOneshotCollection::m_NsightPerfGPUProfilerOneshotCollection;

    NsightPerfGPUProfilerOneshotCollection::NsightPerfGPUProfilerOneshotCollection(VulkanState& state)
        : m_VulkanState(state)
        , m_IsInSession(false)
    {
        SPICES_PROFILE_ZONE;

        Create(state);
        //Reset();
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

        const size_t samplingIntervalInNanoSeconds = 1024 * 16;
        const size_t maxIntervalPerFrameInNanoSeconds = 100 * 1000 * 1000; // 100ms
        const size_t numFramesToSample = MaxFrameInFlight;

        const size_t numRangesPerFrame = 50;
        auto onStopSampling = [this](const char* outputDirectory) {
            m_OutputDirectory = outputDirectory;
        };
        NSPERF_CHECK(m_PeriodicSamplerOneShot.Initialize(
            state.m_Instance                 , 
            state.m_PhysicalDevice           , 
            state.m_Device                   , 
            samplingIntervalInNanoSeconds    , 
            maxIntervalPerFrameInNanoSeconds , 
            numRangesPerFrame, 
            onStopSampling
        ))

        m_PeriodicSamplerOneShot.m_outputOption.directoryName  = SPICES_GPUPROFILEONESHOT_PATH;
        m_PeriodicSamplerOneShot.m_outputOption.appendDateTime = nv::perf::sampler::PeriodicSamplerOneShotVulkan::AppendDateTime::yes;

        m_FrameLevelTraceIndice.resize(numFramesToSample, 0);
        m_ApiTracers.resize(numFramesToSample);
        for (auto& apiTracer : m_ApiTracers)
        {
            NSPERF_CHECK(apiTracer.Initialize(state.m_Instance, state.m_PhysicalDevice, state.m_Device, numRangesPerFrame))
        }
    }

    void NsightPerfGPUProfilerOneshotCollection::BeginFrame(VkCommandBuffer commandBuffer)
    {
        SPICES_PROFILE_ZONE;

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

        NSPERF_CHECK(m_PeriodicSamplerOneShot.OnFrameEnd())

        if (!m_OutputDirectory.empty())
        {
            std::vector<nv::perf::mini_trace::APITraceData> apiTraceData;
            for (auto& apiTracer : m_ApiTracers)
            {
                NSPERF_CHECK(apiTracer.ResolveQueries(apiTraceData))
            }
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
            m_OutputDirectory = "";
        }

        nv::perf::sampler::PeriodicSamplerOneShotVulkan::SamplerStatus samplerStatus = m_PeriodicSamplerOneShot.GetSamplerStatus();
        if (samplerStatus == nv::perf::sampler::PeriodicSamplerOneShotVulkan::SamplerStatus::Failed)
        {
            SPICES_CORE_ERROR("Nsight Perf: Failed to initialize");
        }
        else if (samplerStatus == nv::perf::sampler::PeriodicSamplerOneShotVulkan::SamplerStatus::Initialized)
        {}
        else if (samplerStatus == nv::perf::sampler::PeriodicSamplerOneShotVulkan::SamplerStatus::Sampling)
        {
            SPICES_CORE_INFO("Nsight Perf: One-Shot is sampling.");
        }
    }

    void NsightPerfGPUProfilerOneshotCollection::Reset()
    {
        SPICES_PROFILE_ZONE;

        NSPERF_CHECK(m_PeriodicSamplerOneShot.Reset())
    }

    void NsightPerfGPUProfilerOneshotCollection::CaptureFrame()
    {
        SPICES_PROFILE_ZONE;

        m_PeriodicSamplerOneShot.StartCollectionOnFrameEnd();
    }

}