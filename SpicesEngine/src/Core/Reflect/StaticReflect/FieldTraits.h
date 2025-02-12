/**
* @file FieldTraits.h.
* @brief The FieldTraits Class Definitions and Implementation.
* @author VisualGMQ.
*/

#pragma once

#include "FunctionTraits.h"
#include "VariableTraits.h"

#include <type_traits>

namespace Spices {

	/**
	* @brief Declare of Field Traits template.
	* @tparam T Specific type.
	* @tparam isFunc True if T is a function. 
	*/
	template<typename T, bool isFunc>
	struct basic_field_traits;

	/**
	* @brief Field Traits template (Function Traits).
	* @tparam T Specific type.
	*/
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

		constexpr size_t param_count() const
		{
			return traits::Params::size;
		}
	};

	/**
	* @brief Field Traits template (Variable Traits).
	* @tparam T Specific type.
	*/
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
	};

	/**
	* @brief Field Traits template.
	* @tparam T Specific type.
	*/
	template<typename T>
	struct field_traits : public basic_field_traits<T, is_function_v<T>>
	{
		constexpr field_traits(T&& p, std::string_view n, size_t offset = 0)
			: name(n)
			, pointer{ p }
			, offset(offset)
		{
			size_t pos = n.find_last_of(":");
			if (pos < n.length())
			{
				name = n.substr(pos + 1);
			}
		}

		/**
		* @brief Field Name.
		*/
		std::string_view name;

		/**
		* @brief Field pointer.
		*/
		T pointer;

		/**
		* @brief Field offset.
		*/
		size_t offset;
	};

}