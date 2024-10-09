/**
* @file Core.h.
* @brief SpicesEngine Core Macro.
* @author Spices.
*/

#pragma once
#include "Debugger/Tracy/TracyProfiler.hpp"

#include "TimeStep.h"
#include "Log/Log.h"

/**
* @brief GLFW macro.
* This means we want use vulkan in glfw.
* glfw will include vulkan headers automatically for us.
*/
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <glfw/glfw3native.h>

namespace Spices {

/**
* @brief Assert macro.
* @todo better Assert System.
*/
#define ASSERT(expr)                                                                                          \
    {                                                                                                         \
        if (expr) {}                                                                                          \
        else                                                                                                  \
        {                                                                                                     \
            std::stringstream ss;                                                                             \
            ss << "Assert Failed \n    At File: " << __FILE__ << " \n   At Line: " << __LINE__ << "\n   ";    \
            SPICES_CORE_ERROR(ss.str());                                                                      \
        }                                                                                                     \
    }                                                                                                         \

/**
* @brief Vulkan Check macro.
* Verify Vulkan API Effectiveness.
*/
#define VK_CHECK(expr)                                              \
    {                                                               \
        auto value = expr;                                          \
        NSIGHTAFTERMATH_GPUCRASHTRACKER_DEVICELOSECHECK(value);     \
        ASSERT(value == VK_SUCCESS);                                \
    }
}