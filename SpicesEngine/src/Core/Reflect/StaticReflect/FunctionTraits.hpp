/**
* @file FunctionTraits.hpp.
* @brief The FunctionTraits Class Definitions and Implementation.
* @author Spices & VisualGMQ.
*/

#pragma once
#include <type_traits>
#include <tuple>

namespace Spices {

#define FUNCTION_TYPE(v) decltype(&v)

	namespace detail {

		template<typename>
		struct basic_function_traits;

		template<typename Ret, typename ...Args>
		struct basic_function_traits<Ret(Args...)>
		{
			using Params     = std::tuple<Args...>;
			using ReturnType = Ret;
		};
	}

	template<typename>
	struct function_traits;

	template<typename Ret, typename ...Args>
	struct function_traits<Ret(Args...)> : public datail::basic_function_traits<Ret(Args...)>
	{
		using Type                      = Ret(Args...);
		using ParamsWithClass           = std::tuple<Args...>;
		using Pointer                   = Ret(*)(Args...);
		static constexpr bool is_member = false;
		static constexpr bool is_const  = false;
	};

	template<typename Ret, typename Class, typename ...Args>
	struct function_traits<Ret(Class::*)(Args...)> : datail::basic_function_traits<Ret(Args...)>
	{
		using Type                      = Ret(Class::*)(Args...);
		using ParamsWithClass           = std::tuple<Class*, Args...>;
		using Pointer                   = Ret(Class::*)(Args...);
		static constexpr bool is_member = true;
		static constexpr bool is_const  = false;
	};

	template<typename Ret, typename Class, typename ...Args>
	struct function_traits<Ret(Class::*)(Args...) const> : datail::basic_function_traits<Ret(Args...)>
	{
		using Type                      = Ret(Class::*)(Args...) const;
		using ParamsWithClass           = std::tuple<Class*, Args...>;
		using Pointer                   = Ret(Class::*)(Args...) const;
		static constexpr bool is_member = true;
		static constexpr bool is_const  = true;
	};

}