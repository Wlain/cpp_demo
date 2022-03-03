//
// Created by william on 2022/3/1.
//
#include <eigen3/Eigen/Eigen>
#include <iostream>
using namespace Eigen;

void eigenTest()
{
    MatrixXf A(4, 4);
    float data[4][4] = {
        { 1.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f }
    };

    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            A(row, col) = data[row][col];
        }
    }

    MatrixXf B(4, 2);
    for (int row = 0; row < 4; ++row)
    {
        B(row, 0) = 1;
        B(row, 1) = 2;
    }
    // 解线性方程组 Ax = B；
    MatrixXf x = A.colPivHouseholderQr().solve(B);
    std::cout << x << std::endl;
}
