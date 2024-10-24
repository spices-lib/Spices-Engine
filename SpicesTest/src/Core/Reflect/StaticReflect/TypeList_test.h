/**
* @file TypeList_test.h.
* @brief The TypeList_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Reflect/StaticReflect/TypeList.h>
#include "Instrumentor.h"

namespace SpicesTest {

    template<typename T>
    struct is_intergral
    {
        static constexpr bool value = std::is_integral_v<T>;
    };

    template<typename T>
    struct change_int_to_float
    {
        using type = std::conditional_t<std::is_integral_v<T>, float, T>;
    };

    template<typename T>
    struct is_not_float
    {
        static constexpr bool value = !std::is_same_v<T, float>;
    };

    /**
    * @brief Testing Spices::TypeList.
    */
    TEST(TypeListTest_test, TypeList) {

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
            constexpr bool v = std::is_same_v<Spices::map<TypeList, change_int_to_float>, Spices::type_list<float, float, std::string, void*, void(*)(int)>>;

            EXPECT_EQ(v, true);
        }

        {
            constexpr bool v = std::is_same_v<Spices::cons<char, TypeList>, Spices::type_list<char, int, float, std::string, void*, void(*)(int)>>;

            EXPECT_EQ(v, true);
        }

        {
            using TypeList1 = Spices::type_list<char*, float>;
            constexpr bool v = std::is_same_v<Spices::concat<TypeList, TypeList1>, Spices::type_list<int, float, std::string, void*, void(*)(int), char*, float>>;

            EXPECT_EQ(v, true);
        }

        {
            constexpr bool v = std::is_same_v<Spices::init<TypeList>, Spices::type_list<int, float, std::string, void*>>;

            EXPECT_EQ(v, true);
        }

        {
            constexpr bool v = std::is_same_v<Spices::filter<TypeList, is_not_float>, Spices::type_list<int, std::string, void*, void(*)(int)>>;

            EXPECT_EQ(v, true);
        }
    }

    /**
    * @brief Testing Spices::size.
    */
    TEST(TypeListTest_test, size) {

        SPICESTEST_PROFILE_FUNCTION();

        using TypeList = Spices::type_list<int, float, std::string, void*, void(*)(int)>;

        EXPECT_EQ(TypeList::size, 5);
    }
}