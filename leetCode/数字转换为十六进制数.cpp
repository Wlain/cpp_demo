//
// Created by william on 2021/10/2.
//
#include "base.h"

//给定一个整数，编写一个算法将这个数转换为十六进制数。对于负整数，我们通常使用 补码运算 方法。
//
//注意:
//
//十六进制中所有字母(a-f)都必须是小写。
//十六进制字符串中不能包含多余的前导零。如果要转化的数为0，那么以单个字符'0'来表示；对于其他情况，十六进制字符串中的第一个字符将不会是0字符。 
//给定的数确保在32位有符号整数范围内。
//不能使用任何由库提供的将数字直接转换或格式化为十六进制的方法。
//示例 1：
//
//输入:
//26
//
//输出:
//"1a"
//示例 2：
//
//输入:
//-1
//
//输出:
//"ffffffff"
//

//链接：https://leetcode-cn.com/problems/convert-a-number-to-hexadecimal

std::string toHex(int num)
{
    if (num == 0)
    {
        return "0";
    }
    else if (num > 0)
    {
        int index = 0;
        while (16 * (++index) <= num)
            ;
        std::string hex;
        for (int i = index - 1; i > -1; --i)
        {
            if (num >= 16)
            {
                int powNum =16 * i;
                auto dev = num / powNum;
                num = num - powNum;
                hex += (char)(dev < 10 ? '0' + dev : 'a' + (dev - 10));
            }
            else
            {
                hex += (char)(num < 10 ? '0' + num : 'a' + (num - 10));
            }
        }
        return hex;
    }
    else
    {
        std::string hex = "0123456789abcdef", ans;
        while (num && ans.size() < 8)
        {
            ans = hex[num & 0xf] + ans;
            num >>= 4;
        }
        return ans;
    }
}