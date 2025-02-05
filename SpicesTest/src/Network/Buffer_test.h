/**
* @file Buffer_test.h.
* @brief The Buffer_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Network/Buffer.h>
#include "Instrumentor.h"

namespace SpicesTest {

    /**
    * @brief Testing Spices::Net::Buffer.
    */
    TEST(Buffer_test, Buffer) {

        SPICESTEST_PROFILE_FUNCTION();

        Spices::Net::Buffer buffer;

        EXPECT_EQ(buffer.ReadableBytes(), 0);
        EXPECT_EQ(buffer.WriteableBytes(), 1024);
        EXPECT_EQ(buffer.HeaderBytes(), 8);

    }

}