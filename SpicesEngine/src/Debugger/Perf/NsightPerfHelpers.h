/**
* @file NsightPerfHelpers.h
* @brief The NsightPerfHelpers Class Definitions.
* @author Spices
*/

#pragma once
#include "Core/Core.h"

namespace Spices {

#define NSPERF_CHECK(val)                                                                                     \
	{                                                                                                         \
        if (val) {}                                                                                           \
        else                                                                                                  \
        {                                                                                                     \
            std::stringstream ss;                                                                             \
            ss << "Assert Failed \n    At File: " << __FILE__ << " \n   At Line: " << __LINE__ << "\n   ";    \
            SPICES_CORE_WARN(ss.str());                                                                      \
        }                                                                                                     \
    }  
}