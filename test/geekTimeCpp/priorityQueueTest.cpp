//
// Created by william on 2021/12/13.
//
#include "outputContainer.h"

#include <functional>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>

namespace geekTimeTest
{
template <typename T>
void printQueue(T& q)
{
    while (!q.empty())
    {
        std::cout << q.top() << " ";
        q.pop();
    }
    std::cout << '\n';
}
void priorityQueueTest()
{
    using namespace std;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> q;
    q.push({ 1, 1 });
    q.push({ 2, 2 });
    q.push({ 0, 3 });
    q.push({ 9, 4 });
    printQueue(q);
    std::priority_queue<int> q1;
    for (int n : { 1, 8, 5, 6, 3, 4, 0, 9, 7, 2 })
        q1.push(n);
    printQueue(q1);
    // 用 lambda 比较元素。
    auto cmp = [](int left, int right) { return (left ^ 1) < (right ^ 1); };
    std::priority_queue<int, std::vector<int>, decltype(cmp)> q3(cmp);
    for(int n : {1,8,5,6,3,4,0,9,7,2})
        q3.push(n);

    printQueue(q3);
}
} // namespace geekTimeTest