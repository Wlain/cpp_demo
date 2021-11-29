//
// Created by william on 2021/11/28.
//

#pragma once
#include "vec3.h"
#include <iomanip>
#include <cstdlib>
template <typename T>
class Mat4
{
public:
    Mat4() = default;
    ~Mat4() = default;
    Mat4(T a00, T a01, T a02, T a03, T a10, T a11, T a12, T a13,
         T a20, T a21, T a22, T a23, T a30, T a31, T a32, T a33)
    {
        x[0][0] = a00;
        x[0][1] = a01;
        x[0][2] = a02;
        x[0][3] = a03;
        x[1][0] = a10;
        x[1][1] = a11;
        x[1][2] = a12;
        x[1][3] = a13;
        x[2][0] = a20;
        x[2][1] = a21;
        x[2][2] = a22;
        x[2][3] = a23;
        x[3][0] = a30;
        x[3][1] = a31;
        x[3][2] = a32;
        x[3][3] = a33;
    }
    const T* operator[](uint8_t i) const { return x[i]; }
    T* operator[](uint8_t i) { return x[i]; }
    Mat4 operator*(const Mat4& v) const
    {
        Mat4 temp;
        multiply(*this, v, temp);
        return temp;
    }

    static void multiply(const Mat4& a, const Mat4& b, Mat4& dst)
    {
        const T* __restrict ap = &a.x[0][0];
        const T* __restrict bp = &b.x[0][0];
        T* __restrict cp = &dst.x[0][0];
        T a0, a1, a2, a3;
        a0 = ap[0];
        a1 = ap[1];
        a2 = ap[2];
        a3 = ap[3];
        cp[0] = a0 * bp[0] + a1 * bp[4] + a2 * bp[8] + a3 * bp[12];
        cp[1] = a0 * bp[1] + a1 * bp[5] + a2 * bp[9] + a3 * bp[13];
        cp[2] = a0 * bp[2] + a1 * bp[6] + a2 * bp[10] + a3 * bp[14];
        cp[3] = a0 * bp[3] + a1 * bp[7] + a2 * bp[11] + a3 * bp[15];
        a0 = ap[4];
        a1 = ap[5];
        a2 = ap[6];
        a3 = ap[7];
        cp[4] = a0 * bp[0] + a1 * bp[4] + a2 * bp[8] + a3 * bp[12];
        cp[5] = a0 * bp[1] + a1 * bp[5] + a2 * bp[9] + a3 * bp[13];
        cp[6] = a0 * bp[2] + a1 * bp[6] + a2 * bp[10] + a3 * bp[14];
        cp[7] = a0 * bp[3] + a1 * bp[7] + a2 * bp[11] + a3 * bp[15];
        a0 = ap[8];
        a1 = ap[9];
        a2 = ap[10];
        a3 = ap[11];
        cp[8] = a0 * bp[0] + a1 * bp[4] + a2 * bp[8] + a3 * bp[12];
        cp[9] = a0 * bp[1] + a1 * bp[5] + a2 * bp[9] + a3 * bp[13];
        cp[10] = a0 * bp[2] + a1 * bp[6] + a2 * bp[10] + a3 * bp[14];
        cp[11] = a0 * bp[3] + a1 * bp[7] + a2 * bp[11] + a3 * bp[15];
        a0 = ap[12];
        a1 = ap[13];
        a2 = ap[14];
        a3 = ap[15];
        cp[12] = a0 * bp[0] + a1 * bp[4] + a2 * bp[8] + a3 * bp[12];
        cp[13] = a0 * bp[1] + a1 * bp[5] + a2 * bp[9] + a3 * bp[13];
        cp[14] = a0 * bp[2] + a1 * bp[6] + a2 * bp[10] + a3 * bp[14];
        cp[15] = a0 * bp[3] + a1 * bp[7] + a2 * bp[11] + a3 * bp[15];
    }

    Mat4 transposed() const
    {
        return Matrix44(x[0][0],
                        x[1][0],
                        x[2][0],
                        x[3][0],
                        x[0][1],
                        x[1][1],
                        x[2][1],
                        x[3][1],
                        x[0][2],
                        x[1][2],
                        x[2][2],
                        x[3][2],
                        x[0][3],
                        x[1][3],
                        x[2][3],
                        x[3][3]);
    }

    Mat4& transposed()
    {
        Mat4 tmp(x[0][0],
                 x[1][0],
                 x[2][0],
                 x[3][0],
                 x[0][1],
                 x[1][1],
                 x[2][1],
                 x[3][1],
                 x[0][2],
                 x[1][2],
                 x[2][2],
                 x[3][2],
                 x[0][3],
                 x[1][3],
                 x[2][3],
                 x[3][3]);
        *this = tmp;
        return *this;
    }

