//
// Created by william on 2022/5/18.
//

#ifndef CPP_DEMO_RENDERHELP_H
#define CPP_DEMO_RENDERHELP_H
/// 软光栅渲染器：基于D3D

#include "timer.h"

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
/// 数学工具
// 基础矢量
template <size_t N, typename T>
struct Vector
{
    T m[N];
    Vector()
    {
        for (size_t i = 0; i < N; ++i)
            m[N] = T();
    }
    explicit Vector(const T* ptr)
    {
        for (size_t i = 0; i < N; ++i)
            m[N] = ptr[i];
    }
    Vector(const Vector<N, T>& v)
    {
        for (size_t i = 0; i < N; ++i)
            m[N] = v.m[i];
    }
    Vector(const std::initializer_list<T>& v)
    {
        auto it = v.begin();
        for (int i = 0; i < N; ++i)
            m[N] = *(it++);
    }
    inline const T& operator[](size_t i) const { return m[i]; }
    inline T& operator[](size_t i) { return m[i]; }
    inline void load(const T* ptr)
    {
        for (size_t i = 0; i < N; ++i)
            m[i] = ptr[i];
    }
    inline void save(T* ptr)
    {
        for (size_t i = 0; i < N; ++i)
            ptr[i] = m[i];
    }
};

/// 特化二维矢量
template <typename T>
struct Vector<2, T>
{
    union
    {
        struct
        {
            T x, y;
        };
        struct
        {
            T u, v;
        };
        T m[2];
    };
    Vector() :
        x(T()), y(T()) {}
    Vector(T _x, T _y) :
        x(_x), y(_y) {}
    Vector(const Vector<2, T>& v) :
        x(v.x), y(v.y) {}
    explicit Vector(const T* ptr) :
        x(ptr[0]), y(ptr[1]) {}
    inline const T& operator[](size_t i) const
    {
        assert(i < 2);
        return m[i];
    }
    inline T& operator[](size_t i)
    {
        assert(i < 2);
        return m[i];
    }
    inline void load(const T* ptr)
    {
        for (size_t i = 0; i < 2; ++i)
            m[i] = ptr[i];
    }
    inline void save(T* ptr)
    {
        for (size_t i = 0; i < 2; ++i)
            ptr[i] = m[i];
    }
};

// 特化三维矢量
template <typename T>
struct Vector<3, T>
{
    union
    {
        struct
        {
            T x, y, z;
        }; // 元素别名
        struct
        {
            T r, g, b;
        };      // 元素别名
        T m[3]; // 元素数组
    };
    Vector() :
        x(T()), y(T()), z(T()) {}
    Vector(T X, T Y, T Z) :
        x(X), y(Y), z(Z) {}
    Vector(const Vector<3, T>& u) :
        x(u.x), y(u.y), z(u.z) {}
    explicit Vector(const T* ptr) :
        x(ptr[0]), y(ptr[1]), z(ptr[2]) {}
    inline const T& operator[](size_t i) const
    {
        assert(i < 3);
        return m[i];
    }
    inline T& operator[](size_t i)
    {
        assert(i < 3);
        return m[i];
    }
    inline void load(const T* ptr)
    {
        for (size_t i = 0; i < 3; ++i)
            m[i] = ptr[i];
    }
    inline void save(T* ptr)
    {
        for (size_t i = 0; i < 3; ++i)
            ptr[i] = m[i];
    }
    inline Vector<2, T> xy() const { return Vector<2, T>(x, y); }
    inline Vector<3, T> xyz() const { return *this; }
    inline Vector<4, T> xyz1() const { return Vector<4, T>(x, y, z, 1); }
};

/// 特化四维矢量
template <typename T>
struct Vector<4, T>
{
    union
    {
        struct
        {
            T x, y, z, w;
        }; // 元素别名
        struct
        {
            T r, g, b, a;
        };      // 元素别名
        T m[4]; // 元素数组
    };
    Vector() :
        x(T()), y(T()), z(T()), w(T()) {}
    Vector(T X, T Y, T Z, T W) :
        x(X), y(Y), z(Z), w(W) {}
    Vector(const Vector<4, T>& u) :
        x(u.x), y(u.y), z(u.z), w(u.w) {}
    explicit Vector(const T* ptr) :
        x(ptr[0]), y(ptr[1]), z(ptr[2]), w(ptr[3]) {}
    inline const T& operator[](size_t i) const
    {
        assert(i < 4);
        return m[i];
    }
    inline T& operator[](size_t i)
    {
        assert(i < 4);
        return m[i];
    }
    inline void load(const T* ptr)
    {
        for (size_t i = 0; i < 4; ++i)
            m[i] = ptr[i];
    }
    inline void save(T* ptr)
    {
        for (size_t i = 0; i < 4; ++i)
            ptr[i] = m[i];
    }
    inline Vector<2, T> xy() const { return Vector<2, T>(x, y); }
    inline Vector<3, T> xyz() const { return Vector<3, T>(x, y, z); }
    inline Vector<4, T> xyzw() const { return *this; }
};

