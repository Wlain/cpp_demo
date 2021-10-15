//
// Created by william on 2021/4/17.
//
#include "base.h"

class F
{
public:
    F() = default;
    ~F() = default;
    F(const F&) { std::cout << "new" << std::endl; }
    F(const F&&) = default;
};

class F2
{
public:
    F2() = default;
    ~F2() = default;
    F2(const F2&) = default;
    F2(const F2&&) noexcept = default;
    F2& operator=(const F2& f) = default;

public:
    F m_f1{};
};

F2 test(F2 f)
{
    return f;
}

void dtorTest()
{
    F2 f1;
    auto f3 = test(f1);
}