/**
* @file HoudiniCore.h.
* @brief SpicesEngine HoudiniCore Macro.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "HAPI/HAPI.h"

namespace HoudiniEngine {

/**
* @brief HoudiniEngine Check macro.
* Verify HoudiniEngine API Effectiveness.
*/
#define HE_CHECK(expr)                                                                     \
    if((expr) != HAPI_RESULT_SUCCESS)                                                      \
    {                                                                                      \
        std::stringstream ss;                                                              \
        ss << "Houdini Call Failed:\n        Error Code:  "                                \
        << GetHoudiniLastError()                                                           \
        << "\n        At Lines: " << __LINE__                                              \
        << "\n        At Files: " << __FILE__;                                             \
        SPICES_CORE_WARN(ss.str());                                                        \
    }

/**
* @brief HoudiniEngine Check Cook macro.
* Verify HoudiniEngine Cook API Effectiveness.
*/
#define HE_CHECK_COOK(expr)                                                                \
    if((expr) != HAPI_RESULT_SUCCESS)                                                      \
    {                                                                                      \
        std::stringstream ss;                                                              \
        ss << "Houdini Call Failed:\n        Error Code:  "                                \
        << GetHoudiniLastCookError()                                                       \
        << "\n        At Lines: " << __LINE__                                              \
        << "\n        At Files: " << __FILE__;                                             \
        SPICES_CORE_WARN(ss.str());                                                        \
    }
    
    static std::string GetHoudiniLastError()
    {
        int bufferLength;
        HAPI_GetStatusStringBufLength(nullptr, HAPI_STATUS_CALL_RESULT, HAPI_STATUSVERBOSITY_ERRORS, &bufferLength);

        char* buffer = new char[bufferLength];
        HAPI_GetStatusString(nullptr, HAPI_STATUS_CALL_RESULT, buffer, bufferLength);

        std::string result(buffer);
        delete[] buffer;

        return result;
    }

    static std::string GetHoudiniLastCookError()
    {
        int bufferLength;
        HAPI_GetStatusStringBufLength(nullptr, HAPI_STATUS_COOK_RESULT, HAPI_STATUSVERBOSITY_ERRORS, &bufferLength);

        char* buffer = new char[bufferLength];
        HAPI_GetStatusString(nullptr, HAPI_STATUS_COOK_RESULT, buffer, bufferLength);

        std::string result(buffer);
        delete[] buffer;

        return result;
    }

}