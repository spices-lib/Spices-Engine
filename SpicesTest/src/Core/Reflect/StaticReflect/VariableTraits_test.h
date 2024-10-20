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
        int         a;
        uint32_t    b;
        float       c;
        std::string d;
        void*       e;
    };

    /**
    * @brief Non Member Variable
    */
    namespace VariableTraitsTestN
    {
        int         a;
        uint32_t    b;
        float       c;
        std::string d;
        void*       e;
    }

    /**
    * @brief Testing Spices::VariableTraits.
    */
    TEST(VariableTraitsTest, VariableTraits) {

        SPICESTEST_PROFILE_FUNCTION();

        {
            {
                using ca = Spices::variable_traits<VARIABLE_TYPE(VariableTraitsTestClass::a)>;
                using  a = Spices::variable_traits<VARIABLE_TYPE(VariableTraitsTestN::a)>;

                constexpr bool v0 = std::is_same_v<ca::Type, int>;
                constexpr bool v1 = std::is_same_v<a::Type, int>;
                constexpr bool v2 = std::is_same_v<ca::Class, VariableTraitsTestClass>;
                constexpr bool v3 = std::is_same_v<a::Class, void>;

                EXPECT_EQ(v0, true);
                EXPECT_EQ(v1, true);
                EXPECT_EQ(v2, true);
                EXPECT_EQ(v3, true);

                EXPECT_EQ(ca::is_member, true);
                EXPECT_EQ(a::is_member, false);
            }
            {
                using cb = Spices::variable_traits<VARIABLE_TYPE(VariableTraitsTestClass::b)>;
                using  b = Spices::variable_traits<VARIABLE_TYPE(VariableTraitsTestN::b)>;

                constexpr bool v0 = std::is_same_v<cb::Type, uint32_t>;
                constexpr bool v1 = std::is_same_v<b::Type, uint32_t>;
                constexpr bool v2 = std::is_same_v<cb::Class, VariableTraitsTestClass>;
                constexpr bool v3 = std::is_same_v<b::Class, void>;

                EXPECT_EQ(v0, true);
                EXPECT_EQ(v1, true);
                EXPECT_EQ(v2, true);
                EXPECT_EQ(v3, true);

                EXPECT_EQ(cb::is_member, true);
                EXPECT_EQ(b::is_member, false);
            }
            {
                using cc = Spices::variable_traits<VARIABLE_TYPE(VariableTraitsTestClass::c)>;
                using  c = Spices::variable_traits<VARIABLE_TYPE(VariableTraitsTestN::c)>;

                constexpr bool v0 = std::is_same_v<cc::Type, float>;
                constexpr bool v1 = std::is_same_v<c::Type, float>;
                constexpr bool v2 = std::is_same_v<cc::Class, VariableTraitsTestClass>;
                constexpr bool v3 = std::is_same_v<c::Class, void>;

                EXPECT_EQ(v0, true);
                EXPECT_EQ(v1, true);
                EXPECT_EQ(v2, true);
                EXPECT_EQ(v3, true);

                EXPECT_EQ(cc::is_member, true);
                EXPECT_EQ(c::is_member, false);
            }
            {
                using cd = Spices::variable_traits<VARIABLE_TYPE(VariableTraitsTestClass::d)>;
                using  d = Spices::variable_traits<VARIABLE_TYPE(VariableTraitsTestN::d)>;

                constexpr bool v0 = std::is_same_v<cd::Type, std::string>;
                constexpr bool v1 = std::is_same_v<d::Type, std::string>;
                constexpr bool v2 = std::is_same_v<cd::Class, VariableTraitsTestClass>;
                constexpr bool v3 = std::is_same_v<d::Class, void>;

                EXPECT_EQ(v0, true);
                EXPECT_EQ(v1, true);
                EXPECT_EQ(v2, true);
                EXPECT_EQ(v3, true);

                EXPECT_EQ(cd::is_member, true);
                EXPECT_EQ(d::is_member, false);
            }
            {
                using ce = Spices::variable_traits<VARIABLE_TYPE(VariableTraitsTestClass::e)>;
                using  e = Spices::variable_traits<VARIABLE_TYPE(VariableTraitsTestN::e)>;

                constexpr bool v0 = std::is_same_v<ce::Type, void*>;
                constexpr bool v1 = std::is_same_v<e::Type, void*>;
                constexpr bool v2 = std::is_same_v<ce::Class, VariableTraitsTestClass>;
                constexpr bool v3 = std::is_same_v<e::Class, void>;

                EXPECT_EQ(v0, true);
                EXPECT_EQ(v1, true);
                EXPECT_EQ(v2, true);
                EXPECT_EQ(v3, true);

                EXPECT_EQ(ce::is_member, true);
                EXPECT_EQ(e::is_member, false);
            }
        }
    }
}