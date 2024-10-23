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
            : m_Name_a(name)
            , m_Name_b(name)
        {};

        ~FieldTraitsTest() = default;

        bool a(int q, float w) { return true; };
        bool b(int q, float w) const { return true; };
        static bool c(int q, float w) { return true; };

        std::string m_Name_a;
        const volatile std::string m_Name_b;
        static std::string m_Name_c;
    };

    std::string FieldTraitsTest::m_Name_c;

    /**
    * @brief Testing Spices::field_traits.
    */
    TEST(FieldTraitsTest_test, field_traits) {

        SPICESTEST_PROFILE_FUNCTION();

        {
            using traits = Spices::field_traits<decltype(&FieldTraitsTest::a)>;
            auto field = traits(&FieldTraitsTest::a, "a");

            EXPECT_EQ(field.pointer, &FieldTraitsTest::a);
            EXPECT_EQ(field.name, "a");

            EXPECT_EQ(field.is_member(), true);
            EXPECT_EQ(field.is_const(), false);
            EXPECT_EQ(field.is_function(), true);
            EXPECT_EQ(field.is_variable(), false);
            EXPECT_EQ(field.param_count(), 2);
        }

        {
            using traits = Spices::field_traits<decltype(&FieldTraitsTest::b)>;
            auto field = traits(&FieldTraitsTest::b, "b");

            EXPECT_EQ(field.pointer, &FieldTraitsTest::b);
            EXPECT_EQ(field.name, "b");

            EXPECT_EQ(field.is_member(), true);
            EXPECT_EQ(field.is_const(), true);
            EXPECT_EQ(field.is_function(), true);
            EXPECT_EQ(field.is_variable(), false);
            EXPECT_EQ(field.param_count(), 2);
        }

        {
            using traits = Spices::field_traits<decltype(&FieldTraitsTest::c)>;
            auto field = traits(&FieldTraitsTest::c, "c");

            EXPECT_EQ(field.pointer, &FieldTraitsTest::c);
            EXPECT_EQ(field.name, "c");

            EXPECT_EQ(field.is_member(), false);
            EXPECT_EQ(field.is_const(), false);
            EXPECT_EQ(field.is_function(), true);
            EXPECT_EQ(field.is_variable(), false);
            EXPECT_EQ(field.param_count(), 2);
        }

        {
            using traits = Spices::field_traits<decltype(&FieldTraitsTest::m_Name_a)>;
            auto field = traits(&FieldTraitsTest::m_Name_a, "m_Name_a");

            EXPECT_EQ(field.pointer, &FieldTraitsTest::m_Name_a);
            EXPECT_EQ(field.name, "m_Name_a");

            EXPECT_EQ(field.is_member(), true);
            EXPECT_EQ(field.is_const(), false);
            EXPECT_EQ(field.is_function(), false);
            EXPECT_EQ(field.is_variable(), true);
        }

        {
            using traits = Spices::field_traits<decltype(&FieldTraitsTest::m_Name_b)>;
            auto field = traits(&FieldTraitsTest::m_Name_b, "m_Name_b");

            EXPECT_EQ(field.pointer, &FieldTraitsTest::m_Name_b);
            EXPECT_EQ(field.name, "m_Name_b");

            EXPECT_EQ(field.is_member(), true);
            EXPECT_EQ(field.is_const(), true);
            EXPECT_EQ(field.is_function(), false);
            EXPECT_EQ(field.is_variable(), true);
        }

        {
            using traits = Spices::field_traits<decltype(&FieldTraitsTest::m_Name_c)>;
            auto field = traits(&FieldTraitsTest::m_Name_c, "m_Name_c");

            EXPECT_EQ(field.pointer, &FieldTraitsTest::m_Name_c);
            EXPECT_EQ(field.name, "m_Name_c");
            
            EXPECT_EQ(field.is_member(), false);
            EXPECT_EQ(field.is_const(), false);
            EXPECT_EQ(field.is_function(), false);
            EXPECT_EQ(field.is_variable(), true);
        }
    }
}