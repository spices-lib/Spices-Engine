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

	template<typename>
	struct class_traits;

#define UCLASS()              template<> struct Spices::class_traits<CLASS_SCOPE> {
#define UCONSTRUCT(...)       class_traits(CLASS_SCOPE& inst){__VA_ARGS__}
#define UFUNCTIONS(...)       static constexpr auto functions = std::make_tuple(__VA_ARGS__);
#define UPROPERTYS(...)       static constexpr auto properties = std::make_tuple(__VA_ARGS__);
#define UFUNCTION_T(T, N)     field_traits{ (T)&CLASS_SCOPE::N, #N }
#define UFUNCTION(N)          field_traits{ &CLASS_SCOPE::N, #N }
#define UPROPERTY(N)          field_traits{ &CLASS_SCOPE::N, #N }
#define END_CLASS             };
	
	template<typename T>
	auto class_traits_i(T& inst)
	{
		auto traits = class_traits<T>(inst);

		return std::move(traits);
	}

}