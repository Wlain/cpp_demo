//
// Created by cwb on 2022/10/8.
//

#include <cmath>
#include <cstdarg>
#include <iostream>

// 宏定义可变参数
#define ADD_CODE_BLOCK_TEST(...)                              \
    std::cout << "=========code before========" << std::endl; \
    __VA_ARGS__                                               \
    std::cout << "=========code end========" << std::endl

// 方法可变参数
double stddev(int count, ...)
{
    double sum = 0;
    double sum_sq = 0;
    std::va_list args;
    va_start(args, count);
    for (int i = 0; i < count; ++i)
    {
        double num = va_arg(args, double);
        sum += num;
        sum_sq += num * num;
    }
    va_end(args);
    return std::sqrt(sum_sq / count - (sum / count) * (sum / count));
}

void microTest()
{
    ADD_CODE_BLOCK_TEST({
        std::cout << "this is the insert code block!!!" << std::endl;
    });
    ADD_CODE_BLOCK_TEST();
    std::cout << stddev(4, 25.0, 27.3, 26.9, 25.7) << '\n';
}
