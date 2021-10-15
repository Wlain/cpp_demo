//
// Created by william on 2021/1/29.
//

/**
 * 题目描述:
 * 有一群孩子和一堆饼干，每个孩子有一个饥饿度，每个饼干都有一个大小。每个孩子只能吃 一个饼干，且只有饼干的大小不小于孩子的饥饿度时，这个孩子才能吃饱。求解最多有多少孩子 可以吃饱。
 *
 * 输入输出样例：
 * 输入两个数组，分别代表孩子的饥饿度和饼干的大小。输出最多有多少孩子可以吃饱的数量。
 * Input: [1,2], [1,2,3]
 * Output: 2
 */
#include <vector>

int32_t findContentChildren(std::vector<int32_t>& children, std::vector<int>& cookies)
{
    std::sort(children.begin(), children.end());
    std::sort(cookies.begin(), cookies.end());
    int childIndex = 0, cookieIndex = 0;
    while (childIndex < children.size() && cookieIndex < cookies.size())
    {
        if (children[childIndex] <= cookies[cookieIndex])
        {
            ++childIndex;
        }
        ++cookieIndex;
    }
    return childIndex;
}
