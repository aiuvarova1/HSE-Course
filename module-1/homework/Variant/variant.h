#include <cstdlib>
#include <utility>

#pragma once

namespace task {

    //  ======== TypeList =============
    template<typename Head, typename... Tail>
    struct TypeList {
        Head head;
        typedef TypeList<Tail...> tail;
    };

    template<size_t Idx, typename List>
    struct TypeAt;

    template<typename Head, typename... Tail>
    struct TypeAt<0, TypeList<Head, Tail...>> {
        typedef Head type;
    };

    template<size_t Idx, typename Head, typename... Tail>
    struct TypeAt<Idx, TypeList<Head, Tail...>> {
        typedef typename TypeAt<Idx - 1, TypeList<Tail...>>::type type;
    };


    //  ======== TypeUnion =============
    template<size_t Idx, typename... Types>
    union UnionByType;

    template<size_t Idx>
    union UnionByType<Idx> {
    };

    template<size_t Idx, typename T, typename... Types>
    union UnionByType<Idx, T, Types...> {
    private:
        friend struct TypeUnionUtils;
        T value_;
        UnionByType<Idx + 1, Types...> tail_;
    public:
        UnionByType() : tail_() {
            if (!std::is_trivially_constructible_v<T>) {
                value_ = T();
            }
        }

        ~UnionByType() {
            if (!std::is_trivially_destructible_v<T>) {
                value_.~T();
            }
        }
    };

    struct TypeUnionUtils {
    public:
        template<size_t Idx, typename T>
        static constexpr auto&& Get(T&& value, std::in_place_index_t<0>) {
            return std::forward<T>(value).value_;
        }

        template<size_t Idx, typename T>
        static constexpr auto&& Get(T&& value, std::in_place_index_t<Idx>) {
            return Get<Idx - 1>(std::forward<T>(value).tail_, std::in_place_index<Idx - 1>);
        }

        template<size_t Idx, std::size_t TypeIdx, typename T, typename Head, typename... Tail>
        static void Set(T&& value, std::in_place_index_t<0>, UnionByType<TypeIdx, Head, Tail...>& union_by_type) {
            if (std::is_same_v<T, Head> || std::is_convertible_v<T, Head>) {
                union_by_type.value_ = value;
            }
        }

        template<size_t Idx, std::size_t TypeIdx, typename T, typename Head, typename... Tail>
        static void Set(T&& value, std::in_place_index_t<Idx>, UnionByType<TypeIdx, Head, Tail...>& union_by_type) {
            Set<Idx - 1>(std::forward<T>(value), std::in_place_index<Idx - 1>, union_by_type.tail_);
        }
    };


    //  ======== Search =============

    template<std::size_t S>
    constexpr std::size_t PositionOf(const bool (& is_child)[S], std::size_t idx = 0) {
        if (idx >= S) {
            return -1;
        }
        return is_child[idx] ? idx : PositionOf(is_child, idx + 1);
    }

    template<typename T, typename... Types>
    struct Search {
        constexpr static bool kIsExactType[sizeof...(Types)] = {std::is_same<T, Types>::value...};
        constexpr static bool kIsConvertibleType[sizeof...(Types)] = {std::is_convertible<T, Types>::value...};
        constexpr static std::size_t kExactIdx = PositionOf(kIsExactType);
        constexpr static std::size_t kConvertibleIdx = kExactIdx != -1 ? kExactIdx : PositionOf(kIsConvertibleType);
    };


//  ======== Variant =============

    template<typename... Types>
    class Variant;

    template<size_t Idx, typename T>
    struct VariantAlternative;

    template<size_t Idx, typename T>
    using variant_alternative_t = typename VariantAlternative<Idx, T>::type;

    template<size_t Idx, typename... Types>
    struct VariantAlternative<Idx, Variant<Types...>> {
        using type = typename TypeAt<Idx, TypeList<Types...>>::type;
    };

    template<typename... Types>
    class Variant {
    public:
        friend struct VariantUtils;

        // Special member functions
        constexpr Variant() noexcept {};

        template<typename T>
        Variant& operator=(T&& t) noexcept;

    private:
        UnionByType<0, Types...> union_by_type_;
    };

    struct VariantUtils {
        template<typename T, typename... Types>
        static constexpr auto&& Get(Variant<Types...>&& v) {
            const size_t idx = Search<T, Types...>::kExactIdx;
            return TypeUnionUtils::Get<idx>(std::forward<Variant<Types...>>(v).union_by_type_,
                                            std::in_place_index<idx>);
        }
    };

    template<typename... Types>
    template<typename T>
    Variant<Types...>& Variant<Types...>::operator=(T&& t) noexcept {
        const std::size_t idx = Search<T, Types...>::kConvertibleIdx;
        assert(idx != (size_t) -1);
        TypeUnionUtils::Set<idx>(t, std::in_place_index<idx>, this->union_by_type_);

        return *this;
    }

// Non-member functions
    template<size_t I, typename... Types>
    constexpr const variant_alternative_t<I, Variant<Types...>>& Get(Variant<Types...>& v) {
        return VariantUtils::Get<typename TypeAt<I, TypeList<Types...>>::type>(std::forward<Variant<Types...>>(v));
    }

    template<size_t I, typename... Types>
    constexpr variant_alternative_t<I, Variant<Types...>>&& Get(Variant<Types...>&& v) {
        return VariantUtils::Get<typename TypeAt<I, TypeList<Types...>>::type>(std::move(v));
    }

    template<typename T, typename... Types>
    constexpr const T& Get(Variant<Types...>& v) {
        return VariantUtils::Get<T>(std::forward<Variant<Types...>>(v));
    }

    template<typename T, typename... Types>
    constexpr T&& Get(Variant<Types...>&& v) {
        return VariantUtils::Get<T>(std::move(v));
    }

};  // namespace task