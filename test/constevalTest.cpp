//
// Created by william on 2020/12/10.
//

consteval int sqr(int n)
{
    return n * n;
}

constexpr int r = sqr(100);

int x = 100;
//int r2 = sqr(x); // error 调用不产生常量

consteval int sqrsqr(int n)
{
    return sqr(n); // 在此处不是常量表达式，但是OK
}

//constexpr int dblsqr(int n)
//{
//    return 2 * sqr(n); // error ：外围函数非 consteval 且 sqr(n) 不是常量
//}

// 指代某个立即函数的标识表达式，仅可出现在立即调用的子表达式内，或某个立即函数语境内。可以取得立即函数的指针或引用，但它们不可逃出常量表达式求值：

//consteval int f()
//{
//    return 42;
//}
//consteval auto g()
//{
//    return &f;
//}
//
//consteval int h(int (*p)() = g())
//{
//    return p();
//}
//
//constexpr int r1 = h(); // ok
//constexpr auto e = g(); // 非良构：指向立即函数的指针不是常量表达式的容许结果
