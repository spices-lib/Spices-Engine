/**
* @file Vector_test.h.
* @brief The Vector_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Container/Vector.h>
#include "Instrumentor.h"

namespace SpicesTest {

    /**
    * @brief Testing scl::vector::push_back.
    */
    TEST(vector_test, push_back) {

        SPICESTEST_PROFILE_FUNCTION();

        {
            scl::vector<int> v;
            
            for (int i = 0; i < 100000; i++)
            {
                v.push_back(i);

                EXPECT_EQ(v.size(), i + 1);
                EXPECT_EQ(v.empty(), false);
            }
        }

        {
            scl::vector<void*> v;

            void* p = nullptr;

            for (int i = 0; i < 100000; i++)
            {
                v.push_back(p);

                EXPECT_EQ(v.size(), i + 1);
                EXPECT_EQ(v.empty(), false);
            }
        }

        {
            scl::vector<scl::vector<void*>> v;

            void* p = nullptr;

            for (int i = 0; i < 100000; i++)
            {
                scl::vector<void*> t;

                t.push_back(p);
                v.push_back(t);

                EXPECT_EQ(v.size(), i + 1);
                EXPECT_EQ(v.empty(), false);
            }
        }
    }

    /**
    * @brief Testing scl::vector::get.
    */
    TEST(vector_test, get) {

        SPICESTEST_PROFILE_FUNCTION();

        scl::vector<int> v;

        for (int i = 0; i < 100000; i++)
        {
            v.push_back(i);

            EXPECT_EQ(v.size(), i + 1);
            EXPECT_EQ(v.empty(), false);
        }

        for (int i = 0; i < 100000; i++)
        {
            EXPECT_EQ(v.get(i), i);
        }
    }
}