/// 数学库：矢量运算
// = (+a)
template <size_t N, typename T>
inline Vector<N, T> operator+(const Vector<N, T>& a)
{
    return a;
}

// = (-a)
template <size_t N, typename T>
inline Vector<N, T> operator-(const Vector<N, T>& a)
{
    Vector<N, T> b;
    for (int i = 0; i < N; ++i)
        b.m[i] = a.m[i];
    return b;
}

// = (a == b) ? true : false
template <size_t N, typename T>
inline bool operator==(const Vector<N, T>& a, const Vector<N, T>& b)
{
    for (size_t i = 0; i < N; ++i)
        if (a[i] != b[i]) return false;
    return true;
}

// = (a != b)? true : false
template <size_t N, typename T>
inline bool operator!=(const Vector<N, T>& a, const Vector<N, T>& b)
{
    return !(a == b);
}

// = a + b
template <size_t N, typename T>
inline Vector<N, T> operator+(const Vector<N, T>& a, const Vector<N, T>& b)
{
    Vector<N, T> c;
    for (size_t i = 0; i < N; ++i)
        c[i] = a[i] + b[i];
    return c;
}

// = a - b
template <size_t N, typename T>
inline Vector<N, T> operator-(const Vector<N, T>& a, const Vector<N, T>& b)
{
    Vector<N, T> c;
    for (size_t i = 0; i < N; ++i)
        c[i] = a[i] - b[i];
    return c;
}

// = a * b，不是点乘也不是叉乘，而是各个元素分别相乘，色彩计算时有用
template <size_t N, typename T>
inline Vector<N, T> operator*(const Vector<N, T>& a, const Vector<N, T>& b)
{
    Vector<N, T> c;
    for (size_t i = 0; i < N; ++i)
        c[i] = a[i] * b[i];
    return c;
}

// = a / b，各个元素相除
template <size_t N, typename T>
inline Vector<N, T> operator/(const Vector<N, T>& a, const Vector<N, T>& b)
{
    Vector<N, T> c;
    for (size_t i = 0; i < N; ++i)
        c[i] = a[i] / b[i];
    return c;
}

// = a * x
template <size_t N, typename T>
inline Vector<N, T> operator*(const Vector<N, T>& a, T x)
{
    Vector<N, T> b;
    for (size_t i = 0; i < N; ++i)
        b[i] = a[i] * x;
    return b;
}

// = x * a
template <size_t N, typename T>
inline Vector<N, T> operator*(T x, const Vector<N, T>& a)
{
    return a * x;
}

// = a / x
template <size_t N, typename T>
inline Vector<N, T> operator/(const Vector<N, T>& a, T x)
{
    Vector<N, T> b;
    for (size_t i = 0; i < N; i++)
        b[i] = a[i] / x;
    return b;
}

// = x / a
template <size_t N, typename T>
inline Vector<N, T> operator/(T x, const Vector<N, T>& a)
{
    Vector<N, T> b;
    for (size_t i = 0; i < N; ++i)
        b[i] = x / a[i];
    return b;
}

// a += b
template <size_t N, typename T>
inline Vector<N, T>& operator+=(Vector<N, T>& a, const Vector<N, T>& b)
{
    for (size_t i = 0; i < N; ++i)
        a[i] += b[i];
    return a;
}

// a -= b
template <size_t N, typename T>
inline Vector<N, T>& operator-=(Vector<N, T>& a, const Vector<N, T>& b)
{
    for (size_t i = 0; i < N; ++i)
        a[i] -= b[i];
    return a;
}

// a *= b
template <size_t N, typename T>
inline Vector<N, T>& operator*=(Vector<N, T>& a, const Vector<N, T>& b)
{
    for (size_t i = 0; i < N; ++i)
        a[i] *= b[i];
    return a;
}

// a /= b
template <size_t N, typename T>
inline Vector<N, T>& operator/=(Vector<N, T>& a, const Vector<N, T>& b)
{
    for (size_t i = 0; i < N; ++i)
        a[i] /= b[i];
    return a;
}

