//
// Created by william on 2021/11/28.
//

#ifndef CPP_DEMO_VEC2_H
#define CPP_DEMO_VEC2_H
#include <cmath>
#include <cstdlib>
#include <iostream>
template <typename T>
class Vec2
{
public:
    Vec2() :
        x(T(0)), y(T(0)) {}
    explicit Vec2(T xx) :
        x(xx), y(xx) {}
    Vec2(T xx, T yy) :
        x(xx), y(yy) {}
    ~Vec2() = default;
    Vec2 operator+(const Vec2& v) const
    {
        return Vec2(x + v.x, y + v.y);
    }
    Vec2 operator+(const T r) const
    {
        return Vec2(x + r, y + r);
    }
    Vec2& operator+=(const Vec2& v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }
    Vec2 operator-(const Vec2& v) const
    {
        Vec2(x - v.x, y - v.y);
    }
    Vec2 operator-(const T r) const
    {
        return Vec2(x - r, y - r);
    }
    Vec2& operator-=(const Vec2& v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }
    Vec2 operator*(const Vec2& v) const
    {
        Vec2(x * v.x, y * v.y);
    }
    Vec2 operator*(const T r) const
    {
        return Vec2(x * r, y * r);
    }
    Vec2& operator*=(const Vec2& v)
    {
        x *= v.x;
        y *= v.y;
        return *this;
    }
    Vec2& operator*=(const T &r)
    {
        x *= r;
        y *= r;
        return *this;
    }
    Vec2 operator/(const Vec2& v) const
    {
        Vec2(x / v.x, y / v.y);
    }
    Vec2 operator/(const T r) const
    {
        return Vec2(x / r, y / r);
    }
    Vec2& operator/=(const Vec2& v)
    {
        x /= v.x;
        y /= v.y;
        return *this;
    }
    Vec2& operator-()
    {
        x = -x;
        y = -y;
        return *this;
    }
    Vec2 operator-() const
    {
        return Vec2(-x, -y);
    }
    friend std::ostream& operator<<(std::ostream& s, const Vec2& v)
    {
        return s << '[' << v.x << ' ' << v.y << ']';
    }
    friend Vec2 operator+(const T& r, const Vec2& v)
    {
        return Vec2(v.x + r, v.y + r);
    }
    friend Vec2 operator-(const T& r, const Vec2& v)
    {
        return Vec(r - v.x, r - v.y);
    }
    friend Vec2 operator*(const T& r, const Vec2& v)
    {
        return Vec2(v.x * r, v.y * r);
    }
    friend Vec2 operator/(const T& r, const Vec2& v)
    {
        return Vec2(r / v.x, r / v.y);
    }
    T x, y;
};

using Vec2f = Vec2<float>;
using Vec2i = Vec2<int>;
#endif //CPP_DEMO_VEC2_H
