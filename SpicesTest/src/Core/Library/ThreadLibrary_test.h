/**
* @file ThreadLibrary_test.h.
* @brief The ThreadLibrary_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Library/ThreadLibrary.h>
#include "Instrumentor.h"

namespace SpicesTest {

    /**
    * @brief Testing Spices::ThreadLibrary::SetThreadName.
    */
    TEST(ThreadLibraryTest, SetThreadName) {

        SPICESTEST_PROFILE_FUNCTION();

        std::thread t([]() {
            Spices::ThreadLibrary::SetThreadName("Hello");
        });
        
        t.join();
    }

}