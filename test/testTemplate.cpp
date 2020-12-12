//
// Created by william on 2020/12/12.
//
#include "base.h"
#include <algorithm>
template<typename T>
T maxValue(const T& t1, const T& t2)
{
    return t1 > t2 ? t1 : t2;
}

// 应该用大写字母表示类型
// C++ 14
template<typename T, typename S>
auto maxValue(const T& t1, const S& t2)
{
    return t1 > t2 ? t1 : t2;
}

// 三种方式，实例函数，实例类
// 函数模板实例化，函数模板不是一个方法，也不是一个类型,函数模板的实例，就是一段函数代码
template double maxValue<double>(const double&, const double&);
template <short> short maxValue(const short&, const short&);
// int 表示一个形参
template int maxValue(const int&, const int&);
template float maxValue<>(const float&, const float&);
// 函数指针，ptr指向maxValue
std::string(*ptr)(const std::string&, const std::string&) = maxValue;

// 选择排序
std::size_t  size;

// std::size_t 是 sizeof 运算符还有 sizeof... 运算符和 alignof 运算符 (C++11 起)所返回的无符号整数类型。
// std::size_t 的位宽不小于 16
constexpr int a = 1;
// 声明一个编译器常量

// 设计泛型函数

template <typename T>
void selectionSort(T list[], const std::size_t size)
{

}


template <typename T, int N>
void selectionSort(std::array<T, N>& list)
{

}

// 类模板，数据成员，函数成员
template <typename T>
class Stack
{
public:
    Stack();
    bool empty();
    T peek();
    T push(T);
    T pop();
    int getSize();

private:
    std::array<T, 100> m_elements;
    int size = 0;
};

template <typename T>
Stack<T>::Stack() = default;

template <typename T>
bool Stack<T>::empty()
{
    return (size == 0);
}

template <typename T>
T Stack<T>::peek()
{
    T t;
    return t;
}

template <typename T>
T Stack<T>::push(T)
{
    T t;
    return t;
}

template <typename T>
T Stack<T>::pop()
{
    T t;
    return t;
}

template <typename T>
int Stack<T>::getSize()
{
    return 0;
}

// 默认类型
int  circle(int x, int y, int r = 100);

// 默认类型参数, 只适用于类模板，不能用于函数模板
template <typename  T = int>
class Test
{
    Test() = default;
    ~Test() = default;
private:
    T m_t;
};

// 非类型参数
template <typename T = char, int capacity = 1>
class Test2
{
private:
    std::array<T, capacity> m_elems;
};

Test2<char, 100> charTest2;

Test2<> charTest3;

// 模板与继承
// 普通类可以从类模板实例继承
// 模板可以从普通类继承
// 类模板可继承类模板



















