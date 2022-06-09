//
// Created by cwb on 2022/6/9.
//
#include <map>
#include <vector>

/*
 * easy
 * 两数字求和问题：https://leetcode.cn/problems/two-sum
 * 解法一：暴力解法(o(n^2))
 * 解法二：哈希表（o(n)）
 */

using namespace std;

vector<int> twoSum(vector<int>& nums, int target)
{
    std::map<int, int> flag;
    for (int i = 0; i < nums.size(); ++i)
    {
        auto num = nums[i];
        if (flag.find(target - num) != flag.end())
        {
            return { flag[target - num], i };
        }
        flag[num] = i;
    }
    return { 0, 0 };
}
