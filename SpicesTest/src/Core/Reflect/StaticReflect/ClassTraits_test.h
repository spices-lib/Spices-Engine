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

    int ei = 5;
    int* eip = &ei;
    const int* ecip = eip;

    class ClassTraitsTest
    {
    public:

        ClassTraitsTest()
            : ir(ei)
            , ipr(eip)
            , ci(ei)
            , cir(ei)
            , cipr(ecip)
            , cicpcr(eip)
            , i(150)
            , vi(30)
        {};

        ~ClassTraitsTest() = default;

        bool f(int q, float w) { return true; }
        bool f(int q, float w, void* c, bool(ClassTraitsTest::*)(int**, int&&)) { return true; }
        bool fc(int q, float w) const { return true; }
        static bool fs(int q, float w) { return true; }

        template<typename T>
        void mf(T a) {}

        template<typename F, typename ...Args>
        auto mfc(F func, Args  ...args) -> decltype(f(std::forward<Args>(args)...)) { return f(std::forward<Args>(args)...); }

        virtual void vf(int) {}

        int i;
        std::reference_wrapper<int> ir;
        int* ip = nullptr;
        std::reference_wrapper<int*> ipr;
        int** ipp = nullptr;

        const int ci;
        std::reference_wrapper<const int> cir;
        const int* cip = nullptr;
        std::reference_wrapper<const int*> cipr;
        const int** cipp = nullptr;

        const int const* cicp = nullptr;
        std::reference_wrapper<const int const* const> cicpcr;

        static int si;
        volatile int vi;
    };

    int ClassTraitsTest::si = 500;
    Spices::ClassTraitsTest inst;

#ifdef CLASS_SCOPE
#undef CLASS_SCOPE
#endif
#define CLASS_SCOPE ClassTraitsTest

    UCLASS()
    UFUNCTIONS(
        UCONSTRUCT(void(CLASS_SCOPE::*)(), CLASS_SCOPE),
        UDECONSTRUCT(void(CLASS_SCOPE::*)(), ~CLASS_SCOPE),
        UFUNCTION_T(bool(CLASS_SCOPE::*)(int, float), f),
        UFUNCTION_T(bool(CLASS_SCOPE::*)(int, float, void*, bool(CLASS_SCOPE::*)(int**, int&&)), f),
        UFUNCTION(fc),
        UFUNCTION(fs),
        UFUNCTION(mf<int>),
        //UFUNCTION(static_cast<bool(ClassTraitsTest::*)(bool(*)(int, char*), int, char*>)>(&ClassTraitsTest::mf<bool(*)(int, char*), int, char*>)),
        //UFUNCTION(mfc<void(*)()>),
        UFUNCTION(vf)
    )
    UPROPERTYS(
        UPROPERTY(i),
        UPROPERTY(ir),
        UPROPERTY(ip),
        UPROPERTY(ipr),
        UPROPERTY(ipp),
        UPROPERTY(ci),
        UPROPERTY(cir),
        UPROPERTY(cip),
        UPROPERTY(cipr),
        UPROPERTY(cipp),
        UPROPERTY(cicp),
        UPROPERTY(cicpcr),
        UPROPERTY_S(si),
        UPROPERTY(vi)
    )
    
    END_CLASS

#undef CLASS_SCOPE

}

namespace SpicesTest {

