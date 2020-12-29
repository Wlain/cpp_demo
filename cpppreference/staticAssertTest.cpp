//
// Created by william on 2020/12/28.
//
/*
 * Syntax：语法：
 * static_assert ( bool_constexpr , message ) C++11
 * static_assert ( bool_constexpr ) C++17
 * Explanation：解释
 * 声明可以出现在命名空间和块作用域中，可以作为块声明，也可以做为成员声明
 */
#include <type_traits>

template <typename T>
void swap(T& a, T& b)
{
    static_assert(std::is_copy_constructible<T>::value, "Swap requires copying");
    static_assert(std::is_nothrow_copy_constructible<T>::value && std::is_nothrow_copy_assignable<T>::value, "Swap requires nothrow copy/assign");
    auto c = b;
    b = a;
    a = c;
}

template <typename T>
struct dataStructure
{
    static_assert(std::is_default_constructible<T>::value, "Data Structure requires default-constructible elements");
};

struct NoCopy
{
    NoCopy(const NoCopy&) = delete;
    NoCopy() = default;
};

struct NoDefault
{
    NoDefault() = delete;
};

void staticAssertTest()
{
    int a, b;
    swap(a, b);
    NoCopy ncA, ncB;
//    swap(ncA, ncB);
    dataStructure<int> dsOK;
//    dataStructure<NoDefault> dsError;
}