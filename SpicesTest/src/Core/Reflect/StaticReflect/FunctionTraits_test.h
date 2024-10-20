/**
* @file FunctionTraits_test.h.
* @brief The FunctionTraits_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Reflect/StaticReflect/FunctionTraits.hpp>
#include "Instrumentor.h"

namespace SpicesTest {

    /**
    * @brief Member Function
    */
    class FunctionTraitsTestClass
    {
    public:
        
        bool f(int a, float b, void* c) {}
        bool fc(int a, float b, void* c) const {}
        static bool sf(int a, float b, void* c) {}
    };

    /**
    * @brief Non Member Function
    */
    namespace FunctionTraitsTestN
    {
        bool f(int a, float b, void* c) {}
        static bool sf(int a, float b, void* c) {}
    }

    /**
    * @brief Testing Spices::FunctionTraits.
    */
    TEST(FunctionTraitsTest, FunctionTraits) {

        SPICESTEST_PROFILE_FUNCTION();

        {
            using trait = Spices::function_traits<FUNCTION_TYPE(FunctionTraitsTestClass::f)>;

            
        }
    }
}