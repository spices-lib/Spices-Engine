/**
* @file ScopeTimer_test.h.
* @brief The ScopeTimer_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Timer/ScopeTimer.h>
#include "Instrumentor.h"

namespace SpicesTest {

    /**
    * @brief Testing Spices::ScopeTimer.
    */
    TEST(ScopeTimer_test, ScopeTimer) {

        SPICESTEST_PROFILE_FUNCTION();

        {
            SCOPE_TIME_COUNTER("Cost should be 1 ms.");

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        {
            SCOPE_TIME_COUNTER("Cost should be 100 ms.");

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        {
            SCOPE_TIME_COUNTER("Cost should be 1000 ms.");

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }

}