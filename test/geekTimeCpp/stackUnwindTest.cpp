//
// Created by william on 2021/12/9.
//
#include <stdio.h>

namespace geekTimeTest
{
class Obj
{
public:
    Obj()
    {
        puts("Obj()");
    }
    ~Obj()
    {
        puts("~Obj()");
    }
};

void foo(int n)
{
    Obj obj;
    if (n == 42)
    {
        throw "life, the universe and everything";
    }
}

void stackUnwindTest()
{
    try
    {
        foo(41);
        foo(42);
    }
    catch (const char* s)
    {
        puts(s);
    }
}
} // namespace geekTimeTest