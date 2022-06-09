//
// Created by cwb on 2022/6/9.
//
#include "leetCodeCommonDefine.h"

/*
 * difficulty：easy
 * 解法一：暴力解法(o(n^2))
 * 解法二：哈希表（o(n)）
 */

class Solution
{
public:
    std::vector<int> twoSum(std::vector<int>& nums, int target)
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
};

void twoSumTest()
{
    std::string line;
    while (getline(std::cin, line))
    {
        std::vector<int> nums = stringToIntegerVector(line);
        getline(std::cin, line);
        int target = stringToInteger(line);
        std::vector<int> ret = Solution().twoSum(nums, target);
        std::string out = integerVectorToString(ret);
        std::cout << out << std::endl;
    }
}
