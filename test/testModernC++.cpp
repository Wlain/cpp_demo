//
// Created by william on 2020/12/12.
//
#include <functional>
#include <iostream>

void testModern01()
{
    [out = std::ref(std::cout << "Result from C code")]() {
        out.get();
    };
}

template <typename T, typename S>
auto add2(T x, S s) -> decltype(x + s)
{
    return x + s;
}

template <typename T, typename S>
auto add3(T x, S s)
{
    return x + s;
}

std::string lookup1()
{
    return "";
}

std::string& lookup2()
{
    return (std::string&)"";
}

decltype(auto) lookup_1()
{
    return lookup1();
}

decltype(auto) lookup_2()
{
    return lookup2();
}

template <typename T>
auto printTypeInfo(const T& t)
{
    if constexpr (std::is_integral<T>::value)
    {
        return t + 1;
    }
    else
    {
        return t + 0.001;
    }
}
/*
 * 实际表现
    int print_type_info(const int& t) {
    return t + 1;
    }
    double print_type_info(const double& t) {
        return t + 0.001;
    }
 */

template <typename T, typename U>
class MagicType
{
public:
    T t;
    U y;
};
template <typename T>
using MagicTypePro = MagicType<std::vector<T>, std::string>;

template class MagicType<int, bool>;

template <auto value>
void func()
{
    std::cout << value << std::endl;
}

class Base
{
public:
    Base()
    {
        m_v1 = 1;
    }
    explicit Base(int value) :
        Base()
    {
        m_v2 = value;
    }
    virtual void foo() final;
public:
    int m_v1;
    int m_v2{};
};
void Base::foo()
{
}

class SubClass : public Base
{
public:
    using Base::Base; // 继承构造
};

void testModern02()
{
    // 1.使用nullptr
    // 2.使用类型推导decltype
    auto x = 1;
    auto y = 2;
    decltype(y + x) z;
    std::cout << z << std::endl;
    if (std::is_same<decltype(x), decltype(z)>::value)
    {
        std::cout << "type x == z" << std::endl;
    }
    // decltype(auto) C++ 中参数转发的概念
    // 3.if constexpr
    // 4.区间for迭代
    // 5.使用using
    typedef int (*ptr)(void*);
    using ptr2 = int (*)(void*);
    // 委托构造：构造函数可以在同一个类中一个构造函数调用另一个构造函数，从而达到简化代码的目的
    Base b(2);
    // 6.继承构造
    SubClass s(3);
    // final:则是为了防止类被继续继承以及终止虚函数继续重载引入的。
    // 7.使用lambda
    // [] 空捕获列表
    // [name1, name2, ...] 捕获一系列变量
    // [&] 引用捕获, 让编译器自行推导捕获列表
    // [=] 值捕获, 让编译器执行推导引用列表

}
