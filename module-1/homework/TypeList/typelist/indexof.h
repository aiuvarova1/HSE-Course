#pragma once

#include "typelist.h"

template<typename TList, typename TargetType>
struct IndexOf;

template<typename TargetType>
struct IndexOf<NullType, TargetType> {
    static constexpr int pos = -1;
};

template<typename Tail, typename TargetType>
struct IndexOf<TypeList<TargetType, Tail>, TargetType> {
    static constexpr int pos = 0;
};

template<typename Head, typename Tail, typename TargetType>
struct IndexOf<TypeList<Head, Tail>, TargetType> {
private:
    static constexpr int tmp = IndexOf<Tail, TargetType>::pos;
public:
    static constexpr int pos = tmp == -1 ? -1 : 1 + tmp;
};