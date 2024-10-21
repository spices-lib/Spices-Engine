/**
* @file Tuple.h.
* @brief The Tuple Helper Functions Definitions and Implementation.
* @author VisualGMQ.
*/

#pragma once
#include <tuple>

namespace Spices {

	/**
	* @brief Iter a tuple.
	* @tparam Idx Size of typle.
	* @tparam Args tuple elements.
	* @tparam Class instance.
	*/
	template<size_t Idx, typename ...Args, typename Class>
	void IterTuple(const std::tuple<Args...>& tuple, Class* instance)
	{
		using tuple_type = std::tuple<Args...>;

		if constexpr (Idx >= std::tuple_size_v<tuple_type>)
		{
			return;
		}
		else
		{
			if constexpr (auto elem = std::get<Idx>(tuple); elem.param_count() = 0)
			{
				(instance->*elem.pointer)();

				IterTuple<Idx + 1>(tuple, instance);
			}
		}
	}

	/**
	* @brief Iter a tuple.
	* @tparam Idx Size of typle.
	* @tparam Tuple tuple.
	* @tparam Function Iter Function.
	*/
	template<size_t... Idx, typename Tuple, typename Function>
	void IterTuple(Tuple tuple, Function&& f, std::index_sequence<Idx...>)
	{
		(f(std::get<Idx>(tuple)), ...);
	}
}