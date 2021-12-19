//
// Created by william on 2021/11/28.
//

#ifndef CPP_DEMO_VEC3_H
#define CPP_DEMO_VEC3_H

#include <cmath>
#include <iostream>

template <typename T>
class Vec3
{
public:
    Vec3() :
        x(T(0)), y(T(0)), z(T(0))
    {}
    explicit Vec3(T xx) :
        x(xx), y(xx), z(xx)
    {}
    Vec3(T xx, T yy, T zz) :
        x(xx), y(yy), z(zz) {}
    ~Vec3() = default;
    T r() const { return x; }
    T g() const { return y; }
    T b() const { return z; }
    Vec3 operator+(const Vec3& v) const
    {
        return Vec3(x + v.x, y + v.y, z + v.z);
    }
    Vec3 operator+(const T r) const
    {
        return Vec3(x + r, y + r, z + r);
    }
    Vec3& operator+=(const T& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    Vec3 operator-(const Vec3& v) const
    {
        return Vec3(x - v.x, y - v.y, z - v.z);
    }
    Vec3 operator-(const T r) const
    {
        return Vec3(x - r, y - r, z - r);
    }
    Vec3& operator-=(const Vec3& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
    Vec3 operator*(const Vec3& v) const
    {
        return Vec3(x * v.x, y * v.y, z * v.z);
    }
    Vec3 operator*(const T r) const
    {
        return Vec3(x * r, y * r, z * r);
    }
    Vec3& operator*=(const T& v)
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }
    Vec3 operator/(const Vec3& v) const
    {
        return Vec3(x / v.x, y / v.y, z / v.z);
    }
    Vec3 operator/(const T r) const
    {
        return Vec3(x / r, y / r, z / r);
    }
    Vec3& operator/=(const T& v)
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }
    Vec3& operator-()
    {
        x = -x;
        y = -y;
        z = -z;
        return *this;
    }
    Vec3 operator-() const
    {
        return Vec3(-x, -y, -z);
    }
    T dotProduct(const Vec3& v) const
    {
        return x * v.x + y * v.y + z * v.z;
    }
    Vec3 crossProduct(const Vec3& v) const
    {
        return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
    T norm() const
    {
        return dotProduct(*this);
    }
    T length() const
    {
        return sqrt(norm());
    }
    const T& operator[](uint8_t i) const { return (&x)[i]; }
    T& operator[](uint8_t i) { return (&x)[i]; }
    Vec3& normalize()
    {
        T n = norm();
        if (n > 0)
        {
            T factor = 1 / sqrt(n);
            x *= factor;
            y *= factor;
            z *= factor;
        }
        return *this;
    }
    friend std::ostream& operator<<(std::ostream& s, const Vec3& v)
    {
        return s << '[' << v.x << ' ' << v.y << ' ' << v.z << ']';
    }
    friend Vec3 operator+(const T& r, const Vec3& v)
    {
        return Vec3(v.x + r, v.y + r, v.z + r);
    }
    friend Vec3 operator-(const T& r, const Vec3& v)
    {
        return Vec3(r - v.x, r - v.y, r - v.z);
    }
    friend Vec3 operator*(const T& r, const Vec3& v)
    {
        return Vec3(v.x * r, v.y * r, v.z * r);
    }
    friend Vec3 operator/(const T& r, const Vec3& v)
    {
        return Vec3(r / v.x, r / v.y, r / v.z);
    }
    T x, y, z;
};

using Vec3f = Vec3<float>;
using Vec3i = Vec3<int>;
#endif //CPP_DEMO_VEC3_H
