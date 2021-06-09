#pragma once

#include <type_traits>
#include <utility>

#include "is_copy_constructible.h"
#include "is_nothrow_move_constructible.h"
#include "utility.h"

template <bool condition, typename T, typename F>
struct Conditional {};

// Conditional - partial specialization
template <typename T, typename F>
struct Conditional<false, T, F> {
    using type = F;
};

template <typename T, typename F>
struct Conditional<true, T, F> {
    using type = T;
};
// Conditional - partial specialization

template <bool condition, typename T, typename F>
using conditional_v = typename Conditional<condition, T, F>::type;

// MoveIfNoExcept
template <typename T>
typename Conditional<!IsNoThrowMoveConstructible<T>::value && IsNoThrowConstructible<T>::value,
                     const T&, T&&>::type
MoveIfNoExcept(T& t) noexcept {
    return std::move(t);
}
// MoveIfNoExcept