    template <typename S>
    void multVecMatrix(const Vec3<S>& src, Vec3<S>& dst) const
    {
        S a, b, c, w;
        a = src[0] * x[0][0] + src[1] * x[1][0] + src[2] * x[2][0] + x[3][0];
        b = src[0] * x[0][1] + src[1] * x[1][1] + src[2] * x[2][1] + x[3][1];
        c = src[0] * x[0][2] + src[1] * x[1][2] + src[2] * x[2][2] + x[3][2];
        w = src[0] * x[0][3] + src[1] * x[1][3] + src[2] * x[2][3] + x[3][3];

        dst.x = a / w;
        dst.y = b / w;
        dst.z = c / w;
    }

    template <typename S>
    void multDirMatrix(const Vec3<S>& src, Vec3<S>& dst) const
    {
        S a, b, c;

        a = src[0] * x[0][0] + src[1] * x[1][0] + src[2] * x[2][0];
        b = src[0] * x[0][1] + src[1] * x[1][1] + src[2] * x[2][1];
        c = src[0] * x[0][2] + src[1] * x[1][2] + src[2] * x[2][2];

        dst.x = a;
        dst.y = b;
        dst.z = c;
    }
    /// Gauss-Jordan 消元求逆
    Mat4 inverse() const
    {
        int i, j, k;
        Mat4 s;
        Mat4 t(*this);

        // Forward elimination
        for (i = 0; i < 3; i++)
        {
            int pivot = i;
            /// 寻找主元
            T pivotsize = t[i][i];

            if (pivotsize < 0)
            {
                pivotsize = -pivotsize;
            }

            for (j = i + 1; j < 4; j++)
            {
                T tmp = t[j][i];

                if (tmp < 0)
                    tmp = -tmp;

                if (tmp > pivotsize)
                {
                    pivot = j;
                    pivotsize = tmp;
                }
            }

            if (pivotsize == 0)
            {
                // Cannot invert singular matrix
                return Mat4();
            }
            /// 互换主元
            if (pivot != i)
            {
                for (j = 0; j < 4; j++)
                {
                    T tmp;

                    tmp = t[i][j];
                    t[i][j] = t[pivot][j];
                    t[pivot][j] = tmp;

                    tmp = s[i][j];
                    s[i][j] = s[pivot][j];
                    s[pivot][j] = tmp;
                }
            }

            for (j = i + 1; j < 4; j++)
            {
                T f = t[j][i] / t[i][i];

                for (k = 0; k < 4; k++)
                {
                    t[j][k] -= f * t[i][k];
                    s[j][k] -= f * s[i][k];
                }
            }
        }

        // Backward substitution
        for (i = 3; i >= 0; --i)
        {
            T f;

            if ((f = t[i][i]) == 0)
            {
                // Cannot invert singular matrix
                return Mat4();
            }

            for (j = 0; j < 4; j++)
            {
                t[i][j] /= f;
                s[i][j] /= f;
            }

            for (j = 0; j < i; j++)
            {
                f = t[j][i];

                for (k = 0; k < 4; k++)
                {
                    t[j][k] -= f * t[i][k];
                    s[j][k] -= f * s[i][k];
                }
            }
        }

        return s;
    }

    const Mat4& invert()
    {
        *this = inverse();
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& s, const Mat4& m)
    {
        std::ios_base::fmtflags oldFlags = s.flags();
        int width = 12; // total with of the displayed number
        s.precision(5); // control the number of displayed decimals
        s.setf(std::ios_base::fixed);
        s << "[" << std::setw(width) << m[0][0] << " " << std::setw(width) << m[0][1] << " " << std::setw(width) << m[0][2] << " " << std::setw(width) << m[0][3] << "\n"
          << " " << std::setw(width) << m[1][0] << " " << std::setw(width) << m[1][1] << " " << std::setw(width) << m[1][2] << " " << std::setw(width) << m[1][3] << "\n"
          << " " << std::setw(width) << m[2][0] << " " << std::setw(width) << m[2][1] << " " << std::setw(width) << m[2][2] << " " << std::setw(width) << m[2][3] << "\n"
          << " " << std::setw(width) << m[3][0] << " " << std::setw(width) << m[3][1] << " " << std::setw(width) << m[3][2] << " " << std::setw(width) << m[3][3] << "]";
        s.flags(oldFlags);
        return s;
    }

    T x[4][4] = { { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } };
};

using Mat4f = Mat4<float>;