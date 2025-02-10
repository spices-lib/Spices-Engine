/**
* @file Semaphore_test.h.
* @brief The Semaphore_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Thread/Semaphore.h>
#include "Instrumentor.h"

namespace SpicesTest {

    /**
    * @brief Testing Spices::Semaphore::Sign.
    */
    TEST(Semaphore_test, Sign) {

        SPICESTEST_PROFILE_FUNCTION();

        Spices::semaphore sem;

        std::thread t([&]() {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            sem.Sign(1);
        });

        // Should not block here.
        EXPECT_EQ(sem.GetSign(), 0);

        t.join();

        // Should block here.
        EXPECT_EQ(sem.GetSign(), 1);
    }

    /**
    * @brief Testing Spices::Semaphore::Wait.
    */
    TEST(Semaphore_test, Wait) {

        SPICESTEST_PROFILE_FUNCTION();

        Spices::semaphore sem;

        const auto t0 = std::chrono::high_resolution_clock::now();

        std::thread t([&]() {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            sem.Sign(1);
        });

        t.detach();

        // Should not block here.
        {
            sem.Wait(0);

            const auto time = std::chrono::high_resolution_clock::now();
            const float period = std::chrono::duration<float, std::chrono::milliseconds::period>(time - t0).count();

            EXPECT_LE(period, 50);
        }

        // Should block here.
        {
            sem.Wait(1);

            const auto time = std::chrono::high_resolution_clock::now();
            const float period = std::chrono::duration<float, std::chrono::milliseconds::period>(time - t0).count();

            EXPECT_LE(period, 1050);
            EXPECT_GE(period, 950);
        }
    }

}