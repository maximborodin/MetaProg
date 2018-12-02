#pragma once

template<typename ...List>
struct TypeList {};

template<typename Head, typename ...Tail>
struct TypeList<Head, Tail...> {};

typedef TypeList<> EmptyTypeList;
