/**
* @file TypeList.h.
* @brief The TypeList Class Definitions and Implementation.
* @author VisualGMQ.
*/

#pragma once

#include <type_traits>
#include <tuple>

namespace Spices {

	/**
	* @brief Container of store Types.
	* @tparam Args Types.
	*/
	template<typename ...Args>
	struct type_list
	{
		static constexpr size_t size = sizeof...(Args);
	};

	namespace detail {
	
		/*************************************************************************/

		/**
		* @brief Declare of head.
		*/
		template<typename>
		struct head;

		/**
		* @brief Implementation of head.
		* Get first element of type_list.
		* @tparam T First element type.
		* @tparam Remains Remains of other elements.
		*/
		template<typename T, typename ...Remains>
		struct head<type_list<T, Remains...>>
		{
			using type = T;
		};

		/*************************************************************************/

		/**
		* @brief Declare of tail.
		*/
		template<typename>
		struct tail;

		/**
		* @brief Implementation of tail.
		* Get elements of type_list except the first.
		* @tparam T First element type.
		* @tparam Remains Remains of other elements.
		*/
		template<typename T, typename ...Remains>
		struct tail<type_list<T, Remains...>>
		{
			using type = type_list<Remains...>;
		};

		/*************************************************************************/

		/**
		* @brief Declare of nth.
		*/
		template<typename, size_t>
		struct nth;

		/**
		* @brief Implementation of nth(end condition).
		* Get element of type_list by index 0.
		* @tparam T First element type.
		* @tparam Remains Remains of other elements.
		*/
		template<typename T, typename ...Remains>
		struct nth<type_list<T, Remains...>, 0>
		{
			using type = T;
		};

		/**
		* @brief Implementation of nth.
		* Get element of type_list by specific index.
		* @tparam T First element type.
		* @tparam Remains Remains of other elements.
		* @tparam N Specific index of type_list.
		*/
		template<typename T, typename ...Remains, size_t N>
		struct nth<type_list<T, Remains...>, N>
		{
			using type = typename nth<type_list<Remains...>, N - 1>::type;
		};

		/*************************************************************************/

		/**
		* @brief Declare of count.
		*/
		template<typename, template<typename> typename, size_t N>
		struct count;

		/**
		* @brief Implementation of count(end condition).
		* Calculate count that meets requeirment.
		* @tparam T First element type.
		* @tparam Remains Remains of other elements.
		* @tparam F Specific requeirment function.
		*/
		template<typename T, typename ...Remains, template<typename> typename F>
		struct count<type_list<T, Remains...>, F, 0>
		{
			static constexpr int value = F<T>::value ? 1 : 0;
		};

		/**
		* @brief Implementation of count.
		* Calculate count that meets requeirment.
		* @tparam T First element type.
		* @tparam Remains Remains of other elements.
		* @tparam F Specific requeirment function.
		* @tparam N Specific index of type_list.
		*/
		template<typename T, typename ...Remains, template<typename> typename F, size_t N>
		struct count<type_list<T, Remains...>, F, N>
		{
			static constexpr int value = (F<T>::value ? 1 : 0) + count<type_list<Remains...>, F, N - 1>::value;
		};

		/*************************************************************************/

		/**
		* @brief Declare of map.
		*/
		template<typename, template<typename> typename>
		struct map;

		/**
		* @brief Implementation of map.
		* Replace type with a new type.
		* @tparam Args All elements.
		* @tparam F Specific map function.
		*/
		template<typename ...Args, template<typename> typename F>
		struct map<type_list<Args...>, F>
		{
			using type = type_list<typename F<Args>::type...>;
		};

		/*************************************************************************/

		/**
		* @brief Declare of cons.
		*/
		template<typename, typename>
		struct cons;

		/**
		* @brief Implementation of cons.
		* Psuh a param to params first.
		* @tparam Args All elements.
		* @tparam T Added element.
		*/
		template<typename T, typename ...Args>
		struct cons<T, type_list<Args...>>
		{
			using type = type_list<T, Args...>;
		};

