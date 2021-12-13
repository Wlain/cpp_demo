//
// Created by william on 2021/12/13.
//

#include <iostream>
#include <queue>

namespace geekTimeTest
{
void queueTest()
{
    std::queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);
    while (!q.empty())
    {
        std::cout << q.front() << std::endl;
        std::cout << q.back() << std::endl;
        q.pop();
    }
}
} // namespace geekTimeTest
