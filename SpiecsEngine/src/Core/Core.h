/**
* @file Core.h.
* @brief SpiecsEngine Core Macro.
* @author Spiecs.
*/

#pragma once
#include "TimeStep.h"
#include "Log.h"

/**
* @brief GLFW macro.
* This means we want use vulkan in glfw.
* glfw will include vulkan headers automatically for us.
*/
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Spiecs {

/**
* @brief Assert macro.
* @todo better Assert System.
*/
#define ASSERT(expr)                                                 \
    {                                                                \
        if (expr) {                                                  \
        } else {                                                     \
            throw std::runtime_error("ERROR");                       \
            __debugbreak();                                          \
        }                                                            \
    }

/**
* @brief Vulkan Check macro.
* Verify Vulkan API Effectiveness.
*/
#define VK_CHECK(expr)               \
    {                                \
        ASSERT(expr == VK_SUCCESS); \
    }

}