//
// Created by william on 2022/3/1.
//
#include <eigen3/Eigen/Eigen>
#include <iostream>
using namespace Eigen;

void eigenTest()
{
    // 解线性方程组 Ax = B；
    Matrix3f A;
    Vector3f b;
    A << 1,2,3,  4,5,6,  7,8,10;
    b << 3, 3, 4;
    std::cout << "The matrix A:\n" << A << std::endl;
    std::cout << "The vector b:\n" << b << std::endl;
    Vector3f x = A.colPivHouseholderQr().solve(b);
    std::cout << "The solution is:\n" << x << std::endl;
}
