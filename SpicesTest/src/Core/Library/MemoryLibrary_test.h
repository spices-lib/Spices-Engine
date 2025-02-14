/**
* @file MemoryLibrary_test.h.
* @brief The MemoryLibrary_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Library/MemoryLibrary.h>
#include "Instrumentor.h"

namespace SpicesTest {

    /**
    * @brief Testing Spices::MemoryLibrary::is_aligned.
    */
    TEST(MemoryLibraryTest, is_aligned) {

        SPICESTEST_PROFILE_FUNCTION();

        EXPECT_EQ(Spices::MemoryLibrary::is_aligned(1, 2), false);
        EXPECT_EQ(Spices::MemoryLibrary::is_aligned(2, 4), false);
        EXPECT_EQ(Spices::MemoryLibrary::is_aligned(3, 6), false);

        EXPECT_EQ(Spices::MemoryLibrary::is_aligned(4, 4), true);
        EXPECT_EQ(Spices::MemoryLibrary::is_aligned(8, 4), true);
        EXPECT_EQ(Spices::MemoryLibrary::is_aligned(12, 4), true);
    }

    /**
    * @brief Testing Spices::MemoryLibrary::align_up.
    */
    TEST(MemoryLibraryTest, align_up) {

        SPICESTEST_PROFILE_FUNCTION();

        EXPECT_EQ(Spices::MemoryLibrary::align_up(6, 4), 8);
        EXPECT_EQ(Spices::MemoryLibrary::align_up(1025, 1024), 2048);
        EXPECT_EQ(Spices::MemoryLibrary::align_up(6, 5), 10);
    }

    /**
    * @brief Testing Spices::MemoryLibrary::align_down.
    */
    TEST(MemoryLibraryTest, align_down) {

        SPICESTEST_PROFILE_FUNCTION();

        EXPECT_EQ(Spices::MemoryLibrary::align_down(6, 4), 4);
        EXPECT_EQ(Spices::MemoryLibrary::align_down(1025, 1024), 1024);
        EXPECT_EQ(Spices::MemoryLibrary::align_down(127, 128), 0);
    }
}