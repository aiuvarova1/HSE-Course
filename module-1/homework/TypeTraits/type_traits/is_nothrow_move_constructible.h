#pragma once

#include <type_traits>

#include "is_copy_constructible.h"
#include "utility.h"

//<IsConstructible, is_reference, T, Args...>
template <bool, bool, typename T, typename... Args>
struct LibCppIsNoThrowConstructible;

// LibCppIsNoThrowConstructible - partial specializations
template <bool IsReference, typename T, typename... Args>
struct LibCppIsNoThrowConstructible<false, IsReference, T, Args...> : public std::false_type {};

template <typename T, typename... Args>
struct LibCppIsNoThrowConstructible<true, false, T, Args...> : public std::false_type {};

template <typename T, typename... Args>
struct LibCppIsNoThrowConstructible<true, true, T, Args...>
    : public std::integral_constant<bool, noexcept(T(Declval<Args>()...))> {};

template <typename T, typename A>
struct LibCppIsNoThrowConstructible<true, true, T, A>
    : public std::integral_constant<bool, noexcept(static_cast<T>(Declval<A>()))> {};

// LibCppIsNoThrowConstructible - partial specializations

template <typename T, typename... Args>
struct IsNoThrowConstructible
    : LibCppIsNoThrowConstructible<IsConstructible<T, Args...>::value,
                                   std::is_object_v<T> || std::is_reference_v<T>, T, Args...> {};

template <typename T, std::size_t N>
struct IsNoThrowConstructible<T[N]>
    : LibCppIsNoThrowConstructible<IsConstructible<T>::value,
                                   std::is_object_v<T> || std::is_reference_v<T>, T> {};

template <typename T>
struct IsNoThrowMoveConstructible : IsNoThrowConstructible<T, T&&> {};