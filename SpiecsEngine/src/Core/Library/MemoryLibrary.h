#pragma once
#include "Core/Core.h"

#include <stddef.h>

namespace Spiecs {

	class MemoryLibrary
	{
	public:

		template <class integral>
		static constexpr bool is_aligned(integral x, size_t a) noexcept;

		template <class integral>
		static constexpr integral align_up(integral x, size_t a) noexcept;

		template <class integral>
		static constexpr integral align_down(integral x, size_t a) noexcept;
	};

	template<class integral>
	inline constexpr bool MemoryLibrary::is_aligned(integral x, size_t a) noexcept
	{
		return (x & (integral(a) - 1)) == 0;
	}

	template<class integral>
	inline constexpr integral MemoryLibrary::align_up(integral x, size_t a) noexcept
	{
		return integral((x + (integral(a) - 1)) & ~integral(a - 1));
	}

	template<class integral>
	inline constexpr integral MemoryLibrary::align_down(integral x, size_t a) noexcept
	{
		return integral(x & ~integral(a - 1));
	}
}