//
// Created by william on 2021/12/10.
//
#include <stdio.h> // puts
#include <utility> // std::forward

namespace geekTimeTest
{
class Shape
{
public:
    Shape() { puts("Shape()"); }
    virtual ~Shape() { puts("~Shape()"); }
};

class Circle : public Shape
{
public:
    Circle() { puts("Circle()"); }
    ~Circle() override { puts("~Circle()"); }
};

void foo(const Shape&)
{
    puts("foo(const Shape&)");
}

void foo(Shape&&)
{
    puts("foo(Shape&&)");
}

// 完美转发
template<typename T>
void bar(T&& s)
{
    foo(std::forward<T>(s));
}

void forwardTest()
{
    Circle temp;
    bar(temp);
    bar(Circle());
}
} // namespace geekTimeTest