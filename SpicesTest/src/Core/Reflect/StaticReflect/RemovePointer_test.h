/**
* @file RemovePointer_test.h.
* @brief The RemovePointer_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Reflect/StaticReflect/RemovePointer.h>
#include "Instrumentor.h"

namespace SpicesTest {

    class RemovePointerTest {};

    /**
    * @brief Testing Spices::remove_pointer_t.
    */
    TEST(RemovePointer_test, remove_pointer_t) {

        SPICESTEST_PROFILE_FUNCTION();

        constexpr bool v0 = std::is_same_v<Spices::remove_pointer_t<int>,     int>;
        constexpr bool v1 = std::is_same_v<Spices::remove_pointer_t<int*>,    int>;
        constexpr bool v2 = std::is_same_v<Spices::remove_pointer_t<int**>,   int>;
        constexpr bool v3 = std::is_same_v<Spices::remove_pointer_t<int***>,  int>;
        constexpr bool v4 = std::is_same_v<Spices::remove_pointer_t<int****>, int>;

        constexpr bool v5 = std::is_same_v<Spices::remove_pointer_t<int     RemovePointerTest::*>, int>;
        constexpr bool v6 = std::is_same_v<Spices::remove_pointer_t<int*    RemovePointerTest::*>, int>;
        constexpr bool v7 = std::is_same_v<Spices::remove_pointer_t<int**   RemovePointerTest::*>, int>;
        constexpr bool v8 = std::is_same_v<Spices::remove_pointer_t<int***  RemovePointerTest::*>, int>;
        constexpr bool v9 = std::is_same_v<Spices::remove_pointer_t<int**** RemovePointerTest::*>, int>;

        EXPECT_EQ(v0, true);
        EXPECT_EQ(v1, true);
        EXPECT_EQ(v2, true);
        EXPECT_EQ(v3, true);
        EXPECT_EQ(v4, true);

        EXPECT_EQ(v5, false);
        EXPECT_EQ(v6, true);
        EXPECT_EQ(v7, true);
        EXPECT_EQ(v8, true);
        EXPECT_EQ(v9, true);
    }

}