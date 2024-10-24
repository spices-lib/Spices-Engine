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

    int i = 0;
    int* ip = &i;
    const int* cip = ip;

    class ClassTraitsTest
    {
    public:

        ClassTraitsTest()
            : ir(i)
            , ipr(ip)
            , ci(i)
            , cir(i)
            , cipr(cip)
            , cicpcr(ip)
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
        int* ip;
        std::reference_wrapper<int*> ipr;
        int** ipp;

        const int ci;
        std::reference_wrapper<const int> cir;
        const int* cip;
        std::reference_wrapper<const int*> cipr;
        const int** cipp;

        const int const* cicp;
        std::reference_wrapper<const int const* const> cicpcr;

        static int si;
        volatile int vi;
    };

    int ClassTraitsTest::si;

#ifdef CLASS_SCOPE
#undef CLASS_SCOPE
#endif
#define CLASS_SCOPE ClassTraitsTest

    UCLASS()
    UFUNCTIONS(
        //UFUNCTION(&ClassTraitsTest::ClassTraitsTest)),
        //UFUNCTION(&ClassTraitsTest::~ClassTraitsTest)),
        //UFUNCTION_T(bool(CLASS_SCOPE::*)(int, float), f),
        //UFUNCTION_T(bool(CLASS_SCOPE::*)(int, float, void*, bool(CLASS_SCOPE::*)(int**, int&&)), f),
        UFUNCTION(fc),
        //UFUNCTION(fs),
        //UFUNCTION(mf<int>),
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
        UPROPERTY(si),
        UPROPERTY(vi)
    )
    UCONSTRUCT(
        IterTuple(functions, [&](auto& elem) {
            std::cout << elem.name << " " << elem.pointer << std::endl;
        });
        IterTuple(properties, [&](auto& elem) {
            //std::cout << elem.name << " " << elem.pointer << std::endl;
        });
    )
    END_CLASS

#undef CLASS_SCOPE

}

namespace SpicesTest {

    /**
    * @brief Testing Spices::class_traits.
    */
    TEST(ClassTraits_test, class_traits) {

        SPICESTEST_PROFILE_FUNCTION();

        Spices::ClassTraitsTest test;
        auto ClassTraitsTestTraits = Spices::class_traits_i(test);

        //std::cout << " Functions: " << std::endl;
        //Spices::IterTuple(ClassTraitsTestTraits.functions, [](auto&& elem) {
        //    std::cout << "  " << elem.name << "  " << elem.pointer << std::endl;
        //});
        //
        //std::cout << " Properties: " << std::endl;
        //Spices::IterTuple(ClassTraitsTestTraits.properties, [](auto&& elem) {
        //    std::cout << "  " << elem.name << "  " << elem.pointer << std::endl;
        //});

        //std::_Mem_fn<bool(Spices::ClassTraitsTest::*)(int, float) const> p = std::mem_fn(&Spices::ClassTraitsTest::fc);
        //
        //std::cout << "P " << &p << std::endl;
        //std::cout << "R " << &Spices::ClassTraitsTest::fc << std::endl;
    }

}