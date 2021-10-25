//
// Created by ytech on 2021/10/25.
//

#include "matrix4.h"
#include "base.h"

/// 计算视图矩阵
Matrix4f getViewMatrix(const Vector3f& eyePos)
{
    Matrix4f view = Matrix4f::Identity();
    Eigen::Matrix4f translate;
    translate << 1.0f, 0.0f, 0.0f, -eyePos[0],
        0.0f, 1.0f, 0.0f, -eyePos[1],
        0.0f, 0.0f, 1.0f, -eyePos[2],
        0.0f, 0.0f, 0.0f, 1.0f;
    view = translate * view;
    return view;
}

Matrix4f getModelMatrix(float rotationAngle)
{
    Matrix4f model = Matrix4f::Identity();
    if (rotationAngle == 0) return model;
    float rotation_radian = rotationAngle * MATH_PI / 180.0f;
    Matrix4f rotateZ;
    rotateZ << cos(rotation_radian), -sin(rotation_radian), 0.0f, 0.0f,
        sin(rotation_radian), cos(rotation_radian), 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f;
    model = model * rotateZ;

    return model;
}

/// 任意轴旋转矩阵
Matrix4f getRotation(Eigen::Vector3f axis, float angle)
{
    Matrix4f rotation = Matrix4f::Identity();
    angle = angle / 180.0f * MATH_PI;
    Matrix4f crossMatrix;
    crossMatrix << 0.0f, -axis[2], axis[1], 0.0f,
        axis[2], 0.0f, -axis[0], 0.0f,
        -axis[1], axis[0], 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f;
    rotation += sin(angle) * crossMatrix + (1 - cos(angle)) * crossMatrix * crossMatrix;
    return rotation;
}

/// 计算投影矩阵
Matrix4f getProjectionMatrix(float eyeFov, float aspectRatio, float zNear, float zFar)
{
    Matrix4f projection = Eigen::Matrix4f::Identity();

    Matrix4f proToOrt, translation, scaling;
    proToOrt << zNear, 0.0f, 0.0f, 0.0f,
        0.0f, zNear, 0.0f, 0.0f,
        0.0f, 0.0f, zNear + zFar, -zNear * zFar,
        0.0f, 0.0f, 1.0f, 0.0f;
    float theta = eyeFov / 360.0f * MATH_PI; //divide into 2，360=180*2
    float t = fabs(zNear) * tan(theta);      //top(y axis)
    float b = -t;                            //bottom
    float r = t * aspectRatio;               //right(x axis)
    float l = -r;                            //left

    scaling << 2.0f / (r - l), 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f / (t - b), 0.0f, 0.0f,
        0.0f, 0.0f, 2.0f / (zNear - zFar), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f;
    translation << 1.0f, 0.0f, 0.0f, -(r + l) / 2.0f,
        0.0f, 1.0f, 0.0f, -(t + b) / 2.0f,
        0.0f, 0.0f, 1.0f, -(zNear + zFar) / 2.0f,
        0.0f, 0.0f, 0.0f, 1.0f;
    projection = scaling * translation * proToOrt; //from left to right calculate
    return projection;
}