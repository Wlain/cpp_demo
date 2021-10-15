//
// Created by william on 2020/12/29.
//


/**
 * 对于变量，指定要从其初始化器自动推导出其类型。
 * 对于函数，指定要从其 return 语句推导出其返回类型。(C++14 起)
 * 对于非类型模板形参，指定要从实参推导出其类型。(C++17 起)
 */

/**
 * 语法：
 * auto (C++11 起)
 * decltype(auto) C++14
 * 类型制约 auto C++20
 * 类型制约 decltype(auto) C++ 20
 */

/**
 * 类型制约	-	概念名，可以有限定，可以后随 <> 包围的模板实参列表
 * 1,3) 用模板实参推导的规则推导类型。
 * 2,4) 类型为 decltype(expr)，其中 expr 是初始化器。
 * 占位符 auto 可伴随如 const 或 & 这样的修饰符，它们参与类型推导。占位符 decltype(auto) 必须是被声明类型的唯一组分。 (C++14 起)
 */

#include "base.h"
#include <utility>

template <typename T, class U>
auto add(T t, U u) { return t + u;} // 返回类型是 operator+(T, U) 的类型

// 在其所调用的函数返回引用的情况下
// 函数调用的完美转发必须用 decltype(auto)
template<class F, class... Args>
decltype(auto) perfectFroward(F fun, Args&& ... args)
{
    return fun(std::forward<Args>()...);
}

template<auto n> // C++17 auto 形参声明
auto f() -> std::pair<decltype(n), decltype(n)> // auto 不能从花括号初始化器列表推导
{
    return {n, n};
}

void autoTest()
{
    auto a = 1 + 2; // a 的类型是 int
    auto b = add(1, 1.2); // b 的类型是 double
    static_assert(std::is_same_v<decltype(a), int>);
    static_assert(std::is_same_v<decltype(b), double>);

    auto c0 = a; // c0 的类型是 int，保有 a 的副本
    decltype(auto) c1 = a; //c1 的类型是 int，保有 a 的副本
    decltype(auto) c2 = (a); // c2 的类型是 int&，为 a 的别名
    std::cout << "a, before modification through c2 = " << a << '\n';
    ++c2;
    std::cout << "a, after modification through c2 = " << a << '\n';
    auto  [v, w] = f<0>(); // 结构化绑定声明
    auto d = {1, 2}; // OK：d 的类型是 std::initializer_list<int>
    auto n = {5}; // OK：n 的类型是 std::initializer_list<int>
    //  auto e{1, 2};    // C++17 起错误，之前为 std::initializer_list<int>
    auto m{5};       // OK：C++17 起 m 的类型为 int，之前为 initializer_list<int>
    //  decltype(auto) z = { 1, 2 } // 错误：{1, 2} 不是表达式
    // auto 常用于无名类型，例如 lambda 表达式的类型
    auto lambda = [](int x) { return x + 3; };


}