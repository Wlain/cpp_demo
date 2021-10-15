//
// Created by william on 2020/12/25.
//
#include "base.h"


// 声明具名变量为引用，即既存对象或函数的别名。
// 左值引用声明符 S& D;
// 右值引用声明符 S&& D;
// 引用必须被初始化为指代一个有效的对象或函数
// 不存在 void 的引用，也不存在引用的引用。
// 引用类型无法在顶层被 cv 限定；声明中没有为此而设的语法，而若将限定性添加到 typedef 名、 decltype 说明符或类型模板形参，则忽略它。
// 引用不是对象；它们不必占用存储，尽管若需要分配存储以实现所需语义（例如，引用类型的非静态数据成员通常会增加类的大小，量为存储内存地址所需），则编译器会这么做。
// 因为引用不是对象，故不存在引用的数组，不存在指向引用的指针，不存在引用的引用：

void f(int& x) {
    std::cout << "lvalue reference overload f(" << x << ")\n";
}

void f(const int& x) {
    std::cout << "lvalue reference to const overload f(" << x << ")\n";
}

void f(int&& x) {
    std::cout << "rvalue reference overload f(" << x << ")\n";
}


void referenceTest()
{
    // 容许通过模板或 typedef 中的类型操作构成引用的引用，这种情况下适用引用坍缩（reference collapsing）规则：右值引用的右值引用坍缩成右值引用，所有其他组合均坍缩成左值引用：
    typedef int&  lref;
    typedef int&& rref;
    int n;
    lref&  r1 = n; // r1 的类型是 int&
    lref&& r2 = n; // r2 的类型是 int&
    rref&  r3 = n; // r3 的类型是 int&
    rref&& r4 = 1; // r4 的类型是 int&&

    std::string s1 = "test";
    const std::string& r5 = s1; // okay：到 const 的左值引用延长生存期
    std::string&& r6 = s1 + s1; // okay：右值引用延长生存期
    r6 += "Test";
    std::cout << r3 << '\n';

    int i = 1;
    const int ci =2;
    f(i); // 调用 f(int&)
    f(ci); // 调用 f(const int&)
    f(3);  // 调用 f(int&&)
    // 若不提供 f(int&&) 重载则会调用 f(const int&)
    f(std::move(i)); // 调用 f(int&&)

    // 右值引用变量在用于表达式时是左值
    int&& x = 1;
    f(x);            // 调用 f(int& x)
    f(std::move(x)); // 调用 f(int&& x)
}