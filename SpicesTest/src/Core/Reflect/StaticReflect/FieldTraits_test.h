/**
* @file FieldTraits_test.h.
* @brief The FieldTraits_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Reflect/StaticReflect/FieldTraits.h>
#include "Instrumentor.h"

namespace SpicesTest {

    class FieldTraitsTest
    {
    public:

        FieldTraitsTest(const std::string& name)
            : m_Name(name)
        {};

        ~FieldTraitsTest() = default;

        bool a(int q, float w) { return true; };

        std::string m_Name;
    };

    /**
    * @brief Testing Spices::field_traits.
    */
    TEST(FieldTraitsTest_test, field_traits) {

        SPICESTEST_PROFILE_FUNCTION();

        auto field = Spices::field_traits(&FieldTraitsTest::a, "a");

        EXPECT_EQ(field.pointer, &FieldTraitsTest::a);
        EXPECT_EQ(field.name, "a");

        EXPECT_EQ(field.is_member(),   true );
        EXPECT_EQ(field.is_const(),    false);
        EXPECT_EQ(field.is_function(), true );
        EXPECT_EQ(field.is_variable(), false);
    }
}