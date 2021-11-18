//
// Created by william on 2021/10/15.
//

#include <iostream>
#include <cmath>
#include <eigen3/Eigen/Core>

int eigen3Test()
{
    // Basic Example of cpp
    std::cout << "Example of cpp\n";
    float a = 1.0f, b = 2.0f;
    std::cout << a << std::endl;
    std::cout << a/b << std::endl;
    std::cout << std::sqrt(b) << std::endl;
    std::cout << std::acos(-1) << std::endl;
    std::cout << std::sin(30.0f/180.0f * std::acos(-1)) << std::endl;

    // Example of vector
    std::cout << "Example of vector\n";
    // vector definition
    Eigen::Vector3f v(1.0f, 2.0f, 3.0f);
    Eigen::Vector3f w(1.0f, 0.0f, 0.0f);
    // vector output
    std::cout << "Example of output\n";
    std::cout << v << std::endl;
    // vector add
    std::cout << "Example of add\n";
    std::cout << v + w << std::endl;
    // vector scalar multiply
    // scalar:纯量的
    std::cout << "Example of scalar multiply\n";
    std::cout << v * 3.f << std::endl;
    std::cout << 2.0f * v << std::endl;

    //Example of matrix
    std::cout << "Example of matrix \n";
    // matrix definition
    Eigen::Matrix3f i, j;
    i << 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f;
    j << 2.0f, 3.0f, 1.0f, 4.0f, 6.0f, 5.0f, 9.0f, 7.0f, 8.0f;
    // matrix output
    std::cout << "Example of matrix out\n";
    std::cout << i << std::endl << std::endl;
    std::cout << j << std::endl;
    std::cout << "matrix add:i + j:" << std::endl;
    std::cout << i + j << std::endl;
    std::cout << "matrix scalar: i * 2.0：" << std::endl;
    std::cout << i * 2.0 << std::endl;
    std::cout << "matrix multiply:i * j:" << std::endl;
    std::cout << i * j << std::endl;
    std::cout << "matrix multiply vector:i * v" << std::endl;
    std::cout << i * v << std::endl;
    std::cout << "homework:\n";
    Eigen::Vector3f p(2.0f, 1.0f, 1.0f);
    Eigen::Matrix3f rotateMatrix, translateMatrix;
    float rotateAngle = 45.0f / 180.0f * acos(-1);
    rotateMatrix << std::cosf(rotateAngle), -std::sinf(rotateAngle), 0.0f, std::sinf(rotateAngle), std::cosf(rotateAngle), 0.0f, 0.0f, 0.0f, 1.0f;
    // 矩阵按行存储
    translateMatrix << 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f;
    // p: 默认是列向量
    std::cout << "result:\n"<< translateMatrix * rotateMatrix * p << std::endl;
    return 0;
}