//
// Created by william on 2021/11/30.
//

#ifndef CPP_DEMO_COLOR_H
#define CPP_DEMO_COLOR_H

#include <ostream>

template <typename T>
class Color3
{
public:
    Color3() {}
    Color3(T rr) :
        m_red(rr), m_green(rr), m_blue() {}
    Color3(T rr, T gg, T bb) :
        m_red(rr), m_green(gg), m_blue(bb) {}
    Color3 operator*(const T& v) const
    {
        return Color3(m_red * v, m_green * v, m_blue * v);
    }
    Color3 operator/(const T& v) const
    {
        return Color3(m_red / v, m_green / v, m_blue / v);
    }
    Color3 operator+(const Color3& c) const
    {
        return Color3(m_red + c.m_red, m_green + c.m_green, m_blue + c.m_blue);
    }
    friend Color3 operator*(const float& f, const Color3& c)
    {
        return Color3(c.m_red * f, c.m_green * f, c.m_blue * f);
    }
    friend std::ostream& operator<<(std::ostream& os, const Color3& c)
    {
        os << c.m_red << " " << c.m_green << " " << c.m_blue;
        return os;
    }
    float m_blue, m_green, m_red;
};

using Color3f = Color3<float>;

#endif //CPP_DEMO_COLOR_H
