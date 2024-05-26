/**
* @file Core.h.
* @brief SpiecsEngine Core Macro.
* @author Spiecs.
*/

#pragma once
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