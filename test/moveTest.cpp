//
// Created by william on 2020/12/25.
//

#include "base.h"

#include <sstream>
#include <utility>

// std::move 用于指示对象 t 可以“被移动”,即允许从 t 到另一对象的有效率的资源传递。
// 特别是， std::move 生成标识其参数 t 的亡值表达式。它准确地等价于到右值引用类型的 static_cast 。
// t	-	要被移动的对象
// 返回值 : static_cast<typename std::remove_reference<T>::type&&>(t)

// 不存在 void 的引用，也不存在引用的引用。
/*
 *
 * 语法：
    T & ref = object ;

    T & ref = { arg1, arg2, ... };

    T & ref ( object ) ;

    T & ref { arg1, arg2, ... } ;
    ////////////////////////////

    T && ref = object ;

    T && ref = { arg1, arg2, ... };

    T && ref ( object ) ;

    T && ref { arg1, arg2, ... } ;
 */

// 一旦初始化，则不能更改引用使之引用另一对象。

void referenceDeclarationTest1()
{
    // 若 object 是左值表达式，且其类型为 T 或派生于 T，而且有相等或更少的 cv 限定，则引用被绑定到左值表达式所标识的对象或其基类子对象。
    double d = 2.0;
    double& rd = d;
    const double& rcd = d;
    struct A
    {};
    struct B : A
    {
    } b;
    A& ra = b;        // ra 引用 b 中的 A 子对象
    const A& rca = b; //  rca 引用 b 中的 A 子对象
    // 否则，若 object 的类型与 T 不相同且不从它派生，而 object 拥有到左值的转换函数，其类型为 T 或派生于 T 并有相等或更少的 cv 限定，则绑定引用到转换函数所返回的左值所标识的对象（或到其基类子对象）。
    struct A1
    {};
    struct B1 : A
    {
        int n = 1;
        operator int&(){ return n; };
    };
    int& ir = B1(); // ir指代B::operator int&的结果
    // 否则，若引用是到非 volatile 的 const 限定类型的左值引用或右值引用 (C++11 起)：

    struct A2
    {};
    struct B2 : A2
    {};
    //    extern B f();
    //    const A& rca2 = f(); // 到 B 右值的 A 子对象。
    //    A&& rra = f();
    int i2 = 42;
    int&& rri = static_cast<int&&>(i2);

    const std::string& rs = "abc"; // rs 指代从字符数组复制初始化的临时量
    const double& rcd2 = 2;        // rcd2 指代值为 2.0 的临时量
    int i3 = 2;
    double&& rrd3 = i3; // rrd3 指代值为 2.0 的临时量

    // 一旦引用被绑定到临时量或其子对象，临时量的生存期就被延续以匹配引用的生存期，但有下列例外：
}

struct S
{
    int mi;
    const std::pair<int, int>& mp;
};

void foo1(int)
{}

struct A
{};
struct B : A
{
    int n;
    operator int&() { return n; }
};
B Bar()
{
    return B();
}

extern int& ext_r; // ok

void referenceDeclarationTest2()
{
    // 左值
    int n = 1;
    int& r1 = n;
    const int& cr(n); // // 引用可以有更多 cv 限定
    volatile int& cv{ n };
    int& r2 = r1;                   // 另一到对象 n 的左值引用
    int& r3 = const_cast<int&>(cr); // 需要 const_cast

    void (&rf)(int) = foo1; // 到函数的左值引用
    int ar[3];
    int(&ra)[3] = ar; // 到数组的左值引用

    B b;
    A& base_ref = b;        // 到基类子对象的左值引用
    int& converted_ref = b; // // 到转换结果的左值引用

    // 右值
    //    int& bad = 1; // error: 不能绑定左值引用到右值
    const int& cref = 1;    // 绑定到右值
    int&& rref = 1;         // 绑定到右值
    const A& cref2 = Bar(); // 到 B 临时变量的 A 子对象的引用
    A&& rref2 = Bar();      // 同 const A& cref2 = Bar();

    int&& xref = static_cast<int&&>(n); // 直接绑定到 n
    int&& xref1 = std::move(n);
    //    int&& xref1 = n; // 错误：不能绑定到左值
    double&& copy_ref = n; // 绑定到值为 1.0 的右值临时量

    // 临时变量生存期上的限制
    std::ostream&& buf_ref = std::ostringstream() << 'a'; // ostringstream 临时量被绑定到 operator<< 的左运算数，但其生存期在分号结束，故 buf_ref 为悬垂引用。
    S a{ 1, { 2, 3 } };                                   // 绑定临时量 pair {2,3} 到引用成员 a.mp 并延长其生存期以匹配 a
//    S* p = new S{ 1, { 2, 3 } };                          // 绑定临时量 pair {2,3} 到引用成员 p->mp ，但其生存期在分号结束,p->mp 是悬垂引用
//    delete p;
}
