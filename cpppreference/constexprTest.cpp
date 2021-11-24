//
// Created by william on 2021/1/20.
//
/**
 *  constexpr - 指定变量或函数的值可在常量表达式中出现
 *  解释:
 *  constexpr 说明符声明 可以在编译时求得函数或变量的值。
 *  然后这些变量和函数（若给定了合适的函数实参）即可用于仅允许编译时常量表达式之处。
 *  用于对象或非静态成员函数 (C++14 前)声明的 constexpr说明符蕴含const。
 *  用于函数声明的 constexpr 说明符或 static 成员变量 (C++17 起)蕴含 inline。
 *  若函数或函数模板的任何声明拥有 constexpr 说明符，则每个声明必须都含有该说明符。
 *  constexpr 变量必须满足下列要求：
 *  其类型必须是字面类型 (LiteralType) 。
 *  它必须被立即初始化
 *  其初始化的全表达式，包括所有隐式转换、构造函数调用等，都必须是常量表达式
 *  它必须拥有常量析构，即：
 *      它既非类类型亦非其（可能多维的）数组，或、
 *      它是类类型或其（可能多维的）数组，该类类型拥有 constexpr 析构函数，而对于仅有的作用为销毁该对象的虚设表达式 e ，
 *      假如认为该对象与其非 mutable 子对象（但非其 mutable 子对象）的生存期始于 e 内，则 e 会是核心常量表达式。(C++20)
 *  若 constexpr 变量非翻译单元局部，则它不应被初始化为指向或指代可用于常量表达式的翻译单元局部实体，
 *  或拥有指向或指代这种实体的（可能递归的）子对象。
 *  这种初始化在模块接口单元（在其私有模块片段外，若有）或模块划分中被禁止，而在任何其他语境中被弃用。  (C++20)
 *  constexpr 函数必须满足下列要求：
 *      它必须非虚(C++20 前)
 *      它必须不是协程(C++20 起)
 *      其返回类型（若存在）必须是字面类型 (LiteralType)
 *      其每个参数都必须是字面类型 (LiteralType)
 *      对于构造函数与析构函数 (C++20 起)，该类必须无虚基类
 *      至少存在一组实参值，使得函数的一个调用为核心常量表达式的被求值的子表达式（对于构造函数为足以用于常量初始化器） (C++14 起)。不要求对这点的诊断。
 *      其函数体必须不是函数 try 块(C++20 前)
 *      函数体必须被弃置或预置，或只含有下列内容：
 *          空语句（仅分号）(C++14前）
 *          static_assert 声明
 *          不定义类或枚举的 typedef 声明及别名声明
 *          using 声明
 *          using 指令
 *          恰好一条 return 语句，若函数不是构造函数。
 *      函数体必须不含：
 *          goto 语句
 *          拥有除 case 和 default 之外的标号的语句
 *  析构函数不能为 constexpr，但能在常量表达式中隐式调用平凡析构函数。
 *
 */

/// 因为 noexcept 运算符始终对常量表达式返回 true，故它可用于检查具体特定的 constexpr 函数调用是否采用常量表达式分支：
constexpr int f1();
constexpr bool b1 = noexcept(f1()); // false，constexpr 函数未定义
constexpr int f()
{
    return 0;
}
constexpr bool b2 = noexcept(f()); // true，f() 是常量表达式

// constexpr 构造函数允许用于非字面类型的类。例如，std::unique_ptr 的默认构造函数是 constexpr，允许常量初始化。引用变量可声明为 constexpr（其初始化器必须是引用常量表达式）：

static constexpr int const& x = 42;
// 到 const int 对象的 constexpr 引用
// （该对象拥有静态存储期，因为static引用续命）

// 尽管在 constexpr 函数中允许 try 块与内联汇编，常量表达式中仍然不允许抛异常或执行汇编。
//
//若变量拥有常量析构，则无需为调用其析构函数而生成机器码，即使其析构函数非平凡。

#include <iostream>
#include <stdexcept>

// C++11 constexpr函数使用递归而非迭代
// （C++14 constexpr函数可使用局部变量和循环）
constexpr int factorial(int n)
{
    return n <= 1 ? 1 : (n * factorial(n - 1));
}

// 字面类
class Conststr
{
    const char* p;
    std::size_t sz;

public:
    template <std::size_t N>
    constexpr Conststr(const char (&a)[N]) :
        p(a), sz(N - 1) {}
    // constexpr 函数通过抛异常来提示错误
    // C++11 中，它们必须用条件运算符 ?: 这么做
    constexpr char operator[](std::size_t n) const
    {
        return n < sz ? p[n] : throw std::out_of_range("");
    }
    constexpr std::size_t size() const { return sz; }
};

// C++11 constexpr 函数必须把一切放在单条 return 语句中
// （C++14 无此要求）
constexpr std::size_t countlower(Conststr s, std::size_t n = 0, std::size_t c = 0)
{
    return n == s.size() ? c : 'a' <= s[n] && s[n] < 'z' ? countlower(s, n + 1, c + 1) :
                                                           countlower(s, n + 1, c);
}
// 输出要求编译时常量的函数，用于测试
template <int n>
struct constN
{
    constN() { std::cout << n << std::endl; }
};


void constexprTest()
{
    std::cout << "4!=";
    constN<factorial(4)> out1; // 在编译时计算
    volatile int k = 8; // 不允许使用 volatile 者优化
    std::cout << k << "!=" << factorial(k) << '\n';

    std::cout <<  "the number of lowercase letters in \"Hello, world!\" is ";
    constN<countlower("Hello, world!")> out2; // 隐式转换到 conststr
}