/**
* @file IsConst_test.h.
* @brief The IsConst_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Reflect/StaticReflect/IsConst.h>
#include "Instrumentor.h"

namespace SpicesTest {

    class IsConstTest{};

    /**
    * @brief Testing Spices::is_const_v.
    */
    TEST(IsConst_test, is_const_v) {

        SPICESTEST_PROFILE_FUNCTION();

        constexpr bool v0  = Spices::is_const_v<int>;
        constexpr bool v1  = Spices::is_const_v<const int>;
        constexpr bool v2  = Spices::is_const_v<const int*>;
        constexpr bool v3  = Spices::is_const_v<const int&>;
        constexpr bool v4  = Spices::is_const_v<int const *>;
        constexpr bool v5  = Spices::is_const_v<int*&>;
        constexpr bool v6  = Spices::is_const_v<const int*&>;
        constexpr bool v7  = Spices::is_const_v<int const * const &>;
        constexpr bool v8  = Spices::is_const_v<const int const * const &>;
        constexpr bool v9  = Spices::is_const_v<const int**>;
        constexpr bool v10 = Spices::is_const_v<const int***>;
        constexpr bool v11 = Spices::is_const_v<const int****>;
        constexpr bool v12 = Spices::is_const_v<const int*&&>;

        constexpr bool v13 = Spices::is_const_v<int           IsConstTest::*>;
        constexpr bool v14 = Spices::is_const_v<const int     IsConstTest::*>;
        constexpr bool v15 = Spices::is_const_v<const int*    IsConstTest::*>;
        constexpr bool v16 = Spices::is_const_v<int const*    IsConstTest::*>;
        constexpr bool v17 = Spices::is_const_v<const int**   IsConstTest::*>;
        constexpr bool v18 = Spices::is_const_v<const int***  IsConstTest::*>;
        constexpr bool v19 = Spices::is_const_v<const int**** IsConstTest::*>;

        EXPECT_EQ(v0,  false);
        EXPECT_EQ(v1,  true);
        EXPECT_EQ(v2,  true);
        EXPECT_EQ(v3,  true);
        EXPECT_EQ(v4,  true);
        EXPECT_EQ(v5,  false);
        EXPECT_EQ(v6,  true);
        EXPECT_EQ(v7,  true);
        EXPECT_EQ(v8,  true);
        EXPECT_EQ(v9,  true);
        EXPECT_EQ(v10, true);
        EXPECT_EQ(v11, true);
        EXPECT_EQ(v12, true);

        EXPECT_EQ(v13, false);
        EXPECT_EQ(v14, true);
        EXPECT_EQ(v15, true);
        EXPECT_EQ(v16, true);
        EXPECT_EQ(v17, true);
        EXPECT_EQ(v18, true);
        EXPECT_EQ(v19, true);
    }

}