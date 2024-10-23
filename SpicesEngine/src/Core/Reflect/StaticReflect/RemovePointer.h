/**
* @file RemovePointer.h.
* @brief The RemovePointer Class Definitions and Implementation.
* @author Spices.
*/

#pragma once

namespace Spices {

    template <class _Ty>
    struct remove_pointer {
        using type = _Ty;
    };

    template <class _Ty>
    struct remove_pointer<_Ty*> {
        using type = typename remove_pointer<_Ty>::type;
    };

    template <class _Ty>
    struct remove_pointer<_Ty* const> {
        using type = typename remove_pointer<_Ty const>::type;
    };

    template <class _Ty>
    struct remove_pointer<_Ty* volatile> {
        using type = typename remove_pointer<_Ty volatile>::type;
    };

    template <class _Ty>
    struct remove_pointer<_Ty* const volatile> {
        using type = typename remove_pointer<_Ty const volatile>::type;
    };

    template <class _Ty, class _C>
    struct remove_pointer<_Ty* _C::*> {
        using type = typename remove_pointer<_Ty>::type;
    };

    template <class _Ty, class _C>
    struct remove_pointer<_Ty* const _C::*> {
        using type = typename remove_pointer<_Ty const>::type;
    };

    template <class _Ty, class _C>
    struct remove_pointer<_Ty* volatile _C::*> {
        using type = typename remove_pointer<_Ty volatile>::type;
    };

    template <class _Ty, class _C>
    struct remove_pointer<_Ty* const volatile _C::*> {
        using type = typename remove_pointer<_Ty const volatile>::type;
    };

    template <class _Ty>
    using remove_pointer_t = typename remove_pointer<_Ty>::type;

}