/*
* Copyright 2014-2024 NVIDIA Corporation.  All rights reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#pragma once
#include "NvPerfReportGenerator.h"
#include "NvPerfRangeProfilerEGL.h"

namespace nv { namespace perf { namespace profiler {

    class ReportGeneratorEGL
    {
    protected:
        struct ReportProfiler : public ReportGeneratorStateMachine::IReportProfiler
        {
            RangeProfilerEGL rangeProfiler;

            ReportProfiler()
                : rangeProfiler()
            {
            }

            virtual bool IsInSession() const override
            {
                return rangeProfiler.IsInSession();
            }
            virtual bool IsInPass() const override
            {
                return rangeProfiler.IsInPass();
            }
            virtual bool EndSession() override
            {
                return rangeProfiler.EndSession();
            }
            virtual bool EnqueueCounterCollection(const SetConfigParams& config) override
            {
                return rangeProfiler.EnqueueCounterCollection(config);
            }
            virtual bool BeginPass() override
            {
                return rangeProfiler.BeginPass();
            }
            virtual bool EndPass() override
            {
                return rangeProfiler.EndPass();
            }
            virtual bool PushRange(const char* pRangeName) override
            {
                return rangeProfiler.PushRange(pRangeName);
            }
            virtual bool PopRange() override
            {
                return rangeProfiler.PopRange();
            }
            virtual bool DecodeCounters(DecodeResult& decodeResult) override
            {
                return rangeProfiler.DecodeCounters(decodeResult);
            }
            virtual bool AllPassesSubmitted() const override
            {
                return rangeProfiler.AllPassesSubmitted();
            }
        };

    protected:
        ReportProfiler m_reportProfiler;
        ReportGeneratorStateMachine m_stateMachine;

        // EGL device state, set at initialize
        ReportGeneratorInitStatus m_initStatus;  // the state of InitializeReportGenerator()
        SessionOptions m_defaultSessionOptions;

    protected:
        bool BeginSessionWithOptions(
            const SessionOptions* pSessionOptions = nullptr)
        {
            const SessionOptions& sessionOptions = pSessionOptions ? *pSessionOptions : m_defaultSessionOptions;
            if (!m_reportProfiler.rangeProfiler.BeginSession(sessionOptions))
            {
                NV_PERF_LOG_ERR(10, "m_reportProfiler.rangeProfiler.BeginSession failed\n");
                return false;
            }

            return true;
        }

    public:
        size_t deviceIndex;
        DeviceIdentifiers deviceIdentifiers;
        std::vector<std::string> additionalMetrics;
        ReportOutputOptions outputOptions;

    public:
        ~ReportGeneratorEGL()
        {
            Reset();
        }

        ReportGeneratorEGL()
            : m_reportProfiler()
            , m_stateMachine(m_reportProfiler)
            , m_initStatus(ReportGeneratorInitStatus::NeverCalled)
            , m_defaultSessionOptions()
            , deviceIndex()
            , deviceIdentifiers()
            , additionalMetrics()
            , outputOptions()
        {
            m_defaultSessionOptions.maxNumRanges = ReportGeneratorStateMachine::MaxNumRangesDefault;
        }

        ReportGeneratorInitStatus GetInitStatus() const
        {
            return m_initStatus;
        }

        /// Ends all current sessions and frees all internal memory.
        /// This object may be reused by calling InitializeReportGenerator() again.
        /// Does not reset rangeCommands and deviceIdentifiers.
        void Reset()
        {
            if (m_reportProfiler.rangeProfiler.IsInSession())
            {
                const bool endSessionStatus = m_reportProfiler.rangeProfiler.EndSession();
                if (!endSessionStatus)
                {
                    NV_PERF_LOG_ERR(10, "m_reportProfiler.EndSession failed\n");
                }
            }

            m_stateMachine.Reset();

            if (m_initStatus != ReportGeneratorInitStatus::NeverCalled)
            {
                m_initStatus = ReportGeneratorInitStatus::Reset;
            }
        }

        /// Initialize this object on the provided current context.
        bool InitializeReportGenerator()
        {
            m_initStatus = ReportGeneratorInitStatus::Failed;

            // Can this device be profiled by Nsight Perf SDK?
            if (!nv::perf::EGLIsNvidiaDevice())
            {
                NV_PERF_LOG_ERR(10, "%s is not an NVIDIA Device\n", EGLGetDeviceName().c_str());
                return false;
            }

            if (!InitializeNvPerf())
            {
                NV_PERF_LOG_ERR(10, "InitializeNvPerf failed\n");
                return false;
            }

            if (!nv::perf::EGLLoadDriver())
            {
                NV_PERF_LOG_ERR(10, "Could not load driver\n");
                return false;
            }

            if (!nv::perf::profiler::EGLIsGpuSupported())
            {
                NV_PERF_LOG_ERR(10, "GPU is not supported\n");
                return false;
            }

            deviceIndex = EGLGetNvperfDeviceIndex();
            if (deviceIndex == ~size_t(0))
            {
                NV_PERF_LOG_ERR(10, "Unrecognaized device\n");
                return false;
            }

            deviceIdentifiers = GetDeviceIdentifiers(deviceIndex);
            if (!deviceIdentifiers.pChipName)
            {
                NV_PERF_LOG_ERR(10, "Unrecognized GPU\n");
                return false;
            }

            auto createMetricsEvaluator = [&](std::vector<uint8_t>& scratchBuffer) {
                const size_t scratchBufferSize = nv::perf::EGLCalculateMetricsEvaluatorScratchBufferSize(deviceIdentifiers.pChipName);
                if (!scratchBufferSize)
                {
                    return (NVPW_MetricsEvaluator*)nullptr;
                }
                scratchBuffer.resize(scratchBufferSize);
                NVPW_MetricsEvaluator* pMetricsEvaluator = nv::perf::EGLCreateMetricsEvaluator(scratchBuffer.data(), scratchBuffer.size(), deviceIdentifiers.pChipName);
                return pMetricsEvaluator;
            };
            auto createRawMetricsConfig = [&]() {
                NVPA_RawMetricsConfig* pRawMetricsConfig = nv::perf::profiler::EGLCreateRawMetricsConfig(deviceIdentifiers.pChipName);
                return pRawMetricsConfig;
            };
            if (!m_stateMachine.InitializeReportMetrics(deviceIndex, deviceIdentifiers, createMetricsEvaluator, createRawMetricsConfig, additionalMetrics))
            {
                NV_PERF_LOG_ERR(10, "m_stateMachine.InitializeReportMetrics failed\n");
                return false;
            }

            m_initStatus = ReportGeneratorInitStatus::Succeeded;

            NV_PERF_LOG_INF(50, "Initialization succeeded\n");

            return true;
        }

        /// Explicitly starts a session.  This allows you to control resource allocation.
        /// Calling this function is optional; by default, OnFrameStart() will start a session if this isn't called.
        /// The session must be explicitly ended by calling Reset().
        bool BeginSession(const SessionOptions* pSessionOptions = nullptr)
        {
            if (m_initStatus != ReportGeneratorInitStatus::Succeeded)
            {
                NV_PERF_LOG_WRN(100, "skipping; the state of InitializeReportGenerator() is %s.\n", ToCString(m_initStatus));
                return false;
            }

            auto beginSessionFn = [&]() {
                return BeginSessionWithOptions(pSessionOptions);
            };
            if (!m_stateMachine.BeginSession(beginSessionFn))
            {
                return false;
            }
            return true;
        }

        /// Automatically starts collecting counters for a report, after StartCollectionOnNextFrame().
        /// Call this at the start of each frame.
        bool OnFrameStart()
        {
            if (m_initStatus != ReportGeneratorInitStatus::Succeeded)
            {
                NV_PERF_LOG_WRN(100, "skipping; the state of InitializeReportGenerator() is %s.\n", ToCString(m_initStatus));
                return false;
            }

            auto beginSessionFn = [&]() {
                return BeginSessionWithOptions();
            };
            if (!m_stateMachine.OnFrameStart(beginSessionFn))
            {
                return false;
            }
            return true;
        }

        /// Advances the counter-collection state-machine after rendering.
        /// Call this at the end of each frame.
        bool OnFrameEnd()
        {
            if (m_initStatus != ReportGeneratorInitStatus::Succeeded)
            {
                NV_PERF_LOG_WRN(100, "skipping; the state of InitializeReportGenerator() is %s.\n", ToCString(m_initStatus));
                return false;
            }

            if (!m_stateMachine.OnFrameEnd(outputOptions))
            {
                return false;
            }
            return true;
        }

        bool PushRange(const char* pRangeName)
        {
            if (m_initStatus != ReportGeneratorInitStatus::Succeeded)
            {
                NV_PERF_LOG_WRN(100, "skipping; the state of InitializeReportGenerator() is %s.\n", ToCString(m_initStatus));
                return false;
            }
            if (!m_reportProfiler.IsInPass())
            {
                NV_PERF_LOG_WRN(100, "skipping; not in a profiler pass");
                return false;
            }
            if (!m_reportProfiler.PushRange(pRangeName))
            {
                return false;
            }
            return true;
        }

        bool PopRange()
        {
            if (m_initStatus != ReportGeneratorInitStatus::Succeeded)
            {
                NV_PERF_LOG_WRN(100, "skipping; the state of InitializeReportGenerator() is %s.\n", ToCString(m_initStatus));
                return false;
            }
            if (!m_reportProfiler.IsInPass())
            {
                NV_PERF_LOG_WRN(100, "skipping; not in a profiler pass");
                return false;
            }
            if (!m_reportProfiler.PopRange())
            {
                return false;
            }
            return true;
        }

        /// Reports true after StartCollectionOnNextFrame() is called, until the HTML Report has been written to disk.
        /// This state is cleared by OnFrameEnd().
        bool IsCollectingReport() const
        {
            return m_stateMachine.IsCollectingReport();
        }

        const std::string& GetLastReportDirectoryName() const
        {
            return m_stateMachine.GetLastReportDirectoryName();
        }

        /// Enqueues report collection, starting on the next frame.
        bool StartCollectionOnNextFrame()
        {
            if (m_initStatus != ReportGeneratorInitStatus::Succeeded)
            {
                NV_PERF_LOG_WRN(100, "skipping; the state of InitializeReportGenerator() is %s.\n", ToCString(m_initStatus));
                return false;
            }
            return m_stateMachine.StartCollectionOnNextFrame();
        }

        /// Enables a frame-level parent range.
        /// When enabled (non-NULL, non-empty pRangeName), every frame will have a parent range.
        /// This is also convenient for programs that have no CommandList-level ranges.
        /// Pass in NULL or an empty string to disable this behavior.
        /// The pRangeName string is copied by value, and may be modified or freed after this function returns.
        void SetFrameLevelRangeName(const char* pRangeName)
        {
            m_stateMachine.SetFrameLevelRangeName(pRangeName);
        }

        /// Retrieves the current frame-level parent range.  An empty string signifies no parent range.
        const std::string& GetFrameLevelRangeName() const
        {
            return m_stateMachine.GetFrameLevelRangeName();
        }

        /// Sets the number of Push/Pop nesting levels to collect in the report.
        void SetNumNestingLevels(uint16_t numNestingLevels)
        {
            m_stateMachine.SetNumNestingLevels(numNestingLevels);
        }

        /// Retrieves the number of Push/Pop nesting levels being collected in the report.
        uint16_t GetNumNestingLevels() const
        {
            return m_stateMachine.GetNumNestingLevels();
        }

        /// Sets the maximum number of ranges to collect in the report.
        void SetMaxNumRanges(size_t maxNumRanges)
        {
            m_defaultSessionOptions.maxNumRanges = maxNumRanges;
        }

        /// Retrieves the maximum number of ranges to collect in the report.
        size_t GetMaxNumRanges() const
        {
            return m_defaultSessionOptions.maxNumRanges;
        }

        /// Open the report directory in file browser after perf data collection.
        /// The default behavor is false, and can be changed by enviroment variable NV_PERF_OPEN_REPORT_DIR_AFTER_COLLECTION.
        void SetOpenReportDirectoryAfterCollection(bool openReportDirectoryAfterCollection)
        {
            m_stateMachine.SetOpenReportDirectoryAfterCollection(openReportDirectoryAfterCollection);
        }
    };
}}}
