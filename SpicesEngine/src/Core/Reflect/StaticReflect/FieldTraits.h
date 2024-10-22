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
		constexpr field_traits(T&& p, std::string n) 
			: pointer{ p }
			, name(n)
		{
			size_t pos = n.find_last_of(":");
			if (pos < n.length())
			{
				name = n.substr(pos);
			}
		}

		T pointer;
		std::string name;
	};

}