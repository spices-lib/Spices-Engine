/**
* @file VariableTraits_test.h.
* @brief The VariableTraits_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Reflect/StaticReflect/VariableTraits.hpp>
#include "Instrumentor.h"

namespace SpicesTest {

    /**
    * @brief Member Variable
    */
    class VariableTraitsTestClass
    {
    public:
        const int      a;
        uint32_t       b;
        static float   c;
        std::string    d;
        void*          e;
    };

    /**
    * @brief Non Member Variable
    */
    namespace VariableTraitsTestN
    {
        const int      a = 0;
        uint32_t       b;
        static float   c;
        std::string    d;
        void*          e;
    }

    /**
    * @brief Testing Spices::VariableTraits.
    */
    TEST(VariableTraitsTest, VariableTraits) {

        SPICESTEST_PROFILE_FUNCTION();

        {
            using ca = Spices::variable_traits<decltype(&VariableTraitsTestClass::a)>;
            using  a = Spices::variable_traits<decltype(&VariableTraitsTestN::a)>;

            constexpr bool v0 = std::is_same_v<ca::Type, const int>;
            constexpr bool v1 = std::is_same_v<a::Type, const int>;
            constexpr bool v2 = std::is_same_v<ca::Class, VariableTraitsTestClass>;
            constexpr bool v3 = std::is_same_v<a::Class, void>;
            constexpr bool v4 = std::is_same_v<ca::PointerType, const int VariableTraitsTestClass::*>;
            constexpr bool v5 = std::is_same_v<a::PointerType, const int*>;

            EXPECT_EQ(v0, true);
            EXPECT_EQ(v1, true);
            EXPECT_EQ(v2, true);
            EXPECT_EQ(v3, true);
            EXPECT_EQ(v4, true);
            EXPECT_EQ(v5, true);

            EXPECT_EQ(ca::is_member, true);
            EXPECT_EQ(a::is_member, false);
        }
        {
            using cb = Spices::variable_traits<decltype(&VariableTraitsTestClass::b)>;
            using  b = Spices::variable_traits<decltype(&VariableTraitsTestN::b)>;

            constexpr bool v0 = std::is_same_v<cb::Type, uint32_t>;
            constexpr bool v1 = std::is_same_v<b::Type, uint32_t>;
            constexpr bool v2 = std::is_same_v<cb::Class, VariableTraitsTestClass>;
            constexpr bool v3 = std::is_same_v<b::Class, void>;
            constexpr bool v4 = std::is_same_v<cb::PointerType, uint32_t VariableTraitsTestClass::*>;
            constexpr bool v5 = std::is_same_v<b::PointerType, uint32_t*>;

            EXPECT_EQ(v0, true);
            EXPECT_EQ(v1, true);
            EXPECT_EQ(v2, true);
            EXPECT_EQ(v3, true);
            EXPECT_EQ(v4, true);
            EXPECT_EQ(v5, true);

            EXPECT_EQ(cb::is_member, true);
            EXPECT_EQ(b::is_member, false);
        }
        {
            using cc = Spices::variable_traits<decltype(&VariableTraitsTestClass::c)>;
            using  c = Spices::variable_traits<decltype(&VariableTraitsTestN::c)>;

            constexpr bool v0 = std::is_same_v<cc::Type, float>;
            constexpr bool v1 = std::is_same_v<c::Type, float>;
            constexpr bool v2 = std::is_same_v<cc::Class, void>;
            constexpr bool v3 = std::is_same_v<c::Class, void>;
            constexpr bool v4 = std::is_same_v<cc::PointerType, float*>;
            constexpr bool v5 = std::is_same_v<c::PointerType, float*>;

            EXPECT_EQ(v0, true);
            EXPECT_EQ(v1, true);
            EXPECT_EQ(v2, true);
            EXPECT_EQ(v3, true);
            EXPECT_EQ(v4, true);
            EXPECT_EQ(v5, true);

            EXPECT_EQ(cc::is_member, false);
            EXPECT_EQ(c::is_member, false);
        }
        {
            using cd = Spices::variable_traits<decltype(&VariableTraitsTestClass::d)>;
            using  d = Spices::variable_traits<decltype(&VariableTraitsTestN::d)>;

            constexpr bool v0 = std::is_same_v<cd::Type, std::string>;
            constexpr bool v1 = std::is_same_v<d::Type, std::string>;
            constexpr bool v2 = std::is_same_v<cd::Class, VariableTraitsTestClass>;
            constexpr bool v3 = std::is_same_v<d::Class, void>;
            constexpr bool v4 = std::is_same_v<cd::PointerType, std::string VariableTraitsTestClass::*>;
            constexpr bool v5 = std::is_same_v<d::PointerType, std::string*>;

            EXPECT_EQ(v0, true);
            EXPECT_EQ(v1, true);
            EXPECT_EQ(v2, true);
            EXPECT_EQ(v3, true);
            EXPECT_EQ(v4, true);
            EXPECT_EQ(v5, true);

            EXPECT_EQ(cd::is_member, true);
            EXPECT_EQ(d::is_member, false);
        }
        {
            using ce = Spices::variable_traits<decltype(&VariableTraitsTestClass::e)>;
            using  e = Spices::variable_traits<decltype(&VariableTraitsTestN::e)>;

            constexpr bool v0 = std::is_same_v<ce::Type, void*>;
            constexpr bool v1 = std::is_same_v<e::Type, void*>;
            constexpr bool v2 = std::is_same_v<ce::Class, VariableTraitsTestClass>;
            constexpr bool v3 = std::is_same_v<e::Class, void>;
            constexpr bool v4 = std::is_same_v<ce::PointerType, void* VariableTraitsTestClass::*>;
            constexpr bool v5 = std::is_same_v<e::PointerType, void**>;

            EXPECT_EQ(v0, true);
            EXPECT_EQ(v1, true);
            EXPECT_EQ(v2, true);
            EXPECT_EQ(v3, true);
            EXPECT_EQ(v4, true);
            EXPECT_EQ(v5, true);

            EXPECT_EQ(ce::is_member, true);
            EXPECT_EQ(e::is_member, false);
        }
    }
}