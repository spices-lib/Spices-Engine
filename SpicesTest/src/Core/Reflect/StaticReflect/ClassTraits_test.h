/**
* @file ClassTraits_test.h.
* @brief The ClassTraits_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Reflect/StaticReflect/ClassTraits.h>
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

    UCLASS(FieldTraitsTest)
    UFUNCTIONS(
        UFUNCTION(&FieldTraitsTest::a)
    )
    UPROPERTYS(
        UPROPERTY(&FieldTraitsTest::m_Name)
    )
    END_CLASS

}

namespace SpicesTest {

    /**
    * @brief Testing Spices::class_traits.
    */
    TEST(ClassTraits_test, class_traits) {

        SPICESTEST_PROFILE_FUNCTION();

        auto FieldTraitsTestTraits = Spices::class_traits<Spices::FieldTraitsTest>();

        Spices::IterTuple(FieldTraitsTestTraits.functions, [](auto&& elem) {

        });
    }

}