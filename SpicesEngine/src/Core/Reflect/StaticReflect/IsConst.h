/**
* @file IsConst.h.
* @brief The IsConst Class Definitions and Implementation.
* @author Spices.
*/

#pragma once
#include "RemovePointer.h"
#include "IsPointer.h"

namespace Spices {

	namespace detail {

		/**
		* @brief Declare of is_const.
		*/
		template<typename>
		struct is_const
		{
			static constexpr bool value = false;
		};

		/**
		* @brief is_const with const.
		* @tparam T specific Type.
		*/
		template<typename T>
		struct is_const<const T>
		{
			static constexpr bool value = true;
		};

		/**
		* @brief is_const with const and class.
		* @tparam T specific Type.
		* @tparam C class.
		*/
		template<typename T, typename C>
		struct is_const<const T C::*>
		{
			static constexpr bool value = true;
		};

		/**
		* @brief is_const remove pointer from type.
		* @tparam T type.
		*/
		template<typename T>
		struct basic_pointer_is_const
		{
			static constexpr bool value = is_const<remove_pointer_t<T>>::value;
		};

		/**
		* @brief is_const remove reference(both pointer) from type.
		* For reference and pointer can be mixed together.
		* @tparam T type.
		*/
		template<typename T>
		struct basic_reference_is_const
		{
			static constexpr bool value = is_const<remove_pointer_t<std::remove_reference_t<T>>>::value;
		};

		/**
		* @brief Declare of basic_is_const.
		*/
		template<typename, bool>
		struct basic_is_const;

		/**
		* @brief basic_is_const with true.
		* @tparam T type.
		*/
		template<typename T>
		struct basic_is_const<T, true> : public basic_pointer_is_const<T> {};

		/**
		* @brief basic_is_const with false.
		* @tparam T type.
		*/
		template<typename T>
		struct basic_is_const<T, false> : public basic_reference_is_const<T> {};

	}
	
	/**
	* @brief is_const process pointer or reference.
	* @tparam T type.
	*/
	template<typename T>
	struct is_const : detail::basic_is_const<T, is_pointer_v<T>> {};

	/**
	* @brief value of is_const.
	* @tparam T specific Type.
	*/
	template<typename T>
	constexpr bool is_const_v = is_const<T>::value;
}