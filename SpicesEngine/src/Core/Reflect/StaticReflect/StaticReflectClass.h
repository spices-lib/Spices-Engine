/**
* @file StaticReflectClass.h.
* @brief The StaticReflectClass Class Definitions and Implementation.
* @author VisualGMQ.
*/

#pragma once
#include "FieldTraits.h"
#include "Core/Container/Tuple.h"

namespace Spices {

	template<typename T>
	struct SRClass;

#define BEGIN_CLASS(x)           \
	template<>                   \
	struct Spices::SRClass<x>    \
	{

#define functions(...)           \
	static constexpr auto functions = std::make_tuple(__VA_ARGS__);

#define func(F)                  \
	field_traits{ F, #F }

#define END_CLASS()  };

}