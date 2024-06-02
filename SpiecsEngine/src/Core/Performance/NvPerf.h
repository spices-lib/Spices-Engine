#pragma once
#include "Core/Core.h"

#define NV_PERF_ENABLE_INSTRUMENTATION
#ifdef NV_PERF_ENABLE_INSTRUMENTATION
#include <windows-desktop-x64/nvperf_host_impl.h>
//#include <NvPerfHudDataModel.h>
//#include <NvPerfHudImPlotRenderer.h>
//#include <NvPerfPeriodicSamplerVulkan.h>
#include <implot.h>

// Note:
//   Do this in exactly one source file to add rapidyaml's symbols.
//   If Windows.h is included before ryml_all.hpp, it needs to be included with NOMINMAX defined.
//   Otherwise min/max-related errors occur.
//#define RYML_SINGLE_HDR_DEFINE_NOW
//#include <ryml_all.hpp>
#endif

namespace Spiecs {


}