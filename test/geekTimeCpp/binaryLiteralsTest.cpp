//
// Created by william on 2022/1/10.
//
#include <bitset>   // std::bitset
#include <iostream> // std::cout/endl
#include <thread>   // std::this_thread

using namespace std;

namespace geekTimeTest
{
void binaryLiteralsTest()
{
    unsigned mask = 0b111000000;
    std::cout << std::bitset<9>(mask) << std::endl;
}

void literalsTest()
{
    cout << "i * i = " << 1i * 1i << endl;
    cout << "Waiting for 500ms" << endl;
    this_thread::sleep_for(500ms);
    cout << "Hello world"s.substr(0, 5) << endl;
}

struct length
{
    double value;
    enum unit
    {
        metre,
        kilometre,
        millimetre,
        centimetre,
        inch,
        foot,
        yard,
        mile,
    };
    static constexpr double factors[] = { 1.0, 1000.0, 1e-3, 1e-2, 0.0254, 0.3048, 0.9144, 1609.344 };
    explicit length(double v, unit u = metre) { value = v * factors[u]; }
};

length operator+(const length& lhs, const length& rhs)
{
    return length(lhs.value + rhs.value);
}

length operator"" _m(long double v)
{
    return length(v, length::metre);
}

length operator"" _cm(long double v)
{
    return length(v, length::centimetre);
}

void userLiteralsTest()
{
    auto result = 1.0_m + 10.0_cm;
    cout << "Result is " << result.value << "m\n";
}
} // namespace geekTimeTest