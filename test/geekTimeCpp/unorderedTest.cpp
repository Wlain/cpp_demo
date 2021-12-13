//
// Created by william on 2021/12/13.
//
#include "geekTimeCpp/common/outputContainer.h"

#include <complex.h>     // std::complex
#include <iostream>      // std::cout/endl
#include <unordered_map> // std::unordered_map
#include <unordered_set> // std::unordered_set

namespace std
{
template <typename T>
struct hash<complex<T>>
{
    size_t operator()(const complex<T>& v) const noexcept
    {
        hash<T> h;
        return h(v.real() + v.imag());
    }
};
} // namespace std

namespace geekTimeTest
{
using namespace std;

void unorderedTest()
{
    unordered_set<int> s{1, 2, 3, 4, 5, 6, 7, 8, 13, 21};
    cout << s << endl;
    unordered_map<complex<double>, double> umc{{{1.0, 1.0}, 1.4142},
                                               {{3.0, 4.0}, 5.0}};
    cout << umc << endl;
}
} // namespace geekTimeTest