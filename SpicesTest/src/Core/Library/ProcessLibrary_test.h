/**
* @file ProcessLibrary_test.h.
* @brief The ProcessLibrary_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Library/ProcessLibrary.h>
#include "Instrumentor.h"

namespace SpicesTest {

    /**
    * @brief Testing Spices::ProcessLibrary::Open/CloseProcess.
    */
    TEST(ProcessLibraryTest, OpenCloseProcess) {

        SPICESTEST_PROFILE_FUNCTION();

        EXPECT_EQ(Spices::ProcessLibrary::OpenProcess("../vendor/tracy/tracy-profiler.exe"), true);
        EXPECT_EQ(Spices::ProcessLibrary::CloseProcess("tracy-profiler.exe"), true);
    }

    /**
    * @brief Testing Spices::ProcessLibrary::ProcessMemoryInUsed.
    */
    TEST(ProcessLibraryTest, ProcessMemoryInUsed) {

        SPICESTEST_PROFILE_FUNCTION();

        float memory = Spices::ProcessLibrary::ProcessMemoryInUsed();
        std::cout << "This Progress used memory: " << memory << " GB";
    }
}