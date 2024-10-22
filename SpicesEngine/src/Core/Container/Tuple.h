/**
* @file Tuple.h.
* @brief The Tuple Helper Functions Definitions and Implementation.
* @author VisualGMQ.
*/

#pragma once
#include <tuple>

namespace Spices {

	namespace detail {

		/**
		* @brief Iter a tuple.
		* @tparam Idx Size of typle.
		* @tparam Tuple tuple.
		* @tparam Function Iter Function.
		*/
		template<size_t... Idx, typename Tuple, typename Function>
		void IterTuple(Tuple& tuple, Function&& f, std::index_sequence<Idx...>)
		{
			(f(std::get<Idx>(tuple)), ...);
		}

	}

	/**
	* @brief Iter a tuple.
	* @tparam Tuple tuple.
	* @tparam Function Iter Function.
	*/
	template<typename Tuple, typename Function>
	void IterTuple(Tuple& tuple, Function&& f)
	{
		detail::IterTuple(tuple, f, std::make_index_sequence<std::tuple_size_v<Tuple>>());
	}
}