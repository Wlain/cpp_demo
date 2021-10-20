//
// Created by william on 2021/10/18.
//
#include "rasterizer.h"

#include <eigen3/Eigen/Eigen>
#include <opencv2/opencv.hpp>
using namespace Eigen;
#define MATH_PI 3.1415926

namespace rst
{
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
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();
    float rotation_radian = rotationAngle * MATH_PI / 180.0f;
    Eigen::Matrix4f rotateZ;
    rotateZ << cos(rotation_radian), -sin(rotation_radian), 0.0f, 0.0f,
        sin(rotation_radian), cos(rotation_radian), 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f;
    model = model * rotateZ;

    return model;
}

Matrix4f getProjectionMatrix(float eyeFov, float aspectRatio, float zNear, float zFar)
{
    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();
    double d = 1.0f / tan((eyeFov / 2.0f) * MATH_PI / 180.0f);
    double A = -(zFar + zNear) / (zFar - zNear);
    double B = -2.0f * zFar * zNear / (zFar - zNear);
    projection << d / aspectRatio, 0, 0, 0,
        0, d, 0, 0,
        0, 0, A, B,
        0, 0, -1, 0;
    return projection;
}

// 实现简单的直线扫描算法，绘制三角形线框
void Assignment1()
{
    Rasterizer rasterizer(800, 800);
    Vector3f eyePos = { 0.0f, 0.0f, 5.0f };
    std::vector<Vector3f> position = { { 2.0f, 0.0f, -2.0f }, { 0.0f, 2.0f, -2.0f }, { -2.0f, 0.0f, -2.0f } };
    std::vector<Vector4f> color = { { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f}, { 0.0f, 0.0f, 1.0f, 1.0f } };
    std::vector<Eigen::Vector3i> indices = { { 0, 1, 2 } };
    auto posId = rasterizer.loadPositions(position);
    auto colorID = rasterizer.loadColors(color);
    auto indicesId = rasterizer.loadIndices(indices);
    rasterizer.setModel(getModelMatrix(0));
    rasterizer.setView(getViewMatrix(eyePos));
    rasterizer.setProjection(getProjectionMatrix(45, 1, 0.1, 50));
    rasterizer.clearColor(1.0f, 0.0f, 0.0f, 1.0f);
    rasterizer.clear(Buffers::Color | Buffers::Depth);
    rasterizer.draw(posId, indicesId, colorID, Primitive::Triangle_Line);
    cv::Mat image(800, 800, CV_32FC3, rasterizer.frameBuffer().data());
    cv::imshow("triangle line", image);
    cv::waitKey();
}

void Assignment2()
{

}
} // namespace rst
