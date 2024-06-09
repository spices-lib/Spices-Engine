/**
* @file Core.h.
* @brief SpiecsEngine Core Macro.
* @author Spiecs.
*/

#pragma once
#include "Core/Performance/Profiler.h"
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

namespace Spiecs {

/**
* @brief Assert macro.
* @todo better Assert System.
*/
#define ASSERT(expr)                                                                                          \
    {                                                                                                         \
        if (expr) {                                                                                           \
        } else {                                                                                              \
            std::stringstream ss;                                                                             \
            ss << "Assert Failed \n    At File: " << __FILE__ << " \n   At Line: " << __LINE__ << "\n   ";    \
            SPIECS_CORE_ERROR(ss.str());                                                                      \
        }                                                                                                     \
    }                                                                                                         \

/**
* @brief Vulkan Check macro.
* Verify Vulkan API Effectiveness.
*/
#define VK_CHECK(expr)               \
    {                                \
        ASSERT(expr == VK_SUCCESS); \
    }
}