//
// Created by cwb on 2022/8/19.
//

#include <cassert>
#include <string>
#include <variant>
// 类模板 std::variant 表示一个类型安全的联合体,std::variant 的一个实例在任意时刻要么保有其一个可选类型之一的值，要么在错误情况下无值
// variant 不容许保有引用、数组，或类型 void

void variantTest()
{
    std::variant<int, float> v, w;
    v = 12; // v含有int
    int i = std::get<int>(v);
    w = std::get<int>(v);
    w = std::get<0>(v); // 与前一行效果相同
    w = v;              // 与前一行效果相同
    try
    {
        std::get<float>(w); // w 含 int 而非 float ：将抛出
    }
    catch (const std::bad_variant_access&)
    {
    }
    using namespace std::literals;
    std::variant<std::string> x("abc");              // 转换构造函数在无歧义时起作用
    x = "def";                                       // 转换赋值在无歧义时亦起作用
    std::variant<std::string, void const*> y("abc"); // 传递 char const * 时转换成 void const *
    // holds_alternative:若 variant 当前保有可选项 T 则为 true ，否则为 false
    assert(std::holds_alternative<void const*>(y)); // 成功
    y = "xyz"s;
    assert(std::holds_alternative<std::string>(y)); // 成功
}