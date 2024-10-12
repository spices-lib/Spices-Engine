/**
* @file NsightPerfGPUProfilerContinuous.cpp
* @brief The NsightPerfGPUProfilerContinuous Class Implementation.
* @author Spices
*/

#include "Pchheader.h"
#include "NsightPerfGPUProfilerContinuous.h"
#include "NsightPerfHelpers.h"
#include "Render/FrameInfo.h"

#include <NvPerfCounterConfiguration.h>
#include <NvPerfCpuMarkerTrace.h>
#include <NvPerfVulkan.h>

namespace Spices {

	std::shared_ptr<NsightPerfGPUProfilerContinuous> NsightPerfGPUProfilerContinuous::m_NsightPerfGPUProfilerContinuous;

    /**
    * @brief The following metrics are for demonstration purposes only. 
    * For a more comprehensive set of single-pass metrics, please refer to the 'HudConfigurations'.
    */
    const char* Metrics[] = {
        "gpc__cycles_elapsed.avg.per_second",
        "sys__cycles_elapsed.avg.per_second",
        "lts__cycles_elapsed.avg.per_second",
    };

	NsightPerfGPUProfilerContinuous::NsightPerfGPUProfilerContinuous(VulkanState& state)
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

    void NsightPerfGPUProfilerContinuous::CreateInstance(VulkanState& state)
    {
        SPICES_PROFILE_ZONE;

        if (!m_NsightPerfGPUProfilerContinuous)
        {
            m_NsightPerfGPUProfilerContinuous = std::make_shared<NsightPerfGPUProfilerContinuous>(state);
        }
    }

