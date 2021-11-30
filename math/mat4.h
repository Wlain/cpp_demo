//
// Created by william on 2021/11/28.
//

#pragma once

#include "vec3.h"

#include <cstdlib>
#include <iomanip>

template <typename T>
class Mat4
{
public:
    Mat4() = default;

    ~Mat4() = default;

    Mat4(T a00, T a01, T a02, T a03, T a10, T a11, T a12, T a13,
         T a20, T a21, T a22, T a23, T a30, T a31, T a32, T a33)
    {
        m[0][0] = a00;
        m[0][1] = a01;
        m[0][2] = a02;
        m[0][3] = a03;
        m[1][0] = a10;
        m[1][1] = a11;
        m[1][2] = a12;
        m[1][3] = a13;
        m[2][0] = a20;
        m[2][1] = a21;
        m[2][2] = a22;
        m[2][3] = a23;
        m[3][0] = a30;
        m[3][1] = a31;
        m[3][2] = a32;
        m[3][3] = a33;
    }

    const T* operator[](uint8_t i) const { return m[i]; }

    T* operator[](uint8_t i) { return m[i]; }

    Mat4 operator*(const Mat4& v) const
    {
        Mat4 temp;
        multiply(*this, v, temp);
        return temp;
    }

    static void multiply(const Mat4& a, const Mat4& b, Mat4& dst)
    {
        const T* __restrict ap = &a.m[0][0];
        const T* __restrict bp = &b.m[0][0];
        T* __restrict cp = &dst.m[0][0];
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
        return Matrix44(m[0][0],
                        m[1][0],
                        m[2][0],
                        m[3][0],
                        m[0][1],
                        m[1][1],
                        m[2][1],
                        m[3][1],
                        m[0][2],
                        m[1][2],
                        m[2][2],
                        m[3][2],
                        m[0][3],
                        m[1][3],
                        m[2][3],
                        m[3][3]);
    }

    Mat4& transposed()
    {
        Mat4 tmp(m[0][0],
                 m[1][0],
                 m[2][0],
                 m[3][0],
                 m[0][1],
                 m[1][1],
                 m[2][1],
                 m[3][1],
                 m[0][2],
                 m[1][2],
                 m[2][2],
                 m[3][2],
                 m[0][3],
                 m[1][3],
                 m[2][3],
                 m[3][3]);
        *this = tmp;
        return *this;
    }

    template <typename S>
    void multVecMatrix(const Vec3<S>& src, Vec3<S>& dst) const
    {
        S a, b, c, w;
        a = src[0] * m[0][0] + src[1] * m[1][0] + src[2] * m[2][0] + m[3][0];
        b = src[0] * m[0][1] + src[1] * m[1][1] + src[2] * m[2][1] + m[3][1];
        c = src[0] * m[0][2] + src[1] * m[1][2] + src[2] * m[2][2] + m[3][2];
        w = src[0] * m[0][3] + src[1] * m[1][3] + src[2] * m[2][3] + m[3][3];

        dst.x = a / w;
        dst.y = b / w;
        dst.z = c / w;
    }

    template <typename S>
    void multDirMatrix(const Vec3<S>& src, Vec3<S>& dst) const
    {
        S a, b, c;

        a = src[0] * m[0][0] + src[1] * m[1][0] + src[2] * m[2][0];
        b = src[0] * m[0][1] + src[1] * m[1][1] + src[2] * m[2][1];
        c = src[0] * m[0][2] + src[1] * m[1][2] + src[2] * m[2][2];

        dst.x = a;
        dst.y = b;
        dst.z = c;
    }

    /// Gauss-Jordan 消元求逆
    Mat4 inverse()
    {
        Mat4 mat;
        for (unsigned column = 0; column < 4; ++column)
        {
            // Swap row in case our pivot point is not working
            if (m[column][column] == 0)
            {
                unsigned big = column;
                for (unsigned row = 0; row < 4; ++row)
                {
                    if (fabs(m[row][column]) > fabs(m[big][column])) big = row;
                }
                // Print this is a singular matrix, return identity ?
                if (big == column)
                {
                    fprintf(stderr, "Singular matrix\n");
                }
                // Swap rows
                else
                {
                    for (unsigned j = 0; j < 4; ++j)
                    {
                        std::swap(m[column][j], m[big][j]);
                        std::swap(mat.m[column][j], mat.m[big][j]);
                    }
                }
            }
            // Set each row in the column to 0
            for (unsigned row = 0; row < 4; ++row)
            {
                if (row != column)
                {
                    T coeff = m[row][column] / m[column][column];
                    if (coeff != 0)
                    {
                        for (unsigned j = 0; j < 4; ++j)
                        {
                            m[row][j] -= coeff * m[column][j];
                            mat.m[row][j] -= coeff * mat.m[column][j];
                        }
                        // Set the element to 0 for safety
                        m[row][column] = 0;
                    }
                }
            }
        }
        // Set each element of the diagonal to 1
        for (unsigned row = 0; row < 4; ++row)
        {
            for (unsigned column = 0; column < 4; ++column)
            {
                mat.m[row][column] /= m[row][row];
            }
        }
        *this = mat;
        return *this;
    }

    const Mat4& invert()
    {
        *this = inverse();
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& s, const Mat4& mat)
    {
        std::ios_base::fmtflags oldFlags = s.flags();
        int width = 12; // total with of the displayed number
        s.precision(5); // control the number of displayed decimals
        s.setf(std::ios_base::fixed);
        s << "[" << std::setw(width) << mat[0][0] << " " << std::setw(width) << mat[0][1] << " " << std::setw(width)
          << mat[0][2] << " " << std::setw(width) << mat[0][3] << "\n"
          << " " << std::setw(width) << mat[1][0] << " " << std::setw(width) << mat[1][1] << " " << std::setw(width)
          << mat[1][2] << " " << std::setw(width) << mat[1][3] << "\n"
          << " " << std::setw(width) << mat[2][0] << " " << std::setw(width) << mat[2][1] << " " << std::setw(width)
          << mat[2][2] << " " << std::setw(width) << mat[2][3] << "\n"
          << " " << std::setw(width) << mat[3][0] << " " << std::setw(width) << mat[3][1] << " " << std::setw(width)
          << mat[3][2] << " " << std::setw(width) << mat[3][3] << "]";
        s.flags(oldFlags);
        return s;
    }

    T m[4][4] = { { 1, 0, 0, 0 },
                  { 0, 1, 0, 0 },
                  { 0, 0, 1, 0 },
                  { 0, 0, 0, 1 } };
};

using Mat4f = Mat4<float>;