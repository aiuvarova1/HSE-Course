#pragma once

#include <type_traits>
#include <utility>

#include "utility.h"

template <typename T, typename... Args>
struct LibCppIsConstructible;

template <typename Derived, typename Base>
struct IsInvalidBaseToDerivedCast {
    using base = Uncvref<Base>;
    using derived = Uncvref<Derived>;

    using type = typename std::integral_constant<
        bool, !std::is_same_v<base, derived> && std::is_base_of_v<base, derived> &&
                  !LibCppIsConstructible<derived, Base>::value::value>;
    static constexpr bool kValue = type::value;
};

template <typename To, typename From>
struct IsInvalidLvalueToRvalueCast : std::false_type {};

template <typename RefTo, typename RefFrom>
struct IsInvalidLvalueToRvalueCast<RefTo&&, RefFrom&> {
    using to = Uncvref<RefTo>;
    using from = Uncvref<RefFrom>;

    using type = typename std::integral_constant<bool, std::is_same_v<from, to> ||
                                                           std::is_base_of_v<to, from>>;
};

struct IsConstructibleHelper {
    template <typename T, typename... Args, typename C = decltype(T(Declval<Args>()...))>
    static std::true_type NConstructible(int){};

    template <typename T, typename... Args>
    static std::false_type NConstructible(...){};

    template <typename T, typename A, typename C = decltype(::new T(Declval<A>()))>
    static std::is_destructible<T> DirectConstructible(int){};

    template <typename T, typename A>
    static std::false_type DirectConstructible(...){};

    template <typename T, typename A, typename C = decltype(static_cast<T>(Declval<A>()))>
    static std::integral_constant<bool, !(IsInvalidLvalueToRvalueCast<T, A>::type::value) &&
                                            !(IsInvalidBaseToDerivedCast<A, T>::kValue)>

    ReferenceConstructible(int){};

    template <typename T, typename A>
    static std::false_type ReferenceConstructible(...){};
};

// LibCppIsConstructible - partial specializations
template <typename T, typename... Args>
struct LibCppIsConstructible {
    using value = decltype(IsConstructibleHelper::NConstructible<T, Args...>(0));
};

template <typename T, typename A>
struct LibCppIsConstructible<T, A> {
    using value = decltype(IsConstructibleHelper::DirectConstructible<T, A>(0));
};

template <typename T, typename A>
struct LibCppIsConstructible<T&, A> {
    using value = decltype(IsConstructibleHelper::ReferenceConstructible<T&, A>(0));
};

template <typename T, typename A>
struct LibCppIsConstructible<T&&, A> {
    using value = decltype(IsConstructibleHelper::ReferenceConstructible<T&&, A>(0));
};

// LibCppIsConstructible - partial specializations

template <typename T, typename... Args>
struct IsConstructible : public LibCppIsConstructible<T, Args...>::value {};

template <typename T>
struct IsCopyConstructible : public LibCppIsConstructible<T, const T&>::value {};