    void NsightPerfGPUProfilerContinuous::Create(VulkanState& state)
    {
        SPICES_PROFILE_ZONE;

        //NSPERF_CHECK(nv::perf::InitializeNvPerf())
        NSPERF_CHECK(nv::perf::VulkanIsNvidiaDevice(state.m_PhysicalDevice))
        const size_t deviceIndex = nv::perf::VulkanGetNvperfDeviceIndex(state.m_Instance, state.m_PhysicalDevice, state.m_Device);

        /**
        * @brief Initialize the periodic sampler.
        */ 
        NSPERF_CHECK(sampler.Initialize(deviceIndex))
        const nv::perf::DeviceIdentifiers deviceIdentifiers = sampler.GetDeviceIdentifiers();

        /**
        * @brief Create the metrics evaluator.
        */
        {
            std::vector<uint8_t> metricsEvaluatorScratchBuffer;
            NVPW_MetricsEvaluator* pMetricsEvaluator = nv::perf::sampler::DeviceCreateMetricsEvaluator(metricsEvaluatorScratchBuffer, deviceIdentifiers.pChipName);

            /**
            * @brief transfer ownership to m_metricsEvaluator.
            */
            metricsEvaluator = nv::perf::MetricsEvaluator(pMetricsEvaluator, std::move(metricsEvaluatorScratchBuffer));
        }

        /**
        * @brief Create the config builder, this is used to create a counter configuration.
        */
        nv::perf::MetricsConfigBuilder configBuilder;
        {
            NVPA_RawMetricsConfig* pRawMetricsConfig = nv::perf::sampler::DeviceCreateRawMetricsConfig(deviceIdentifiers.pChipName);

            /**
            * @brief transfer pRawMetricsConfig's ownership to configBuilder.
            */
            NSPERF_CHECK(configBuilder.Initialize(metricsEvaluator, pRawMetricsConfig, deviceIdentifiers.pChipName))
        }

        /**
        * @brief Add metrics into config builder.
        */
        for (size_t ii = 0; ii < sizeof(Metrics) / sizeof(Metrics[0]); ++ii)
        {
            const char* const pMetric = Metrics[ii];
            NVPW_MetricEvalRequest request{};
            NSPERF_CHECK(ToMetricEvalRequest(metricsEvaluator, pMetric, request))

            /**
            * @brief By setting "keepInstances" to false, the counter data will only store GPU-level values, reducing its size and improving the performance of metric evaluation.
            * However, this option has the drawback of making max/min submetrics non-evaluable.
            */
            const bool keepInstances = false;
            NSPERF_CHECK(configBuilder.AddMetrics(&request, 1, keepInstances))
            metricEvalRequests.emplace_back(std::move(request));
        }

        /**
        * @brief Create the counter configuration out of the config builder.
        */
        nv::perf::CounterConfiguration counterConfiguration;
        NSPERF_CHECK(CreateConfiguration(configBuilder, counterConfiguration))
        
        /**
        * @brief Periodic sampler supports only single-pass configurations, meaning that all scheduled metrics must be collectable in a single pass.
        */
        assert(counterConfiguration.numPasses == 1);

        /**
        * @brief Initialize the counter data
        * Below setting determines the maximum size of a counter data image. However, because the counter data here is requested to work in the ring buffer mode,
        * when the put pointer reaches the end, it will start from the beginning and overwrite previous data even if it hasn't been read yet.
        * Therefore, the size specified here must be sufficient to cover the latency.
        */
        const uint32_t MaxSamples = 1024;
        const bool Validate = true; // Setting this to true enables extra validation, which is useful for debugging. In production environments, it can be set to false for improved performance.
        NSPERF_CHECK(counterData.Initialize(
        MaxSamples , 
        Validate   , 
        [&](
            uint32_t                                    maxSamples  , 
            NVPW_PeriodicSampler_CounterData_AppendMode appendMode  , 
            std::vector<uint8_t>&                       counterData
        ) 
        {
            return nv::perf::sampler::GpuPeriodicSamplerCreateCounterData(
                deviceIndex                                   ,
                counterConfiguration.counterDataPrefix.data() ,
                counterConfiguration.counterDataPrefix.size() ,
                maxSamples                                    ,
                appendMode                                    ,
                counterData
            );
        }))

        /**
        * @brief Update the metrics evaluator with the actual device's attributes stored in the counter data.
        */
        NSPERF_CHECK(MetricsEvaluatorSetDeviceAttributes(
            metricsEvaluator                    , 
            counterData.GetCounterData().data() , 
            counterData.GetCounterData().size()
        ))

        /**
        * @brief Output the header in CSV format.
        */
        {
            std::cout << "StartTime, EndTime, Duration";
            const auto countersEnumerator    = EnumerateCounters(metricsEvaluator);
            const auto ratiosEnumerator      = EnumerateRatios(metricsEvaluator);
            const auto throughputsEnumerator = EnumerateThroughputs(metricsEvaluator);
            for (const NVPW_MetricEvalRequest& metricEvalRequest : metricEvalRequests)
            {
                std::cout << ", " << ToString(countersEnumerator, ratiosEnumerator, throughputsEnumerator, metricEvalRequest);
            }
            std::cout << "\n";
        }

        /**
        * @brief Start a periodic sampler session.
        */
        const uint32_t SamplingFrequency = 120; // 120 Hz
        const uint32_t samplingIntervalInNanoSeconds = 1000 * 1000 * 1000 / SamplingFrequency;
        const uint32_t MaxDecodeLatencyInNanoSeconds = 1000 * 1000 * 1000 * 10; // tolerate maximum DecodeCounters() latency up to 1 second
        const nv::perf::sampler::GpuPeriodicSampler::GpuPulseSamplingInterval samplingInterval = sampler.GetGpuPulseSamplingInterval(samplingIntervalInNanoSeconds);
        const uint32_t maxNumUndecodedSamples = MaxDecodeLatencyInNanoSeconds / samplingIntervalInNanoSeconds;
        size_t recordBufferSize = 0;
        NSPERF_CHECK(nv::perf::sampler::GpuPeriodicSamplerCalculateRecordBufferSize(deviceIndex, counterConfiguration.configImage, maxNumUndecodedSamples, recordBufferSize))

        const size_t MaxNumUndecodedSamplingRanges = 1; // must be 1
        NSPERF_CHECK(sampler.BeginSession(
            recordBufferSize                   ,
            MaxNumUndecodedSamplingRanges      ,
            { samplingInterval.triggerSource } ,
            samplingInterval.samplingInterval
        ))

        /**
        * @brief Apply the previously generated counter configuration to the periodic sampler.
        */
        const size_t passIndex = 0; // This is a single-pass configuration, so the pass index is fixed at 0.
        NSPERF_CHECK(sampler.SetConfig(counterConfiguration.configImage, passIndex))

        /**
        * @brief Start sampling.
        * Ideally, sampling should only start right before executing the target workloads to prevent the record buffer from being occupied by records generated by GPU triggers before the target workloads.
        * However, in this use case, it is acceptable because the trigger source is set to "NVPW_GPU_PERIODIC_SAMPLER_TRIGGER_SOURCE_GPU_ENGINE_TRIGGER", which doesn't automatically generate GPU triggers but
        * relies on clients manually pushing triggers through the command list. Furthermore, since the metric configuration used is for low-speed sampling, no "overflow prevention records" will be emitted.
        */
        NSPERF_CHECK(sampler.StartSampling())

        /**
        * @brief Set InSession true.
        */
        m_IsInSession = true;
    }

