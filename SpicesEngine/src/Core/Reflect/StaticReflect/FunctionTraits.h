/**
* @file FunctionTraits.h.
* @brief The FunctionTraits Class Definitions and Implementation.
* @author VisualGMQ.
*/

#pragma once
#include <type_traits>
#include <tuple>

namespace Spices {

	namespace detail {

		/**
		* @brief Declare of basic_function_traits.
		*/
		template<typename>
		struct basic_function_traits;

		/**
		* @brief Implementation of basic_function_traits.
		* @tparam Ret Return type of function.
		* @tparam Args Params of function.
		*/
		template<typename Ret, typename ...Args>
		struct basic_function_traits<Ret(Args...)>
		{
			using Params     = std::tuple<Args...>;
			using ReturnType = Ret;
		};

		/**
		* @brief Determain if a given T is a function.
		* @tparam T .
		*/
		template<typename T>
		struct is_function
		{
			static constexpr bool value = std::is_function_v<std::remove_pointer_t<T>> || std::is_member_function_pointer_v<T>;
		};

		/**
		* @brief Get function pointer type.
		* @tparam Ret return valur type.
		* @tparam Params params type.
		*/
		template<typename Ret, typename ...Params>
		auto function_pointer_type(Ret(*)(Params...)) -> Ret(*)(Params...);

		/**
		* @brief Get class function pointer type.
		* @tparam Ret return valur type.
		* @tparam Class function class.
		* @tparam Params params type.
		*/
		template<typename Ret, typename Class, typename ...Params>
		auto function_pointer_type(Ret(Class::*)(Params...)) -> Ret(Class::*)(Params...);

		/**
		* @brief Get class const function pointer type.
		* @tparam Ret return valur type.
		* @tparam Class function class.
		* @tparam Params params type.
		*/
		template<typename Ret, typename Class, typename ...Params>
		auto function_pointer_type(Ret(Class::*)(Params...) const) -> Ret(Class::*)(Params...) const;
	}

	/**
	* @brief Declare of function_traits.
	*/
	template<typename>
	struct function_traits;

	/**
	* @brief Implementation of function_traits without class.
	* @tparam Ret Return type of function.
	* @tparam Args Params of function.
	*/
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

	/**
	* @brief Implementation of function_traits with class.
	* @tparam Ret Return type of function.
	* @tparam C class of function.
	* @tparam Args Params of function.
	*/
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

	/**
	* @brief Implementation of function_traits with class const.
	* @tparam Ret Return type of function.
	* @tparam C class of function.
	* @tparam Args Params of function.
	*/
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

	/**
	* @brief Determain if a given T is a function.
	* @tparam T .
	*/
	template<typename T>
	constexpr bool is_function_v = detail::is_function<T>::value;

	/**
	* @brief Get class const function pointer type.
	* @tparam F function.
	*/
	template<auto F>
	using function_pointer_type_t = decltype(detail::function_pointer_type(F));

	/**
	* @brief traits of function.
	* @tparam F function.
	*/
	template<auto F>
	using function_traits_t = function_traits<function_pointer_type_t<F>>;

}