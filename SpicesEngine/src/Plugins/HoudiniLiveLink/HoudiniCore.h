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
#define HE_CHECK(expr)                         \
    {                                          \
        ASSERT(expr == HAPI_RESULT_SUCCESS );  \
    }
    
}