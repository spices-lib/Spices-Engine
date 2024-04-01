#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Spiecs {

#define GET_NAME(cls) ::std::string(#cls) 
#define SPIECS_LOG(...) ::std::cout << __VA_ARGS__ << std::endl

#define ASSERT(expr)                                                 \
    {                                                                \
        if (expr) {                                                  \
        } else {                                                     \
            throw std::runtime_error("ERROR");                       \
            __debugbreak();                                          \
        }                                                            \
    }

#define VK_CHECK(expr)               \
    {                                \
        ASSERT(expr == VK_SUCCESS); \
    }

}