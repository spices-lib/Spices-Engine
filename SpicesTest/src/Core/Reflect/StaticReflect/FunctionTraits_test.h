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
        
        bool f(int a, float b) { return true; }
        int f(int a, float b, void* c) { return 1; }
        bool fc(int a, float b, void* c) const { return true; }
        static bool sf(int a, float b, void* c) { return true; }

        template<typename T, typename ...Args>
        auto GetFResult(T&& func, Args&&... args) -> decltype(func(std::forward<Args>(args)...))
        {
            using RType = decltype(func(args...));

            return func(args...);
        }
    };

    /**
    * @brief Non Member Function
    */
    namespace FunctionTraitsTestN
    {
        bool f(int a, float b) { return true; }
        static bool sf(int a, float b, void* c) { return true; }
    }

    /**
    * @brief Testing Spices::FunctionTraits.
    */
    TEST(FunctionTraitsTest, FunctionTraits) {

        SPICESTEST_PROFILE_FUNCTION();

        {
            using trait = Spices::function_traits<decltype(static_cast<bool(FunctionTraitsTestClass::*)(int, float)>(&FunctionTraitsTestClass::f))>;

            constexpr bool v0 = std::is_same_v<trait::Params, std::tuple<int, float>>;
            constexpr bool v1 = std::is_same_v<trait::ReturnType, bool>;
            constexpr bool v2 = std::is_same_v<trait::ParamsWithClass, std::tuple<FunctionTraitsTestClass*, int, float>>;
            constexpr bool v3 = std::is_same_v<trait::Pointer, bool(FunctionTraitsTestClass::*)(int, float)>;
            constexpr bool v4 = std::is_same_v<trait::Type, bool(FunctionTraitsTestClass::*)(int, float)>;

            EXPECT_EQ(v0, true);
            EXPECT_EQ(v1, true);
            EXPECT_EQ(v2, true);
            EXPECT_EQ(v3, true);
            EXPECT_EQ(v4, true);

            EXPECT_EQ(trait::is_const, false);
            EXPECT_EQ(trait::is_member, true);
        }

        {
            using trait = Spices::function_traits<decltype(static_cast<int(FunctionTraitsTestClass::*)(int, float, void*)>(&FunctionTraitsTestClass::f))>;

            constexpr bool v0 = std::is_same_v<trait::Params, std::tuple<int, float, void*>>;
            constexpr bool v1 = std::is_same_v<trait::ReturnType, int>;
            constexpr bool v2 = std::is_same_v<trait::Class, FunctionTraitsTestClass>;
            constexpr bool v3 = std::is_same_v<trait::ParamsWithClass, std::tuple<FunctionTraitsTestClass*, int, float, void*>>;
            constexpr bool v4 = std::is_same_v<trait::Pointer, int(FunctionTraitsTestClass::*)(int, float, void*)>;
            constexpr bool v5 = std::is_same_v<trait::Type, int(FunctionTraitsTestClass::*)(int, float, void*)>;

            EXPECT_EQ(v0, true);
            EXPECT_EQ(v1, true);
            EXPECT_EQ(v2, true);
            EXPECT_EQ(v3, true);
            EXPECT_EQ(v4, true);
            EXPECT_EQ(v5, true);

            EXPECT_EQ(trait::is_const, false);
            EXPECT_EQ(trait::is_member, true);
        }

        {
            using trait = Spices::function_traits<decltype(&FunctionTraitsTestClass::fc)>;

            constexpr bool v0 = std::is_same_v<trait::Params, std::tuple<int, float, void*>>;
            constexpr bool v1 = std::is_same_v<trait::ReturnType, bool>;
            constexpr bool v2 = std::is_same_v<trait::Class, FunctionTraitsTestClass>;
            constexpr bool v3 = std::is_same_v<trait::ParamsWithClass, std::tuple<FunctionTraitsTestClass*, int, float, void*>>;
            constexpr bool v4 = std::is_same_v<trait::Pointer, bool(FunctionTraitsTestClass::*)(int, float, void*) const>;
            constexpr bool v5 = std::is_same_v<trait::Type, bool(FunctionTraitsTestClass::*)(int, float, void*) const>;

            EXPECT_EQ(v0, true);
            EXPECT_EQ(v1, true);
            EXPECT_EQ(v2, true);
            EXPECT_EQ(v3, true);
            EXPECT_EQ(v4, true);
            EXPECT_EQ(v5, true);

            EXPECT_EQ(trait::is_const, true);
            EXPECT_EQ(trait::is_member, true);
        }

        {
            using trait = Spices::function_traits<decltype(&FunctionTraitsTestClass::sf)>;

            constexpr bool v0 = std::is_same_v<trait::Params, std::tuple<int, float, void*>>;
            constexpr bool v1 = std::is_same_v<trait::ReturnType, bool>;
            constexpr bool v2 = std::is_same_v<trait::Class, void>;
            constexpr bool v3 = std::is_same_v<trait::ParamsWithClass, std::tuple<int, float, void*>>;
            constexpr bool v4 = std::is_same_v<trait::Pointer, bool(*)(int, float, void*)>;
            constexpr bool v5 = std::is_same_v<trait::Type, bool(*)(int, float, void*)>;

            EXPECT_EQ(v0, true);
            EXPECT_EQ(v1, true);
            EXPECT_EQ(v2, true);
            EXPECT_EQ(v3, true);
            EXPECT_EQ(v4, true);
            EXPECT_EQ(v5, true);

            EXPECT_EQ(trait::is_const, false);
            EXPECT_EQ(trait::is_member, false);
        }

        {
            using trait = Spices::function_traits<int(FunctionTraitsTestClass::*)(int(*)(float, void*), float, void*)>;

            constexpr bool v0 = std::is_same_v<trait::Params, std::tuple<int(*)(float, void*), float, void*>>;
            constexpr bool v1 = std::is_same_v<trait::ReturnType, int>;
            constexpr bool v2 = std::is_same_v<trait::Class, FunctionTraitsTestClass>;
            constexpr bool v3 = std::is_same_v<trait::ParamsWithClass, std::tuple<FunctionTraitsTestClass*, int(*)(float, void*), float, void*>>;
            constexpr bool v4 = std::is_same_v<trait::Pointer, int(FunctionTraitsTestClass::*)(int(*)(float, void*), float, void*)>;
            constexpr bool v5 = std::is_same_v<trait::Type, int(FunctionTraitsTestClass::*)(int(*)(float, void*), float, void*)>;

            EXPECT_EQ(v0, true);
            EXPECT_EQ(v1, true);
            EXPECT_EQ(v2, true);
            EXPECT_EQ(v3, true);
            EXPECT_EQ(v4, true);
            EXPECT_EQ(v5, true);

            EXPECT_EQ(trait::is_const, false);
            EXPECT_EQ(trait::is_member, true);
        }

        {
            using trait = Spices::function_traits<decltype(static_cast<bool(*)(int, float)>(&FunctionTraitsTestN::f))>;

            constexpr bool v0 = std::is_same_v<trait::Params, std::tuple<int, float>>;
            constexpr bool v1 = std::is_same_v<trait::ReturnType, bool>;
            constexpr bool v2 = std::is_same_v<trait::Class, void>;
            constexpr bool v3 = std::is_same_v<trait::ParamsWithClass, std::tuple<int, float>>;
            constexpr bool v4 = std::is_same_v<trait::Pointer, bool(*)(int, float)>;
            constexpr bool v5 = std::is_same_v<trait::Type, bool(*)(int, float)>;

            EXPECT_EQ(v0, true);
            EXPECT_EQ(v1, true);
            EXPECT_EQ(v2, true);
            EXPECT_EQ(v3, true);
            EXPECT_EQ(v4, true);
            EXPECT_EQ(v5, true);

            EXPECT_EQ(trait::is_const, false);
            EXPECT_EQ(trait::is_member, false);
        }

        {
            using trait = Spices::function_traits<decltype(&FunctionTraitsTestN::sf)>;

            constexpr bool v0 = std::is_same_v<trait::Params, std::tuple<int, float, void*>>;
            constexpr bool v1 = std::is_same_v<trait::ReturnType, bool>;
            constexpr bool v2 = std::is_same_v<trait::Class, void>;
            constexpr bool v3 = std::is_same_v<trait::ParamsWithClass, std::tuple<int, float, void*>>;
            constexpr bool v4 = std::is_same_v<trait::Pointer, bool(*)(int, float, void*)>;
            constexpr bool v5 = std::is_same_v<trait::Type, bool(*)(int, float, void*)>;

            EXPECT_EQ(v0, true);
            EXPECT_EQ(v1, true);
            EXPECT_EQ(v2, true);
            EXPECT_EQ(v3, true);
            EXPECT_EQ(v4, true);
            EXPECT_EQ(v5, true);

            EXPECT_EQ(trait::is_const, false);
            EXPECT_EQ(trait::is_member, false);
        }
    }
}