/**
* @file Tuple_test.h.
* @brief The Tuple_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Container/Tuple.h>
#include "Instrumentor.h"

namespace Spices {

    /**
    * @brief Testing Spices::Tuple Helper Function.
    */
    TEST(TupleTest, IterTuple) {

        SPICESTEST_PROFILE_FUNCTION();

        auto t = std::make_tuple(1, 2.0f, "Hellp", nullptr);

        int index = 0;
        
        Spices::IterTuple(t, [&](auto&& elem) {

            if (index == 0)
            {
                EXPECT_EQ((void*)&elem, (void*)&std::get<0>(t));
            }
            else if (index == 1)
            {
                EXPECT_EQ((void*)&elem, (void*)&std::get<1>(t));
            }
            else if (index == 2)
            {
                EXPECT_EQ((void*)&elem, (void*)&std::get<2>(t));
            }
            else if (index == 3)
            {
                EXPECT_EQ((void*)&elem, (void*)&std::get<3>(t));
            }

            index++;
        });
    }
}