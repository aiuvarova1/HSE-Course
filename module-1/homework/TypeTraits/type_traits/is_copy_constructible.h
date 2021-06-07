#pragma once

#include <type_traits>
#include <utility>

#include "utility.h"

template<typename T, typename... Args>
struct LibCppIsConstructible;

template<typename Derived, typename Base>
struct IsInvalidBaseToDerivedCast {
    typedef Uncvref<Base> base;
    typedef Uncvref<Derived> derived;

    typedef typename std::integral_constant<bool,
            !std::is_same_v<base, derived> &&
            std::is_base_of_v<base, derived> &&
            !LibCppIsConstructible<derived, Base>::value::value> type;
    static constexpr bool kValue = type::value;
};

template<typename To, typename From>
struct IsInvalidLvalueToRvalueCast : std::false_type {

};

template<typename RefTo, typename RefFrom>
struct IsInvalidLvalueToRvalueCast<RefTo&&, RefFrom&> {
    typedef Uncvref<RefTo> to;
    typedef Uncvref<RefFrom> from;

    typedef typename std::integral_constant<bool,
            std::is_same_v<from, to> ||
            std::is_base_of_v<to, from>> type;
};

struct IsConstructibleHelper {
    template<typename T, typename... Args, typename C = decltype(T(Declval<Args>()...))>
    static std::true_type NConstructible(int) {};

    template<typename T, typename... Args>
    static std::false_type NConstructible(...) {};

    template<typename T, typename A, typename C = decltype(::new T(Declval<A>()))>
    static std::is_destructible<T> DirectConstructible(int) {};

    template<typename T, typename A>
    static std::false_type DirectConstructible(...) {};

    template<typename T, typename A, typename C = decltype(static_cast<T>(Declval<A>()))>
    static std::integral_constant<bool,
            !(IsInvalidLvalueToRvalueCast<T, A>::type::value) &&
            !(IsInvalidBaseToDerivedCast<A, T>::kValue)>

    ReferenceConstructible(int) {};

    template<typename T, typename A>
    static std::false_type ReferenceConstructible(...) {};
};

// LibCppIsConstructible - partial specializations
template<typename T, typename... Args>
struct LibCppIsConstructible {
    typedef decltype(IsConstructibleHelper::NConstructible<T, Args...>(0)) value;

};

template<typename T, typename A>
struct LibCppIsConstructible<T, A> {
    typedef decltype(IsConstructibleHelper::DirectConstructible<T, A>(0)) value;

};

template<typename T, typename A>
struct LibCppIsConstructible<T&, A> {
    typedef decltype(IsConstructibleHelper::ReferenceConstructible<T&, A>(0)) value;
};

template<typename T, typename A>
struct LibCppIsConstructible<T&&, A> {
    typedef decltype(IsConstructibleHelper::ReferenceConstructible<T&&, A>(0)) value;

};

// LibCppIsConstructible - partial specializations

template<typename T, typename... Args>
struct IsConstructible : public LibCppIsConstructible<T, Args...>::value {
};

template<typename T>
struct IsCopyConstructible : public LibCppIsConstructible<T, const T&>::value {
};