/**
* @file FieldTraits.h.
* @brief The FieldTraits Class Definitions and Implementation.
* @author VisualGMQ.
*/

#pragma once

#include "FunctionTraits.h"
#include "VariableTraits.h"

#include <type_traits>
#include <tuple>

namespace Spices {

	template<typename T>
	struct TypeInfo;

	template<typename Ret, typename ...Params>
	auto function_pointer_type(Ret(*)(Params...)) -> Ret(*)(Params...);

	template<typename Ret, typename Class, typename ...Params>
	auto function_pointer_type(Ret(Class::*)(Params...)) -> Ret(Class::*)(Params...);

	template<typename Ret, typename Class, typename ...Params>
	auto function_pointer_type(Ret(Class::*)(Params...) const) -> Ret(Class::*)(Params...) const;

	template<auto F>
	using function_pointer_type_t = decltype(function_pointer_type(F));

	template<auto F>
	using function_traits_t = function_traits<function_pointer_type_t<F>>;

	template<typename T>
	struct is_function
	{
		static constexpr bool value = std::is_function_v<std::remove_pointer_t<T>> || std::is_member_function_pointer_v<T>;
	};

	template<typename T>
	constexpr bool is_function_v = is_function<T>::value;

	template<typename T, bool isFunc>
	struct basic_field_traits;

	template<typename T>
	struct basic_field_traits<T, true> : public function_traits<T>
	{
		using traits = function_traits<T>;

		constexpr bool is_member() const
		{
			return traits::is_member;
		}

		constexpr bool is_const() const
		{
			return traits::is_const;
		}

		constexpr bool is_function() const
		{
			return true;
		}

		constexpr bool is_variable() const
		{
			return false;
		}
	};

	template<typename T>
	struct basic_field_traits<T, false> : public variable_traits<T>
	{
		using traits = variable_traits<T>;

		constexpr bool is_member() const
		{
			return traits::is_member;
		}

		constexpr bool is_const() const
		{
			return traits::is_const;
		}

		constexpr bool is_function() const
		{
			return false;
		}

		constexpr bool is_variable() const
		{
			return true;
		}

		constexpr size_t param_count() const
		{
			return std::tuple_size_v<typename traits::args>;
		}
	};

	template<typename T>
	struct field_traits : public basic_field_traits<T, is_function_v<T>>
	{
		constexpr field_traits(T&& pointer, std::string_view name) 
			: pointer{ pointer }
			, name(name.substr(name.find_last_of(":"))) 
		{}

		T pointer;
		std::string name;
	};
}