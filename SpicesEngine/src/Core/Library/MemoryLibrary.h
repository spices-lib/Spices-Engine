/**
* @file MemoryLibrary.h
* @brief The MemoryLibrary Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"

#include <stddef.h>

namespace Spices {

	/**
    * @brief Memory Static Function Library.
    */
	class MemoryLibrary
	{
	public:

		/**
		* @brief Determine is memory size aligned with specific value.
		* @param[in] x memory size.
		* @param[in] a aligned size.
		* @return Returns true if is aligned.
		*/
		template <class integral>
		static constexpr bool is_aligned(integral x, size_t a) noexcept;

		/**
		* @brief Align up a memory size aligned with specific value.
		* @param[in] x memory size.
		* @param[in] a aligned size.
		*/
		template <class integral>
		static constexpr integral align_up(integral x, size_t a) noexcept;

		/**
		* @brief Align down a memory size aligned with specific value.
		* @param[in] x memory size.
		* @param[in] a aligned size.
		*/
		template <class integral>
		static constexpr integral align_down(integral x, size_t a) noexcept;
	};

	template<class integral>
	inline constexpr bool MemoryLibrary::is_aligned(integral x, size_t a) noexcept
	{
		SPICES_PROFILE_ZONE;

		return (x & (integral(a) - 1)) == 0;
	}

	template<class integral>
	inline constexpr integral MemoryLibrary::align_up(integral x, size_t a) noexcept
	{
		SPICES_PROFILE_ZONE;

		return integral((x + (integral(a) - 1)) & ~integral(a - 1));
	}

	template<class integral>
	inline constexpr integral MemoryLibrary::align_down(integral x, size_t a) noexcept
	{
		SPICES_PROFILE_ZONE;

		return integral(x & ~integral(a - 1));
	}
}