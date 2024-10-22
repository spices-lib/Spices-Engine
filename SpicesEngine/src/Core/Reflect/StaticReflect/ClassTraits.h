/**
* @file ClassTraits.h.
* @brief The ClassTraits Class Definitions and Implementation.
* @author VisualGMQ.
*/

#pragma once
#include "FieldTraits.h"
#include "Core/Container/Tuple.h"
#include <string>
namespace Spices {

	template<typename T>
	struct class_traits;

#define UCLASS(x)             template<> struct Spices::class_traits<x> {
#define UFUNCTIONS(...)       static constexpr auto functions = std::make_tuple(__VA_ARGS__);
#define UPROPERTYS(...)       static constexpr auto properties = std::make_tuple(__VA_ARGS__);
#define UFUNCTION(F)          field_traits{ F, #F }
#define UPROPERTY(F)          field_traits{ F, #F }
#define END_CLASS             };
	
}