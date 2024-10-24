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

#define UCLASS()                                            \
	template<>                                              \
	struct Spices::class_traits<CLASS_SCOPE>                \
	{                                                       \
		CLASS_SCOPE* m_Instance;                            \
		class_traits(CLASS_SCOPE& inst)                     \
		{                                                   \
			m_Instance = &inst;                             \
		}

#define UCONSTRUCT(T, N)      field_traits<T>{ nullptr, #N }
#define UDECONSTRUCT(T, N)    field_traits<T>{ nullptr, #N }
#define UFUNCTIONS(...)       static constexpr auto functions  = std::make_tuple(__VA_ARGS__);
#define UPROPERTYS(...)       static constexpr auto properties = std::make_tuple(__VA_ARGS__);
#define UFUNCTION_T(T, N)     field_traits<T>{ nullptr, #N }
#define UFUNCTION(N)          field_traits<decltype(&CLASS_SCOPE::N)>{ nullptr, #N }
#define UPROPERTY(N)          field_traits<decltype(&CLASS_SCOPE::N)>{ nullptr, #N, offsetof(CLASS_SCOPE, N) }
#define UPROPERTY_S(N)        field_traits<decltype(&CLASS_SCOPE::N)>{ &CLASS_SCOPE::N, #N, 0 }
#define END_CLASS             };

	template<typename T>
	auto class_traits_i(T& inst)
	{
		auto traits = class_traits<T>(inst);

		return std::move(traits);
	}

}