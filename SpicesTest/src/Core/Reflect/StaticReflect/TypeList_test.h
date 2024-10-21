/**
* @file TypeList_test.h.
* @brief The TypeList_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Reflect/StaticReflect/TypeList.hpp>
#include "Instrumentor.h"

namespace SpicesTest {

    template<typename T>
    struct is_intergral
    {
        static constexpr bool value = std::is_integral_v<T>;
    };

    template<typename T>
    struct change_to_float
    {
        using type = std::conditional_t<std::is_integral_v<T>, float, T>;
    };

    /**
    * @brief Testing Spices::TypeList.
    */
    TEST(TypeListTest, TypeList) {

        SPICESTEST_PROFILE_FUNCTION();

        using TypeList = Spices::type_list<int, float, std::string, void*, void(*)(int)>;

        {
            constexpr bool v = std::is_same_v<Spices::head<TypeList>, int>;

            EXPECT_EQ(v, true);
        }

        {
            constexpr bool v = std::is_same_v<Spices::tail<TypeList>, Spices::type_list<float, std::string, void*, void(*)(int)>>;

            EXPECT_EQ(v, true);
        }

        {
            constexpr bool v0 = std::is_same_v<Spices::nth<TypeList, 0>, int>;
            constexpr bool v1 = std::is_same_v<Spices::nth<TypeList, 1>, float>;
            constexpr bool v2 = std::is_same_v<Spices::nth<TypeList, 2>, std::string>;
            constexpr bool v3 = std::is_same_v<Spices::nth<TypeList, 3>, void*>;
            constexpr bool v4 = std::is_same_v<Spices::nth<TypeList, 4>, void(*)(int)>;

            EXPECT_EQ(v0, true);
            EXPECT_EQ(v1, true);
            EXPECT_EQ(v2, true);
            EXPECT_EQ(v3, true);
            EXPECT_EQ(v4, true);
        }

        {
            constexpr int v = Spices::count<TypeList, is_intergral>;

            EXPECT_EQ(v, 1);
        }

        {
            constexpr bool v = std::is_same_v<Spices::map<TypeList, change_to_float>, Spices::type_list<float, float, float, float, float>>;

            EXPECT_EQ(v, true);
        }

        {
            constexpr bool v = std::is_same_v<Spices::cons<char, TypeList>, Spices::type_list<char, int, float, std::string, void*, void(*)(int)>>;

            EXPECT_EQ(v, true);
        }
    }
}