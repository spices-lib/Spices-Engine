/**
* @file InetAddress_test.h.
* @brief The InetAddress_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Network/InetAddress.h>
#include "Instrumentor.h"

namespace SpicesTest {

    /**
    * @brief Testing Spices::Net::InetAddress.
    */
    TEST(InetAddress_test, InetAddress) {

        SPICESTEST_PROFILE_FUNCTION();

        Spices::Net::InetAddress address(8080, "192.168.0.127");

        EXPECT_EQ(address.ToIP(), "192.168.0.127");
        EXPECT_EQ(address.ToIPPort(), "192.168.0.127:8080");
        EXPECT_EQ(address.ToPort(), 8080);
    }

}