//
// Created by william on 2021/12/10.
//
#include "geekTimeCpp/common/shape.h"

#include <stdio.h> // puts

namespace geekTimeTest
{
class Result
{
public:
    Result() { puts("Result()"); }
    ~Result() { puts("~Result()"); }
};

Result processShape(const Shape& shape1, const Shape& shape2)
{
    puts("process_shape()");
    return Result();
}

void lifeTimeTest()
{
    puts("main()");
    auto&& result = processShape(Circle(), Triangle());
    puts("something else");
}
} // namespace geekTimeTest