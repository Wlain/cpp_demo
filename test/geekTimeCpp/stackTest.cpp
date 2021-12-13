//
// Created by william on 2021/12/13.
//

#include <iostream>
#include <stack>

namespace geekTimeTest
{
void stackTest()
{
    std::stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);
    while (!s.empty())
    {
        std::cout << s.top() << std::endl;
        s.pop();
    }
}
} // namespace geekTimeTest