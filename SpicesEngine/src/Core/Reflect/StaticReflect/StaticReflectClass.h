/**
* @file StaticReflectClass.h.
* @brief The StaticReflectClass Class Definitions and Implementation.
* @author VisualGMQ.
*/

#pragma once
#include "FieldTraits.h"
#include "Core/Container/Tuple.h"
#include <string>
namespace Spices {

	template<typename T>
	struct SRClass;

#define BEGIN_CLASS(x)  template<> struct SRClass<x> {
#define functions(...)  static constexpr auto functions = std::make_tuple(__VA_ARGS__);
#define func(F)         field_traits{ F, #F }
#define END_CLASS()     };

    class FieldTraitsTest final
    {
    public:

        FieldTraitsTest(const std::string& name)
            : m_Name(name)
        {};

        ~FieldTraitsTest() = default;

        bool a(int q, float w) { return true; };

        std::string m_Name;
    };

    BEGIN_CLASS(FieldTraitsTest)
        functions(
            func(&FieldTraitsTest::a)
        )
    END_CLASS()
}