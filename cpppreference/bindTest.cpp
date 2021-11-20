//
// Created by william on 2021/6/13.
//
#include <iostream>
#include <random>

void f(int n1, int n2, int n3, const int& n4, int n5)
{
    std::cout << n1 << ' ' << n2 << ' ' << n3 << ' ' << n4 << ' ' << n5 << '\n';
}

int g(int n1)
{
    return n1;
}

struct Foo
{
    void printSum(int n1, int n2)
    {
        std::cout << n1 + n2 << std::endl;
    }
    int data = 10;
};

void bindTest()
{
    using namespace std::placeholders; // 对于_1, _2, _3...
    // 演示参数重排序和按引用传递
    int n = 7;
    // （ _1 与 _2 来自 std::placeholders ，并表示将来会传递给 f1 的参数）
    auto f1 = std::bind(f, _2, 42, _1, std::cref(n), n);
    n = 10;
    f1(1, 2, 1001);// 1 为 _1 所绑定， 2 为 _2 所绑定，不使用 1001
    // 进行到 f(2, 42, 1, n, 7) 的调用
    auto f2 = std::bind(f, _3, std::bind(g, 3), _3, 4, 5);
    f2(10, 11, 12); // 进行到 f(12, g(12), 12, 4, 5); 的调用
    // 常见使用情况：以分布绑定 RNG
    std::default_random_engine e;
    std::uniform_int_distribution<> d(0, 10);
    std::function<int()> rnd = std::bind(d, e); // e 的一个副本存储于 rnd
    for(int i =0; i <10; ++i)
        std::cout << rnd() << ' ';
    std::cout << '\n';
    // 绑定指向成员函数指针
    Foo foo;
    auto f3 = std::bind(&Foo::printSum, &foo, 95, _1);
    f3(5);
    // 绑定指向数据成员指针
    auto f4 = std::bind(&Foo::data, _1);
    std::cout << f4(foo) << '\n';
    // 智能指针亦能用于调用被引用对象的成员
    std::cout << f4(std::make_shared<Foo>(foo)) << '\n'
              << f4(std::make_unique<Foo>(foo)) << '\n';
}
