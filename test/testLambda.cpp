//
// Created by william on 2020/12/10.
//

#include "base.h"

#include <algorithm>
#include <functional>
#include <vector>

void lambdaTest()
{
    std::vector<int> c = { 1, 2, 3, 4, 5, 6, 7 };
    int x = 5;
    c.erase(std::remove_if(c.begin(), c.end(), [x](int n) { return n < x; }), c.end());
    std::cout << "c: ";
    std::for_each(c.begin(), c.end(), [](int i) { std::cout << i << ' '; });
    std::cout << '\n';

    auto func1 = [](int i = 6) { return i + 4; };
    std::cout << "func1:" << func1() << std::endl;

    // 与所有可调用对象相同，闭包能可以被捕获到 std::function 之中
    // （这可能带来不必要的开销）
    std::function<int(int)> func2 = [](int i) { return i + 4; };
    std::cout << "func2" << func2(6) << std::endl;
}