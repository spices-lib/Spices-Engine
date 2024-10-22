/**
* @file VariableTraits.h.
* @brief The VariableTraits Class Definitions and Implementation.
* @author VisualGMQ.
*/

#pragma once
#include <type_traits>

namespace Spices {

	namespace detail {

		/**
		* @brief Store Non Member Variable Type.
		* @tparam T Variable Type.
		*/
		template<typename T>
		struct variable_type {
			using Type  = T;
			using Class = void;
		};

		/**
		* @brief Store Member Variable Type.
		* @tparam T Variable Type.
		* @tparam C Variable Class.
		*/
		template<typename C, typename T>
		struct variable_type<T C::*>
		{
			using Type  = T;
			using Class = C;
		};
	}

	/**
	* @brief Simplfy format of variable_type::Type.
	* @tparam T Variable Type with class.
	*/
	template<typename T>
	using variable_type_t = typename detail::variable_type<T>::Type;

	/**
	* @brief Simplfy format of variable_type::Class.
	* @tparam T Variable Type with class.
	*/
	template<typename T>
	using variable_class_t = typename detail::variable_type<T>::Class;

	namespace internal {

		/**
		* @brief Collection of Type and Class.
		* @tparam T Variable Type with class.
		*/
		template<typename T>
		struct basic_variable_traits
		{
			using Type  = variable_type_t<T>;
			using Class = variable_class_t<T>;
			static constexpr bool is_member = std::is_member_pointer_v<T>;
			static constexpr bool is_const  = std::is_const_v<T>;
		};
	}

	/**
	* @brief Declare of variable_traits.
	*/
	template<typename>
	struct variable_traits;

	/**
	* @brief Implementation of variable_traits without class.
	* @tparam T Variable Type.
	*/
	template<typename T>
	struct variable_traits<T*> : internal::basic_variable_traits<T>
	{
		using PointerType = T*;
	};

	/**
	* @brief Implementation of variable_traits with class.
	* @tparam T Variable Type.
	* @tparam C Variable Class.
	*/
	template<typename C, typename T>
	struct variable_traits<T C::*> : internal::basic_variable_traits<T C::*>
	{
		using PointerType = T C::*;
	};
}