/**
* @file FunctionTraits.hpp.
* @brief The FunctionTraits Class Definitions and Implementation.
* @author Spices & VisualGMQ.
*/

#pragma once
#include <type_traits>
#include <tuple>

namespace Spices {

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
	struct function_traits<Ret(*)(Args...)> : detail::basic_function_traits<Ret(Args...)>
	{
		using Type                      = Ret(*)(Args...);
		using Class                     = void;
		using ParamsWithClass           = std::tuple<Args...>;
		using Pointer                   = Ret(*)(Args...);
		static constexpr bool is_member = false;
		static constexpr bool is_const  = false;
	};

	template<typename Ret, typename C, typename ...Args>
	struct function_traits<Ret(C::*)(Args...)> : detail::basic_function_traits<Ret(Args...)>
	{
		using Type                      = Ret(C::*)(Args...);
		using Class                     = C;
		using ParamsWithClass           = std::tuple<Class*, Args...>;
		using Pointer                   = Ret(C::*)(Args...);
		static constexpr bool is_member = true;
		static constexpr bool is_const  = false;
	};

	template<typename Ret, typename C, typename ...Args>
	struct function_traits<Ret(C::*)(Args...) const> : detail::basic_function_traits<Ret(Args...)>
	{
		using Type                      = Ret(C::*)(Args...) const;
		using Class                     = C;
		using ParamsWithClass           = std::tuple<C*, Args...>;
		using Pointer                   = Ret(C::*)(Args...) const;
		static constexpr bool is_member = true;
		static constexpr bool is_const  = true;
	};

}