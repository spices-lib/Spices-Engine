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

    int ClassTraitsTest::si;

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

    template<typename T>
    T& GetProperty(const std::string& property)
    {
        bool isStatic;
        size_t offset;
        //auto ptr;
        IterTuple(properties, [&](auto& elem) {
            if (property == elem.name)
            {
                if (elem.pointer == nullptr)
                {
                    isStatic = false;
                    offset = elem.offset;
                }
                else
                {
                    isStatic = true;
                    //ptr = const_cast<decltype(elem.pointer)>(elem.pointer);
                }
            }
        });

        if (isStatic)
        {
        }
        else
        {
            return *(T*)((char*)m_Instance + offset);
        }
    }

    auto& GetInst() { return *m_Instance; }

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

        int& ref = ClassTraitsTestTraits.GetProperty<int>("i");

        std::cout << offsetof(Spices::ClassTraitsTest, i) << std::endl;
        std::cout << &ref << std::endl;
        std::cout << &test.i << std::endl;
        std::cout << &test << std::endl;
    }

}