    void NsightPerfGPUProfilerContinuous::BeginFrame(VulkanState& state)
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Capture One frame.
        */
        if (m_EnableCaptureNextFrame)
        {
            Create(state);
            m_EnableCaptureNextFrame = false;
        }
    }

    void NsightPerfGPUProfilerContinuous::EndFrame()
    {
        SPICES_PROFILE_ZONE;

        if (!m_IsInSession) return;

        nv::perf::sampler::GpuPeriodicSampler::GetRecordBufferStatusParams getRecordBufferStatusParams = {};
        getRecordBufferStatusParams.queryOverflow       = true;
        getRecordBufferStatusParams.queryNumUnreadBytes = true;
        bool success = sampler.GetRecordBufferStatus(getRecordBufferStatusParams);
        if (!success)
        {
            return;
        }
        if (getRecordBufferStatusParams.overflow)
        {
            SPICES_CORE_ERROR("Record buffer has overflowed. Please ensure that the value of `maxNumUndecodedSamples` is sufficiently large.");
        }

        if (getRecordBufferStatusParams.numUnreadBytes == 0)
        {
            return;
        }

        /**
        * @brief Decode the record buffer and store the decoded counters into the counter data.
        */
        NVPW_GPU_PeriodicSampler_DecodeStopReason decodeStopReason = NVPW_GPU_PERIODIC_SAMPLER_DECODE_STOP_REASON_OTHER;
        size_t numSamplesMerged = 0;
        size_t numBytesConsumed = 0;
        if (!sampler.DecodeCounters(
            counterData.GetCounterData()               ,
            getRecordBufferStatusParams.numUnreadBytes ,
            decodeStopReason                           ,
            numSamplesMerged                           ,
            numBytesConsumed
        ))
        {
            SPICES_CORE_WARN("Failed to decode counters.");
        }

        if (numSamplesMerged)
        {
            SPICES_CORE_WARN("Samples appear to be merged, this can reduce the accuracy of the collected samples. Please check for any back-to-back triggers!");
        }
        if (decodeStopReason != NVPW_GPU_PERIODIC_SAMPLER_DECODE_STOP_REASON_ALL_GIVEN_BYTES_READ)
        {
            SPICES_CORE_WARN("DecodeCounters stopped unexpectedly.");
        }
        if (!sampler.AcknowledgeRecordBuffer(numBytesConsumed))
        {
            SPICES_CORE_WARN("Failed to acknowledge record buffer");
        }
        if (!counterData.UpdatePut())
        {
            SPICES_CORE_WARN("Failed to update counter data's put pointer.");
        }

        const uint32_t numUnreadRanges = counterData.GetNumUnreadRanges();
        if (numUnreadRanges)
        {
            std::vector<double> metricValues(metricEvalRequests.size());
            uint32_t numRangesConsumed = 0;
            NSPERF_CHECK(counterData.ConsumeData(
            [&](
                const uint8_t* pCounterDataImage    , 
                size_t         counterDataImageSize , 
                uint32_t       rangeIndex           , 
                bool&          stop
            ) 
            {
                nv::perf::sampler::SampleTimestamp timestamp{};
                if (!CounterDataGetSampleTime(pCounterDataImage, rangeIndex, timestamp))
                {
                    return false;
                }

                if (!nv::perf::EvaluateToGpuValues(
                    metricsEvaluator,
                    pCounterDataImage,
                    counterDataImageSize,
                    rangeIndex,
                    metricEvalRequests.size(),
                    metricEvalRequests.data(),
                    metricValues.data()))
                {
                    return false;
                }
                {
                    std::cout << std::fixed << std::setprecision(0) << timestamp.start << ", " << timestamp.end << ", " << (timestamp.end - timestamp.start);
                    for (double metricValue : metricValues)
                    {
                        std::cout << ", " << metricValue;
                    }
                    std::cout << "\n";
                }
                if (++numRangesConsumed == numUnreadRanges)
                {
                    /**
                    * @brief Inform counter data to stop iterating because all existing data has been consumed.
                    */
                    stop = true;
                }
                return true;
            }))

            std::cout << std::flush;
            if (!counterData.UpdateGet(numRangesConsumed))
            {
                SPICES_CORE_WARN("Counter data failed to update get pointer.");
            }
        }

        /**
        * @brief Reset after out.
        */
        Reset();
    }

    void NsightPerfGPUProfilerContinuous::CaptureFrame()
    {
        SPICES_PROFILE_ZONE;

        m_EnableCaptureNextFrame = true;
    }

    void NsightPerfGPUProfilerContinuous::Reset()
    {
        SPICES_PROFILE_ZONE;

        NSPERF_CHECK(sampler.StopSampling())
        NSPERF_CHECK(sampler.EndSession())
        sampler.Reset();

        m_IsInSession = false;
    }
}