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
        void mf(int a) {}

        int i;
        int& ir;
        int* ip;
        int*& ipr;
        int** ipp;

        const int ci;
        const int& cir;
        const int* cip;
        const int*& cipr;
        const int** cipp;

        const int const* cicp;
        const int const* const& cicpcr;

        static int si;
        volatile int vi;
    };

    int ClassTraitsTest::si;

    UCLASS(ClassTraitsTest)
    UFUNCTIONS(
        UFUNCTION(static_cast<bool(ClassTraitsTest::*)(int, float)>(&ClassTraitsTest::f)),
        UFUNCTION(static_cast<bool(ClassTraitsTest::*)(int, float, void*, bool(ClassTraitsTest::*)(int**, int&&))>(&ClassTraitsTest::f)),
        UFUNCTION(&ClassTraitsTest::fc),
        UFUNCTION(&ClassTraitsTest::fs)
    )
    UPROPERTYS(
        UPROPERTY(&ClassTraitsTest::i),
        UPROPERTY(&ClassTraitsTest::ir),
        UPROPERTY(&ClassTraitsTest::ip),
        UPROPERTY(&ClassTraitsTest::ipr),
        UPROPERTY(&ClassTraitsTest::ipp),
        UPROPERTY(&ClassTraitsTest::ci),
        UPROPERTY(&ClassTraitsTest::cir),
        UPROPERTY(&ClassTraitsTest::cip),
        UPROPERTY(&ClassTraitsTest::cipr),
        UPROPERTY(&ClassTraitsTest::cipp),
        UPROPERTY(&ClassTraitsTest::cicp),
        UPROPERTY(&ClassTraitsTest::cicpcr),
        UPROPERTY(&ClassTraitsTest::si),
        UPROPERTY(&ClassTraitsTest::vi)
    )
    END_CLASS

}

namespace SpicesTest {

    /**
    * @brief Testing Spices::class_traits.
    */
    TEST(ClassTraits_test, class_traits) {

        SPICESTEST_PROFILE_FUNCTION();

        //auto FieldTraitsTestTraits = Spices::class_traits<Spices::ClassTraitsTest>();
        //
        //Spices::IterTuple(FieldTraitsTestTraits.functions, [](auto&& elem) {
        //
        //});
    }

}