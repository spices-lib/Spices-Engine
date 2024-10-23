/**
* @file IsPointer.h.
* @brief The IsPointer Class Definitions and Implementation.
* @author Spices.
*/

#pragma once

namespace Spices {

	template <class>
	constexpr bool is_pointer_v = false;

	template <class _Ty>
	constexpr bool is_pointer_v<_Ty*> = true;

	template <class _Ty>
	constexpr bool is_pointer_v<_Ty* const> = true;

	template <class _Ty>
	constexpr bool is_pointer_v<_Ty* volatile> = true;

	template <class _Ty>
	constexpr bool is_pointer_v<_Ty* const volatile> = true;


	template <class _Ty, class _C>
	constexpr bool is_pointer_v<_Ty* _C::*> = true;

	template <class _Ty, class _C>
	constexpr bool is_pointer_v<_Ty* const _C::*> = true;

	template <class _Ty, class _C>
	constexpr bool is_pointer_v<_Ty* volatile _C::*> = true;

	template <class _Ty, class _C>
	constexpr bool is_pointer_v<_Ty* const volatile _C::*> = true;

}