// a *= x
template <size_t N, typename T>
inline Vector<N, T>& operator*=(Vector<N, T>& a, T x)
{
    for (size_t i = 0; i < N; ++i)
        a[i] *= x;
    return a;
}

// a /= x
template <size_t N, typename T>
inline Vector<N, T>& operator/=(Vector<N, T>& a, T x)
{
    for (size_t i = 0; i < N; ++i)
        a[i] /= x;
    return a;
}

/// 数学库：矢量函数
// 不同维度的矢量转换
template <size_t N1, size_t N2, typename T>
inline Vector<N1, T> vectorConvert(const Vector<N2, T>& a, T fill = 1)
{
    Vector<N1, T> b;
    for (size_t i = 0; i < N1; ++i)
        b[i] = (i < N2) ? a[i] : fill;
    return b;
}

// = |a| ^ 2
template <size_t N, typename T>
inline T vectorLengthSquare(const Vector<N, T>& a)
{
    T sum = 0;
    for (size_t i = 0; i < N; ++i)
        sum += std::pow(a[i], 2);
    return sum;
}

// = |a|
template <size_t N, typename T>
inline T vectorLength(const Vector<N, T>& a)
{
    return sqrt(vectorLengthSquare(a));
}

// = |a| , 特化 float 类型，使用 sqrtf
template <size_t N>
inline float vectorLength(const Vector<N, float>& a)
{
    return sqrtf(vectorLengthSquare(a));
}

// = a / |a|
template <size_t N, typename T>
inline Vector<N, T> vectorNormalize(const Vector<N, T>& a)
{
    return a / vectorLength(a);
}

// 矢量点乘
template <size_t N, typename T>
inline T vectorDot(const Vector<N, T>& a, const Vector<N, T>& b)
{
    T sum = 0;
    for (size_t i = 0; i < N; ++i)
        sum += a[i] * b[i];
    return sum;
}

// 二维矢量叉乘，得到标量
template <typename T>
inline T vectorCross(const Vector<2, T>& a, const Vector<2, T>& b)
{
    return a.x * b.y - a.y * b.x;
}

