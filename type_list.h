#ifndef  TYPE_LIST_H_
#define  TYPE_LIST_H_

#include <type_traits>
#include <concepts>

template<typename ...Ts>
struct TypeList {
    struct IsTypeList {
    }; // 标记，用于定义concept
    using type = TypeList; // 约定type输出类型
    constexpr static size_t size = sizeof...(Ts); // 列表长度

    template<typename ...T> using append = TypeList<Ts..., T...>;
    template<typename ...T> using prepend = TypeList<T..., Ts...>;
    template<template<typename ...> typename T> using to = T<Ts...>;
};

template<typename TypeList>
concept TL = requires {
    typename TypeList::IsTypeList;
    typename TypeList::type;
};








#endif // TYPE_LIST_H_