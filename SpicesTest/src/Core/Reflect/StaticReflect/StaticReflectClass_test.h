/**
* @file StaticReflectClass_test.h.
* @brief The StaticReflectClass_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Reflect/StaticReflect/StaticReflectClass.h>
#include "Instrumentor.h"

namespace Spices {

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

    template<> 
    struct Spices::SRClass<FieldTraitsTest> 
    {
        auto d = field_traits{ &FieldTraitsTest::a, "&FieldTraitsTest::a" };
    };
}

namespace SpicesTest {

    /**
    * @brief Testing Spices::field_traits.
    */
    TEST(StaticReflectClass_test, StaticReflect) {

        SPICESTEST_PROFILE_FUNCTION();
    }
}