		/*************************************************************************/

		/**
		* @brief Declare of concat.
		*/
		template<typename, typename>
		struct concat;

		/**
		* @brief Implementation of concat.
		* Combine two type_list
		* @tparam Args1 All elements of type_list1.
		* @tparam Args2 All elements of type_list2.
		*/
		template<typename ...Args1, typename ...Args2>
		struct concat<type_list<Args1...>, type_list<Args2...>>
		{
			using type = type_list<Args1..., Args2...>;
		};

		/*************************************************************************/

		/**
		* @brief Declare of init.
		*/
		template<typename>
		struct init;

		/**
		* @brief Implementation of init(end condition).
		* Keep params except last.
		* @tparam T first element.
		*/
		template<typename T>
		struct init<type_list<T>>
		{
			using type = type_list<>;
		};

		/**
		* @brief Implementation of init.
		* Keep params except last.
		* @tparam T first element.
		* @tparam Remains other elements except first.
		*/
		template<typename T, typename ...Remains>
		struct init<type_list<T, Remains...>>
		{
			using type = typename cons<T, typename init<type_list<Remains...>>::type>::type;
		};

		/*************************************************************************/

		/**
		* @brief Declare of filter.
		*/
		template<typename, template<typename> typename>
		struct filter;

		/**
		* @brief Implementation of filter(end condition).
		* Keep elements except last.
		* @tparam T first element.
		*/
		template<template<typename> typename F>
		struct filter<type_list<>, F>
		{
			using type = type_list<>;
		};

		/**
		* @brief Implementation of filter.
		* Keep elements except filtered.
		* @tparam T first element.
		* @tparam Remains other elements except first.
		* @tparam F filter function.
		*/
		template<typename T, typename ...Remains, template<typename> typename F>
		struct filter<type_list<T, Remains...>, F>
		{
			using type = std::conditional_t <F<T>::value,
				typename cons<T, typename filter<type_list<Remains...>, F>::type>::type,
				typename filter<type_list<Remains...>, F>::type
			>;
		};
	}

	/**
	* @brief Get first element of type_list.
	* @tparam TypeList type_list.
	*/
	template<typename TypeList>
	using head = typename detail::head<TypeList>::type;

	/**
	* @brief Get elements of type_list except the first.
	* @tparam TypeList type_list.
	*/
	template<typename TypeList>
	using tail = typename detail::tail<TypeList>::type;

	/**
	* @brief Get element of type_list by specific index.
	* @tparam TypeList type_list.
	* @tparam N Specific index of type_list.
	*/
	template<typename TypeList, size_t N>
	using nth = typename detail::nth<TypeList, N>::type;

	/**
	* @brief Calculate count that meets requeirment.
	* @tparam TypeList type_list.
	* @tparam F count function.
	*/
	template<typename TypeList, template<typename> typename F>
	constexpr int count = detail::count<TypeList, F, TypeList::size - 1>::value;

	/**
	* @brief Replace type with a new type.
	* @tparam TypeList type_list.
	* @tparam F map function.
	*/
	template<typename TypeList, template<typename> typename F>
	using map = typename detail::map<TypeList, F>::type;

	/**
	* @brief Psuh a element to elements first.
	* @tparam TypeList type_list.
	* @tparam T added Type.
	*/
	template<typename T, typename TypeList>
	using cons = typename detail::cons<T, TypeList>::type;

	/**
	* @brief Combine two type_list
	* @tparam TypeList1 first type_list.
	* @tparam TypeList2 second type_list.
	*/
	template<typename TypeList1, typename TypeList2>
	using concat = typename detail::concat<TypeList1, TypeList2>::type;

	/**
	* @brief Keep elements except last.
	* @tparam TypeList type_list.
	*/
	template<typename TypeList>
	using init = typename detail::init<TypeList>::type;

	/**
	* @brief Keep elements except filtered.
	* @tparam TypeList type_list.
	* @tparam F filter function.
	*/
	template<typename TypeList, template<typename> typename F>
	using filter = typename detail::filter<TypeList, F>::type;

}