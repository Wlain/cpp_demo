//
// Created by william on 2020/12/28.
//
/**
 * 友元声明
 * 友元声明出现于类体内，并向一个函数或另一个类授予对包含友元声明的类的私有及受保护成员的访问权。
 * 语法：
 * 1.friend 函数声明
 * 2.friend 函数定义
 * 3.friend 详述类说明符 ;
 * 4.friend 简单类型说明符 ; C++11
 * 5.friend typename-说明符 ;C++11
 * 友元关系不传递
 * 友元关系不继承
 *
 */

#include "base.h"
class Y
{
    int data;
    friend std::ostream& operator<<(std::ostream& out, const Y& o);
    friend class X;
//    friend char* X::foo(int);   // 其他类的成员亦可为友元
//    friend X::X(char), X::~X(); // 构造函数与析构函数亦可为友元
};

class X
{
public:
    X(Y y)
    {
        a = y.data;
    }
    ~X()
    {
    }
    friend void friendSet(X& p, int i)
    {
        p.a = i; // 此为非成员函数
    }
    char* foo(int)
    {
        return "";
    }

public:
    void memberSet(int i)
    {
        a = i; // 此为成员函数
    }

private:
    int a;
};

std::ostream& operator<<(std::ostream& out, const Y& o)
{
    return out << o.data;
}
// 友元声明不声明成员函数

class A
{
    int data; // 私有数据成员
    class B
    {}; // 私有嵌套类型
    enum
    {
        a = 100
    };               // 私有枚举项
    friend class X1; // 友元类的前置声明（详述类型说明符）
    friend Y;        //  友元类声明（简单类型说明符） (C++11 起)
};

class X1 : A::B // ok：友元能访问A::B
{
    A::B mx; // ok 友元的成员能访问 A::B
    class Y
    {
        A::B my; // OK：友元的嵌套成员能访问 A::B
    };
    int v[A::a]; // OK：友元的成员能访问 A::a
};

class F
{};
int f();

// 模板友元
class A1
{
    template <class T>
    friend class B; // 每个 B<T> 都是 A 的友元

    template <class T>
    friend void f(T){} // 每个 f<T> 都是 A 的友元
};

template<class T> class A2 {}; // 主模板
template<class T> class A2<T*> {}; // 部分特化
template<> class A2<int> {}; // 全特化
class X2
{
//    template <class T> friend class A<T*>; // error
    friend class A2<int>; // OK
};



void extendedFriendDeclarationsTest()
{
    extern int g();
    class Local // extendedFriendDeclarationsTest() 函数中的局部类
    {
        //        friend int f(); // 错误，没有声明于 main() 的该函数
        friend int g();   // OK，main() 中有 g 的声明
        friend class F;   // 令局部 F（随后定义）为友元
        friend class ::F; // 令全局 F 为友元
    };
    class F
    {}; // 局部 F
}