    /**
    * @brief Testing Spices::class_traits::GetProperty.
    */
    TEST(ClassTraits_test, GetProperty) {

        SPICESTEST_PROFILE_FUNCTION();

        using namespace Spices;

        auto ClassTraitsTestTraits = Spices::class_traits_i(inst);

        {
            auto [type, ptr] = ClassTraitsTestTraits.GetProperty<int>("i");

            EXPECT_EQ(type, ClassItemType::Member);
            EXPECT_NE(ptr, nullptr);
        }

        {
            auto [type, ptr] = ClassTraitsTestTraits.GetProperty<std::reference_wrapper<int>>("ir");

            EXPECT_EQ(type, ClassItemType::Member);
            EXPECT_NE(ptr, nullptr);
        }

        {
            auto [type, ptr] = ClassTraitsTestTraits.GetProperty<int*>("ip");

            EXPECT_EQ(type, ClassItemType::Member);
            EXPECT_NE(ptr, nullptr);
        }

        {
            auto [type, ptr] = ClassTraitsTestTraits.GetProperty<std::reference_wrapper<int*>>("ipr");

            EXPECT_EQ(type, ClassItemType::Member);
            EXPECT_NE(ptr, nullptr);
        }

        {
            auto [type, ptr] = ClassTraitsTestTraits.GetProperty<int**>("ipp");

            EXPECT_EQ(type, ClassItemType::Member);
            EXPECT_NE(ptr, nullptr);
        }

        {
            auto [type, ptr] = ClassTraitsTestTraits.GetProperty<const int>("ci");

            EXPECT_EQ(type, ClassItemType::Member);
            EXPECT_NE(ptr, nullptr);
        }

        {
            auto [type, ptr] = ClassTraitsTestTraits.GetProperty<std::reference_wrapper<const int>>("cir");

            EXPECT_EQ(type, ClassItemType::Member);
            EXPECT_NE(ptr, nullptr);
        }

        {
            auto [type, ptr] = ClassTraitsTestTraits.GetProperty<const int*>("cip");

            EXPECT_EQ(type, ClassItemType::Member);
            EXPECT_NE(ptr, nullptr);
        }

        {
            auto [type, ptr] = ClassTraitsTestTraits.GetProperty<std::reference_wrapper<const int*>>("cipr");

            EXPECT_EQ(type, ClassItemType::Member);
            EXPECT_NE(ptr, nullptr);
        }

        {
            auto [type, ptr] = ClassTraitsTestTraits.GetProperty<const int**>("cipp");

            EXPECT_EQ(type, ClassItemType::Member);
            EXPECT_NE(ptr, nullptr);
        }

        {
            auto [type, ptr] = ClassTraitsTestTraits.GetProperty<const int const*>("cicp");

            EXPECT_EQ(type, ClassItemType::Member);
            EXPECT_NE(ptr, nullptr);
        }

        {
            auto [type, ptr] = ClassTraitsTestTraits.GetProperty<std::reference_wrapper<const int const* const>>("cicpcr");

            EXPECT_EQ(type, ClassItemType::Member);
            EXPECT_NE(ptr, nullptr);
        }

        {
            auto [type, ptr] = ClassTraitsTestTraits.GetProperty<int>("si");

            EXPECT_EQ(type, ClassItemType::NonMember);
            EXPECT_NE(ptr, nullptr);
        }

        {
            auto [type, ptr] = ClassTraitsTestTraits.GetProperty<volatile int >("vi");

            EXPECT_EQ(type, ClassItemType::Member);
            EXPECT_NE(ptr, nullptr);
        }
    }

    /**
    * @brief Testing Spices::class_traits::GetProperty_V.
    */
    TEST(ClassTraits_test, GetProperty_V) {

        SPICESTEST_PROFILE_FUNCTION();

        using namespace Spices;

        auto ClassTraitsTestTraits = Spices::class_traits_i(inst);

        EXPECT_EQ(ClassTraitsTestTraits.GetProperty_V<int>("i"), &inst.i);
        EXPECT_EQ(ClassTraitsTestTraits.GetProperty_V<std::reference_wrapper<int>>("ir"), &inst.ir);
        EXPECT_EQ(ClassTraitsTestTraits.GetProperty_V<int*>("ip"), &inst.ip);
        EXPECT_EQ(ClassTraitsTestTraits.GetProperty_V<std::reference_wrapper<int*>>("ipr"), &inst.ipr);
        EXPECT_EQ(ClassTraitsTestTraits.GetProperty_V<int**>("ipp"), &inst.ipp);

        EXPECT_EQ(ClassTraitsTestTraits.GetProperty_V<const int>("ci"), &inst.ci);
        EXPECT_EQ(ClassTraitsTestTraits.GetProperty_V<std::reference_wrapper<const int>>("cir"), &inst.cir);
        EXPECT_EQ(ClassTraitsTestTraits.GetProperty_V<const int*>("cip"), &inst.cip);
        EXPECT_EQ(ClassTraitsTestTraits.GetProperty_V<std::reference_wrapper<const int*>>("cipr"), &inst.cipr);
        EXPECT_EQ(ClassTraitsTestTraits.GetProperty_V<const int**>("cipp"), &inst.cipp);

        EXPECT_EQ(ClassTraitsTestTraits.GetProperty_V<const int const*>("cicp"), &inst.cicp);
        EXPECT_EQ(ClassTraitsTestTraits.GetProperty_V<std::reference_wrapper<const int const* const>>("cicpcr"), &inst.cicpcr);

        EXPECT_EQ(ClassTraitsTestTraits.GetProperty_V<int>("si"), &inst.si);
        EXPECT_EQ(ClassTraitsTestTraits.GetProperty_V<volatile int >("vi"), &inst.vi);
    }

    /**
    * @brief Testing Spices::class_traits::GetProperty_T.
    */
    TEST(ClassTraits_test, GetProperty_T) {

    }
    
}