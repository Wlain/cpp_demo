//
// Created by william on 2021/10/25.
//
#include <algorithm>
#include <iostream>
#include <vector>

//  在指定的范围内应用于给定的操作，并将结果存储在指定的另一个范围内,
void transformTest()
{
    std::string s("hello");
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) -> std::size_t {
        return std::toupper(c);
    });
    /// 等价于     std::transform(s.begin(), s.end(), s.begin(), std::towupper);
    /// 相反的操作是： std::transform(s.begin(), s.end(), s.begin(), std::towlower);
    std::vector<std::size_t> ordinals;
    std::transform(s.begin(), s.end(), std::back_inserter(ordinals), [](unsigned char c) -> std::size_t {
        return c;
    });

    std::cout << s << ":";
    for (auto ord : ordinals)
    {
        std::cout << ' ' << ord;
    }

    std::transform(ordinals.cbegin(), ordinals.cend(), ordinals.cbegin(),
                   ordinals.begin(), std::plus<>{});

    std::cout << '\n';
    for (auto ord : ordinals)
    {
        std::cout << ord << ' ';
    }
    std::cout << '\n';
}