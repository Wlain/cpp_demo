//
// Created by ytech on 2021/10/25.
//

#ifndef CPP_DEMO_MATRIX4_H
#define CPP_DEMO_MATRIX4_H

#include <eigen3/Eigen/Eigen>
using namespace Eigen;
/// 计算视图矩阵
Matrix4f getViewMatrix(const Vector3f& eyePos);
/// 计算模型矩阵
Matrix4f getModelMatrix(float rotationAngle);
/// 任意轴旋转矩阵
Matrix4f getRotation(Eigen::Vector3f axis, float angle);
/// 计算投影矩阵
Matrix4f getProjectionMatrix(float eyeFov, float aspectRatio, float zNear, float zFar);

#endif //CPP_DEMO_MATRIX4_H
