//
// Created by cwb on 2022/8/19.
//
#include <iomanip>
#include <iostream>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>
/*
 * 参数：
 * vis	-	接受每个 variant 的每个可能可选项的可调用 (Callable) 对象
 * vars	-	传递给观览器的 variant 列表
*/

// 应用观览器 vis 到 variant 变量组 vars,
// 要观览的 variant
using var_t = std::variant<int, long, double, std::string>;
// 观览器 #3 的辅助常量
template <class>
inline constexpr bool alwaysFalseV = false;
// 观览器 #4 的辅助类型
template <class... Ts>
struct Overloaded : Ts...
{
    using Ts::operator()...;
};
// 显式推导指引（ C++20 起不需要）
template <class... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;

void visitTest()
{
    std::vector<var_t> vec = { 10, 15l, 1.5, "hello" };
    for (auto&& v : vec)
    {
        // 1. void 观览器，仅为其副效应调用
        std::visit([](auto&& arg) { std::cout << arg; }, v);
        // 2. 返回值的观览器，返回另一 variant 的常见模式
        var_t w = std::visit([](auto&& arg) -> var_t { return arg + arg; }, v);
        std::cout << ". After doubling, variant holds ";
        // 3. 类型匹配观览器：亦能为带 4 个重载的 operator() 的类
        std::visit([](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, int>)
                std::cout << "int with value " << arg << '\n';
            else if constexpr (std::is_same_v<T, long>)
                std::cout << "long with value " << arg << '\n';
            else if constexpr (std::is_same_v<T, double>)
                std::cout << "double with value " << arg << '\n';
            else if constexpr (std::is_same_v<T, std::string>)
                std::cout << "std::string with value " << std::quoted(arg) << '\n';
            else
                static_assert(alwaysFalseV<T>, "non-exhaustive visitor!");
        },
                   w);
    }
    for (auto&& v : vec)
    {
        // 4. 另一种类型匹配观览器：有三个重载的 operator() 的类
        // 注：此情况下 '(auto arg)' 模板 operator() 将绑定到 'int' 与 'long' ，
        //    但若它不存在则 '(double arg)' operator() *亦将* 绑定到 'int' 与 'long' ，
        //    因为两者均可隐式转换成 double 。使用此形式时应留心以正确处理隐式转换。
        std::visit(Overloaded{
                       [](auto arg) { std::cout << arg << ' '; },
                       [](double arg) { std::cout << std::fixed << arg << ' '; },
                       [](const std::string& arg) { std::cout << std::quoted(arg) << ' '; },
                   },
                   v);
    }
}