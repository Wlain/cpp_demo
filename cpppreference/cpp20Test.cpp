//
// Created by william on 2021/4/1.
//

static_assert(__cplusplus >= 201707);

#include <functional>
#include <algorithm>
#if __has_include (<bit>)
#include <bit>
#endif
#include <vector>
#include <cmath>
#include <cassert>
#include <map>
#include <memory>
#include <thread>
#if __cpp_impl_coroutine
#include <coroutine>
#endif
#include <sstream>
#if __has_include (<concepts>)
#include <concepts>
#endif

using namespace std;

void init_20()
{
    struct point { int x, y; };
    struct line { point a, b; };

    point p1 = { .x = 1 };
    assert(p1.x == 1);
    assert(!p1.y);

    point p2 { {} , 2 };
    assert(p2.y == 2);
    assert(!p2.x);

    line l1 = { };
    assert(!l1.a.x);
    line l3 = { 1, 2, 3, 4 };
    assert(l3.b.x == 3);
    assert(l3.b.y == 4);
}

void types_20()
{
#if __cpp_lib_bit_cast
    cout << typeid(bit_cast<double>(0));
#endif
}



void cpp20Test()
{

}