// 三维矢量叉乘，得到新矢量
template <typename T>
inline Vector<3, T> vectorCross(const Vector<3, T>& a, const Vector<3, T>& b)
{
    return Vector<3, T>{ a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
}

// 四维矢量叉乘：前三维叉乘，后一位保留
template <typename T>
inline Vector<4, T> vectorCross(const Vector<4, T>& a, const Vector<4, T>& b)
{
    return Vector<4, T>{ a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x, a.w };
}

// = a + (b - a) * t
template <size_t N, typename T>
inline Vector<N, T> vectorLerp(const Vector<N, T>& a, const Vector<N, T>& b, float t)
{
    return a + (b - a) * t;
}

// 各个元素取最大值
template <size_t N, typename T>
inline Vector<N, T> vectorMax(const Vector<N, T>& a, const Vector<N, T>& b)
{
    Vector<N, T> c;
    for (size_t i = 0; i < N; ++i)
        c[i] = std::max(a[i], b[i]);
    return c;
}

// 各个元素取最小值
template <size_t N, typename T>
inline Vector<N, T> vectorMin(const Vector<N, T>& a, const Vector<N, T>& b)
{
    Vector<N, T> c;
    for (size_t i = 0; i < N; ++i)
        c[i] = std::min(a[i], b[i]);
    return c;
}

// 将矢量的值控制在 minx/maxx 范围内
template <size_t N, typename T>
inline Vector<N, T> vectorBetween(const Vector<N, T>& minx, const Vector<N, T>& maxx, const Vector<N, T>& x)
{
    return vectorMin(vectorMax(minx, x), maxx);
}

// 判断矢量是否接近
template <size_t N, typename T>
inline bool vectorNear(const Vector<N, T>& a, const Vector<N, T>& b, T dist)
{
    return (vectorLengthSquare(a - b) <= dist);
}

// 判断两个单精度矢量是否近似
template <size_t N>
inline bool vectorNearEqual(const Vector<N, float>& a, const Vector<N, float>& b, float e = 0.0001)
{
    return vectorNear(a, b, e);
}

// 判断两个双精度矢量是否近似
template <size_t N>
inline bool vectorNearEqual(const Vector<N, double>& a, const Vector<N, double>& b, double e = 0.0000001)
{
    return vectorNear(a, b, e);
}

// 矢量值元素范围裁剪
template <size_t N, typename T>
inline Vector<N, T> vectorClamp(const Vector<N, T>& a, T minx = 0, T maxx = 1)
{
    Vector<N, T> b;
    for (size_t i = 0; i < N; ++i)
    {
        b[i] = std::clamp(a[i], minx, maxx);
    }
    return b;
}

// 输出到文本流
template <size_t N, typename T>
inline std::ostream& operator<<(std::ostream& os, const Vector<N, T>& a)
{
    os << "[";
    for (size_t i = 0; i < N; ++i)
    {
        os << a[i];
        if (i < N - 1) os << ", ";
    }
    os << "]";
    return os;
}

// 输出成字符串
template <size_t N, typename T>
inline std::string vectorRepr(const Vector<N, T>& a)
{
    std::stringstream ss;
    ss << a;
    return ss.str();
}

/// 数学库：矩阵
template <size_t ROW, size_t COL, typename T>
struct Matrix
{
    /// 左手坐标系，行存储
    T m[ROW][COL];
    Matrix() = default;
    Matrix(const Matrix<ROW, COL, T>& src)
    {
        for (size_t r = 0; r < ROW; ++r)
        {
            for (size_t c = 0; c < COL; ++c)
                m[r][c] = src.m[r][c];
        }
    }
    inline Matrix(const std::initializer_list<Vector<COL, T>>& u)
    {
        auto it = u.begin();
        for (size_t i = 0; i < ROW; ++i)
            row(i, *it++);
    }
    inline const T* operator[](size_t row) const
    {
        assert(row < ROW);
        return m[row];
    }

    inline T* operator[](size_t row)
    {
        assert(row < ROW);
        return m[row];
    }

    // 取一行
    inline Vector<COL, T> row(size_t row) const
    {
        assert(row < ROW);
        Vector<COL, T> a;
        for (size_t i = 0; i < COL; ++i)
            a[i] = m[row][i];
        return a;
    }

    // 取一列
    inline Vector<ROW, T> col(size_t col) const
    {
        assert(col < COL);
        Vector<ROW, T> a;
        for (size_t i = 0; i < ROW; ++i)
            a[i] = m[i][col];
        return a;
    }

    // 设置一行
    inline void setRow(size_t row, const Vector<COL, T>& a)
    {
        assert(row < ROW);
        for (size_t i = 0; i < COL; ++i)
            m[row][i] = a[i];
    }

    // 设置一列
    inline void setCol(size_t col, const Vector<ROW, T>& a)
    {
        assert(col < COL);
        for (size_t i = 0; i < ROW; ++i)
            m[i][col] = a[i];
    }

    // 取得删除某行和某列的子矩阵：子式
    inline Matrix<ROW - 1, COL - 1, T> minor(size_t row, size_t col) const
    {
        Matrix<ROW - 1, COL - 1, T> ret;
        for (size_t r = 0; r < ROW - 1; r++)
        {
            for (size_t c = 0; c < COL - 1; c++)
            {
                ret.m[r][c] = m[r < row ? r : r + 1][c < col ? c : c + 1];
            }
        }
        return ret;
    }

    // 取得转置矩阵
    inline Matrix<COL, ROW, T> transpose() const
    {
        Matrix<COL, ROW, T> ret;
        for (size_t r = 0; r < ROW; r++)
        {
            for (size_t c = 0; c < COL; c++)
                ret.m[c][r] = m[r][c];
        }
        return ret;
    }

    // 取得 0 矩阵
    inline static Matrix<ROW, COL, T> zero()
    {
        Matrix<ROW, COL, T> ret{};
        std::memset(&ret.m[0], 0, sizeof(ret.m));
        return ret;
    }

    // 取得单位矩阵
    inline static Matrix<ROW, COL, T> identity()
    {
        auto ret = Matrix::zero();
        for (size_t r = 0; r < ROW; r++)
        {
            for (size_t c = 0; c < COL; c++)
                ret.m[r][c] = (r == c) ? 1 : 0;
        }
        return ret;
    }
};

/// 数学库：矩阵运算
// ==m
template <size_t ROW, size_t COL, typename T>
inline bool operator==(const Matrix<ROW, COL, T>& a, const Matrix<ROW, COL, T>& b)
{
    for (size_t r = 0; r < ROW; r++)
    {
        for (size_t c = 0; c < COL; c++)
        {
            if (a.m[r][c] != b.m[r][c]) return false;
        }
    }
    return true;
}

// !=m
template <size_t ROW, size_t COL, typename T>
inline bool operator!=(const Matrix<ROW, COL, T>& a, const Matrix<ROW, COL, T>& b)
{
    return !(a == b);
}

// +m
template <size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> operator+(const Matrix<ROW, COL, T>& src)
{
    return src;
}

// -m
template <size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> operator-(const Matrix<ROW, COL, T>& src)
{
    Matrix<ROW, COL, T> out;
    for (size_t j = 0; j < ROW; ++j)
    {
        for (size_t i = 0; i < COL; ++i)
            out.m[j][i] = -src.m[j][i];
    }
    return out;
}

// m1 + m2
template <size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> operator+(const Matrix<ROW, COL, T>& a, const Matrix<ROW, COL, T>& b)
{
    Matrix<ROW, COL, T> out;
    for (size_t j = 0; j < ROW; ++j)
    {
        for (size_t i = 0; i < COL; ++i)
            out.m[j][i] = a.m[j][i] + b.m[j][i];
    }
    return out;
}

// m1 - m2
template <size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> operator-(const Matrix<ROW, COL, T>& a, const Matrix<ROW, COL, T>& b)
{
    Matrix<ROW, COL, T> out;
    for (size_t j = 0; j < ROW; ++j)
    {
        for (size_t i = 0; i < COL; ++i)
            out.m[j][i] = a.m[j][i] - b.m[j][i];
    }
    return out;
}

// m1 * m2
template <size_t ROW, size_t COL, size_t NEWCOL, typename T>
inline Matrix<ROW, NEWCOL, T> operator*(const Matrix<ROW, COL, T>& a, const Matrix<COL, NEWCOL, T>& b)
{
    Matrix<ROW, NEWCOL, T> out;
    for (size_t j = 0; j < ROW; ++j)
    {
        for (size_t i = 0; i < NEWCOL; ++i)
        {
            out.m[j][i] = vectorDot(a.row(j), b.col(i));
        }
    }
    return out;
}

// m * a
template <size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> operator*(const Matrix<ROW, COL, T>& a, T x)
{
    Matrix<ROW, COL, T> out;
    for (size_t j = 0; j < ROW; j++)
    {
        for (size_t i = 0; i < COL; i++)
        {
            out.m[j][i] = a.m[j][i] * x;
        }
    }
    return out;
}

// m / a
template <size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> operator/(const Matrix<ROW, COL, T>& a, T x)
{
    Matrix<ROW, COL, T> out;
    for (size_t j = 0; j < ROW; j++)
    {
        for (size_t i = 0; i < COL; i++)
        {
            out.m[j][i] = a.m[j][i] / x;
        }
    }
    return out;
}

// a * m
template <size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> operator*(T x, const Matrix<ROW, COL, T>& a)
{
    return (a * x);
}

template <size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> operator/(T x, const Matrix<ROW, COL, T>& a)
{
    Matrix<ROW, COL, T> out;
    for (size_t j = 0; j < ROW; j++)
    {
        for (size_t i = 0; i < COL; i++)
        {
            out.m[j][i] = x / a.m[j][i];
        }
    }
    return out;
}

template <size_t ROW, size_t COL, typename T>
inline Vector<COL, T> operator*(const Vector<ROW, T>& a, const Matrix<ROW, COL, T>& m)
{
    Vector<COL, T> b;
    for (size_t i = 0; i < COL; i++)
        b[i] = vectorDot(a, m.col(i));
    return b;
}

template <size_t ROW, size_t COL, typename T>
inline Vector<ROW, T> operator*(const Matrix<ROW, COL, T>& m, const Vector<COL, T>& a)
{
    Vector<ROW, T> b;
    for (size_t i = 0; i < ROW; i++)
        b[i] = vectoDdot(a, m.row(i));
    return b;
}

/// 数学库：行列式和逆矩阵等，光照计算有用
// 行列式求值：一阶
template <typename T>
inline T matrixDet(const Matrix<1, 1, T>& m)
{
    return m[0][0];
}

// 行列式求值：二阶
template <typename T>
inline T matrixDet(const Matrix<2, 2, T>& m)
{
    return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}

// 行列式求值：多阶行列式，即第一行同他们的余子式相乘求和
template <size_t N, typename T>
inline T matrixDet(const Matrix<N, N, T>& m)
{
    T sum = 0;
    for (size_t i = 0; i < N; i++)
        sum += m[0][i] * matrixCofactor(m, 0, i);
    return sum;
}

// 余子式：一阶
template <typename T>
inline T matrixCofactor(const Matrix<1, 1, T>& m, size_t row, size_t col)
{
    return 0;
}

// 多阶余子式：即删除特定行列的子式的行列式值
template <size_t N, typename T>
inline T matrixCofactor(const Matrix<N, N, T>& m, size_t row, size_t col)
{
    return matrixDet(m.minor(row, col)) * (((row + col) % 2) ? -1 : 1);
}

// 伴随矩阵：即余子式矩阵的转置
template <size_t N, typename T>
inline Matrix<N, N, T> matrixAdjoint(const Matrix<N, N, T>& m)
{
    Matrix<N, N, T> ret;
    for (size_t j = 0; j < N; j++)
    {
        for (size_t i = 0; i < N; i++)
            ret[j][i] = matrixCofactor(m, i, j);
    }
    return ret;
}

// 求逆矩阵：使用伴随矩阵除以行列式的值得到
template <size_t N, typename T>
inline Matrix<N, N, T> matrixInvert(const Matrix<N, N, T>& m)
{
    Matrix<N, N, T> ret = matrixAdjoint(m);
    T det = vectorDot(m.row(0), ret.col(0));
    return ret / det;
}

// 输出到文本流
template <size_t ROW, size_t COL, typename T>
inline std::ostream& operator<<(std::ostream& os, const Matrix<ROW, COL, T>& m)
{
    os << "[\n";
    for (size_t r = 0; r < ROW; r++)
    {
        Vector<COL, T> row = m.row(r);
        os << row << std::endl;
    }
    os << "]\n";
    return os;
}

/// 工具函数
template <typename T>
inline bool nearEqual(T x, T y, T error)
{
    return (std::abs(x - y) < error);
}

template <typename T>
inline T Between(T xmin, T xmax, T x)
{
    return std::min(std::max(xmin, x), xmax);
}

/// 类型别名
using Vec2f = Vector<2, float>;
using Vec2d = Vector<2, double>;
using Vec2i = Vector<2, int>;
using Vec3f = Vector<3, float>;
using Vec3d = Vector<3, double>;
using Vec3i = Vector<3, int>;
using Vec4f = Vector<4, float>;
using Vec4d = Vector<4, double>;
using Vec4i = Vector<4, int>;

using Mat4x4f = Matrix<4, 4, float>;
using Mat3x3f = Matrix<3, 3, float>;
using Mat4x3f = Matrix<4, 3, float>;
using Mat3x4f = Matrix<3, 4, float>;

/// 3D 数学运算
// 矢量转整数颜色
inline static uint32_t vectorToColor(const Vec4f& color)
{
    uint32_t r = (uint32_t)std::clamp((int)(color.r * 255.0f), 0, 255);
    uint32_t g = (uint32_t)std::clamp((int)(color.g * 255.0f), 0, 255);
    uint32_t b = (uint32_t)std::clamp((int)(color.b * 255.0f), 0, 255);
    uint32_t a = (uint32_t)std::clamp((int)(color.a * 255.0f), 0, 255);
    return (r << 16) | (g << 8) | b | (a << 24);
}

// 矢量转换整数颜色
inline static uint32_t vectorToColor(const Vec3f& color)
{
    return vectorToColor(color.xyz1());
}

// 整数颜色到矢量
inline static Vec4f vectorFromColor(uint32_t rgba)
{
    Vec4f out;
    out.r = (float)((rgba >> 16) & 0xff) / 255.0f;
    out.g = (float)((rgba >> 8) & 0xff) / 255.0f;
    out.b = (float)((rgba >> 0) & 0xff) / 255.0f;
    out.a = (float)((rgba >> 24) & 0xff) / 255.0f;
    return out;
}

inline static Mat4x4f matrixSetTranslate(float x, float y, float z)
{
    auto m = Matrix<4, 4, float>::identity();
    m.m[3][0] = x;
    m.m[3][1] = y;
    m.m[3][2] = z;
    return m;
}

// 缩放变换
inline static Mat4x4f matrixSetScale(float x, float y, float z)
{
    auto m = Mat4x4f::identity();
    m.m[0][0] = x;
    m.m[1][1] = y;
    m.m[2][2] = z;
    return m;
}

// 旋转变换，围绕 (x, y, z) 矢量旋转 theta 角度
inline static Mat4x4f matrixSetRotate(float x, float y, float z, float theta)
{
    auto qsin = (float)sin(theta * 0.5f);
    auto qcos = (float)cos(theta * 0.5f);
    float w = qcos;
    Vec3f vec = vectorNormalize(Vec3f(x, y, z));
    x = vec.x * qsin;
    y = vec.y * qsin;
    z = vec.z * qsin;
    Mat4x4f m{};
    m.m[0][0] = 1 - 2 * y * y - 2 * z * z;
    m.m[1][0] = 2 * x * y - 2 * w * z;
    m.m[2][0] = 2 * x * z + 2 * w * y;
    m.m[0][1] = 2 * x * y + 2 * w * z;
    m.m[1][1] = 1 - 2 * x * x - 2 * z * z;
    m.m[2][1] = 2 * y * z - 2 * w * x;
    m.m[0][2] = 2 * x * z - 2 * w * y;
    m.m[1][2] = 2 * y * z + 2 * w * x;
    m.m[2][2] = 1 - 2 * x * x - 2 * y * y;
    m.m[0][3] = m.m[1][3] = m.m[2][3] = 0.0f;
    m.m[3][0] = m.m[3][1] = m.m[3][2] = 0.0f;
    m.m[3][3] = 1.0f;
    return m;
}

// 摄影机变换矩阵：eye/视点位置，at/看向哪里，up/指向上方的矢量
inline static Mat4x4f matrixSetLookat(const Vec3f& eye, const Vec3f& at, const Vec3f& up)
{
    Vec3f zaxis = vectorNormalize(at - eye);
    Vec3f xaxis = vectorNormalize(vectorCross(up, zaxis));
    Vec3f yaxis = vectorCross(zaxis, xaxis);
    Mat4x4f m;
    m.setCol(0, Vec4f(xaxis.x, xaxis.y, xaxis.z, -vectorDot(eye, xaxis)));
    m.setCol(1, Vec4f(yaxis.x, yaxis.y, yaxis.z, -vectorDot(eye, yaxis)));
    m.setCol(2, Vec4f(zaxis.x, zaxis.y, zaxis.z, -vectorDot(eye, zaxis)));
    m.setCol(3, Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
    return m;
}

// D3DXMatrixPerspectiveFovLH
inline static Mat4x4f matrixSetPerspective(float fovy, float aspect, float zn, float zf)
{
    float fax = 1.0f / (float)tan(fovy * 0.5f);
    Mat4x4f m = Mat4x4f::zero();
    m.m[0][0] = (float)(fax / aspect);
    m.m[1][1] = (float)(fax);
    m.m[2][2] = zf / (zf - zn);
    m.m[3][2] = -zn * zf / (zf - zn);
    m.m[2][3] = 1;
    return m;
}

///// 位图库：用于加载/保存图片，画点，画线，颜色读取
class Bitmap
{
public:
    struct Info
    {
        uint32_t biSize;
        uint32_t biWidth;
        int32_t biHeight;
        uint16_t biPlanes;
        uint16_t biBitCount;
        uint32_t biCompression;
        uint32_t biSizeImage;
        uint32_t biXPelsPerMeter;
        uint32_t biYPelsPerMeter;
        uint32_t biClrUsed;
        uint32_t biClrImportant;
    };

public:
    Bitmap(uint32_t width, uint32_t height);
    Bitmap(const Bitmap& src);
    ~Bitmap();
    Bitmap(const char* filename);
    void fill(uint32_t color);

    inline int32_t width() const { return m_width; }
    inline int32_t height() const { return m_height; }
    inline int32_t pitch() const { return m_pitch; }
    inline uint8_t* bits() { return m_bits; }
    inline uint8_t* getLine(int y) { return m_bits + m_pitch * y; }
    inline const uint8_t* getLine(int y) const { return m_bits + m_pitch * y; }
    uint32_t getPixel(int x, int y) const;
    void setPixel(int x, int y, uint32_t color);

    void drawLine(int x1, int y1, int x2, int y2, uint32_t color);

    // 读取 BMP 图片，支持 24/32 位两种格式
    static Bitmap* loadFile(const char* filename);
    // 保存 BMP 图片
    bool saveFile(const char* filename, bool withAlpha = false) const;
    // 双线性插值
    uint32_t sampleBilinear(float x, float y) const;
    // 纹理采样
    Vec4f sample2D(float u, float v) const;
    // 纹理采样：直接传入 Vec2f
    Vec4f sample2D(const Vec2f& uv) const;
    // 按照 Vec4f 画点
    void setPixel(int x, int y, const Vec4f& color);
    // 上下反转
    void flipVertical();
    // 水平反转
    void flipHorizontal();

protected:
    // 双线性插值计算：给出四个点的颜色，以及坐标偏移，计算结果
    static uint32_t bilinearInterp(uint32_t tl, uint32_t tr, uint32_t bl, uint32_t br, int32_t distx, int32_t disty);

private:
    int32_t m_width;
    int32_t m_height;
    int32_t m_pitch;
    uint8_t* m_bits;
};

/// 着色器定义
// 着色器上下文，由 VS 设置，再由渲染器按像素逐点插值后，供 PS 读取
struct ShaderContext
{
    std::map<int, float> varyingFloat; // 浮点数 varying 列表
    std::map<int, Vec2f> varyingVec2f; // 二维矢量 varying 列表
    std::map<int, Vec3f> varyingVec3f; // 三维矢量 varying 列表
    std::map<int, Vec4f> varyingVec4f; // 四维矢量 varying 列表
};

// 顶点着色器：因为是 C++ 编写，无需传递 attribute，传个 0-2 的顶点序号
// 着色器函数直接在外层根据序号读取相应数据即可，最后需要返回一个坐标 pos
typedef std::function<Vec4f(int index, ShaderContext& output)> VertexShader;

// 片元着色器：输入 ShaderContext，需要返回 Vec4f 类型的颜色
typedef std::function<Vec4f(ShaderContext& input)> FragmentShader;

//// 简单渲染器实现：
class RenderHelp
{
public:
    // 顶点结构体
    struct Vertex
    {
        ShaderContext context; // 上下文
        float rhw{};           // w 的倒数
        Vec4f pos;             // 坐标
        Vec2f spf;             // 浮点数屏幕坐标
        Vec2i spi;             // 整数屏幕坐标
    };

    struct Range
    {
        int left{};
        int right{};
        int top{};
        int bottom{};
    };
    RenderHelp();
    RenderHelp(int32_t width, int32_t height);
    ~RenderHelp();
    // 复位状态
    void reset();
    // 初始化 FrameBuffer，渲染前需要先调用
    void init(int32_t width, int32_t height);
    // 清空 FrameBuffer 和深度缓存
    void clear();

    // 设置 VS/PS 着色器函数
    inline void setVertexShader(VertexShader vs) { m_vertexShader = std::move(vs); }
    inline void setFragmentShader(FragmentShader ps) { m_fragmentShader = std::move(ps); }
    // 保存 FrameBuffer 到 BMP 文件
    inline void saveFile(const char* filename)
    {
        if (m_frameBuffer) m_frameBuffer->saveFile(filename);
        std::cout << "save successfully" << std::endl;
    }

    // 设置背景/前景色
    inline void setBGColor(uint32_t color) { m_colorBg = color; }
    inline void setFGColor(uint32_t color) { m_colorFg = color; }

    // FrameBuffer 里画点
    inline void setPixel(int x, int y, uint32_t cc)
    {
        if (m_frameBuffer) m_frameBuffer->setPixel(x, y, cc);
    }
    inline void setPixel(int x, int y, const Vec4f& cc) { setPixel(x, y, vectorToColor(cc)); }
    inline void setPixel(int x, int y, const Vec3f& cc) { setPixel(x, y, vectorToColor(cc)); }
    // FrameBuffer 里画线
    inline void drawLine(int x1, int y1, int x2, int y2)
    {
        if (m_frameBuffer) m_frameBuffer->drawLine(x1, y1, x2, y2, m_colorFg);
    }
    // 设置渲染状态，是否显示线框图，是否填充三角形
    inline void setRenderState(bool frame, bool pixel)
    {
        m_renderFrame = frame;
        m_renderPixel = pixel;
    }
    // 判断一条边是不是三角形的左上边 (Top-Left Edge)
    inline bool isTopLeft(const Vec2i& a, const Vec2i& b)
    {
        return ((a.y == b.y) && (a.x < b.x)) || (a.y > b.y);
    }
    bool drawPrimitive();

private:
    Range m_range;                   // 三角形外接矩形
    Vertex m_vertices[3];            // 顶点坐标
    Bitmap* m_frameBuffer = nullptr; // 像素缓存
    float** m_depthBuffer = nullptr; // 深度缓存
    int m_width = 0;
    int m_height = 0;
    uint32_t m_colorFg = 0;     // 前景色：画线时候用
    uint32_t m_colorBg = 0;     // 背景色：Clear时候用
    bool m_renderFrame = false; // 是否绘制线框
    bool m_renderPixel = true;  // 是否填充像素
    VertexShader m_vertexShader;
    FragmentShader m_fragmentShader;
};
#endif //CPP_DEMO_RENDERHELP_H
