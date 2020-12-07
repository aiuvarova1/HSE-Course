#pragma once

#include "erase.h"
#include "typelist.h"

template<typename TList>
struct NoDuplicates;

template<>
struct NoDuplicates<NullType> {
    typedef NullType NewTypeList;
};

template<typename Head, typename Tail>
struct NoDuplicates<TypeList<Head, Tail>> {
private:
    typedef typename NoDuplicates<Tail>::NewTypeList tail;
    typedef typename Erase<tail, Head>::NewTypeList erased;
public:
    typedef TypeList<Head, erased> NewTypeList;
};