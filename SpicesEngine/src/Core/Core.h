/**
* @file Core.h.
* @brief SpicesEngine Core Macro.
* @author Spices.
*/

#pragma once
#include "Debugger/Tracy/TracyProfiler.hpp"
#include "Debugger/Aftermath/NsightAftermathGpuCrashTracker.h"
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
    * @brief Aftermath handle device lost function.
    */
    static void AftermathDeviceLostCheck()
    {
        // Device lost notification is asynchronous to the NVIDIA display
        // driver's GPU crash handling. Give the Nsight Aftermath GPU crash dump
        // thread some time to do its work before terminating the process.
        auto tdrTerminationTimeout = std::chrono::seconds(3);
        auto tStart = std::chrono::steady_clock::now();
        auto tElapsed = std::chrono::milliseconds::zero();

        GFSDK_Aftermath_CrashDump_Status status = GFSDK_Aftermath_CrashDump_Status_Unknown;
        AFTERMATH_CHECK_ERROR(GFSDK_Aftermath_GetCrashDumpStatus(&status));

        while (status != GFSDK_Aftermath_CrashDump_Status_CollectingDataFailed &&
               status != GFSDK_Aftermath_CrashDump_Status_Finished &&
               tElapsed < tdrTerminationTimeout)
        {
            // Sleep 50ms and poll the status again until timeout or Aftermath finished processing the crash dump.
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            AFTERMATH_CHECK_ERROR(GFSDK_Aftermath_GetCrashDumpStatus(&status));

            auto tEnd = std::chrono::steady_clock::now();
            tElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(tEnd - tStart);
        }

        if (status == GFSDK_Aftermath_CrashDump_Status_Finished)
        {
            std::stringstream ss;
            ss << "Aftermath finished processing the crash dump ";
            SPICES_CORE_INFO(ss.str());
        }
        else
        {
            std::stringstream err_msg;
            err_msg << "Unexpected crash dump status: " << status;
            SPICES_CORE_CRITICAL(err_msg.str().c_str(), "Aftermath Error");
        }

        // Terminate on failure
        exit(1);
    }

    /**
    * @brief handle device lost function.
    * @param[in] result VkResult.
    */
    static void DeviceLostCheck(VkResult result)
    {

#ifdef SPICES_DEBUG

        if (result == VK_ERROR_DEVICE_LOST)
        {                                  
            AftermathDeviceLostCheck();    
        }                                  

#endif

    }

/**
* @brief Vulkan Check macro.
* Verify Vulkan API Effectiveness.
*/
#define VK_CHECK(expr)                        \
    {                                         \
        auto value = expr;                    \
        DeviceLostCheck(value);               \
        ASSERT(value == VK_SUCCESS);          \
    }
}