//
// Created by william on 2021/10/18.
//
#include "base.h"
#include "rasterizer.h"

#include <eigen3/Eigen/Eigen>
#include <opencv2/opencv.hpp>
using namespace Eigen;

namespace graphics
{
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

/// 计算模型矩阵
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
    Matrix4f projection = Matrix4f::Identity();
    double D = 1.0f / tan((eyeFov / 2.0f) * MATH_PI / 180.0f);
    double A = -(zFar + zNear) / (zFar - zNear);
    double B = -2.0f * zFar * zNear / (zFar - zNear);
    projection << D / aspectRatio, 0, 0, 0,
        0, D, 0, 0,
        0, 0, A, B,
        0, 0, -1, 0;

    return projection;
}

// 顶点着色器
Vector3f baseVertexShader(const VertexShader& vertShader)
{
    return vertShader.position();
}

// 片元着色器
Vector3f baseFragShader(const FragmentShader& fragShader)
{
    return { 1.0, 1.0, 1.0f };
}

/// 实现简单的直线扫描算法，绘制三角形线框
void assignment1()
{
    Rasterizer rasterizer(800, 800);
    Vector3f eyePos = { 0.0f, 0.0f, 5.0f };
    std::vector<Vector3f> position = { { 2.0f, 0.0f, -2.0f }, { 0.0f, 2.0f, -2.0f }, { -2.0f, 0.0f, -2.0f } };
    std::vector<Vector4f> color = { { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } };
    std::vector<Eigen::Vector3i> indices = { { 0, 1, 2 } };
    auto posId = rasterizer.loadPositions(position);
    auto colorID = rasterizer.loadColors(color);
    auto indicesId = rasterizer.loadIndices(indices);
    rasterizer.setModel(getRotation({ 1, 1, 0 }, 0));
    rasterizer.setView(getViewMatrix(eyePos));
    rasterizer.setProjection(getProjectionMatrix(45, 1, 0.1, 50));
    rasterizer.clearColor(1.0f, 0.0f, 0.0f, 1.0f);
    rasterizer.clear(Buffers::Color | Buffers::Depth);
    rasterizer.draw(posId, indicesId, colorID, Primitive::Triangle_Line);
    cv::Mat image(800, 800, CV_32FC3, rasterizer.frameBuffer().data());
    cv::imshow("triangle line", image);
    cv::waitKey();
}

/// 实现三角形深度遮挡算法
void assignment2()
{
    Rasterizer rasterizer(800, 800);
    Vector3f eyePos = { 0.0f, 0.0f, 5.0f };
    std::vector<Eigen::Vector3f> position = {
        { 2.0f, 0.0f, -2.0f },
        { 0.0f, 2.0f, -2.0f },
        { -2.0f, 0.0f, -2.0f },
        { 3.5f, -1.0f, -5.0f },
        { 2.5f, 2.5f, -5.0f },
        { -1.0f, 0.5f, -5.0f }
    };
    std::vector<Eigen::Vector4f> colors = {
        { 217.0f / 255.0f, 238.0f / 255.0f, 185.0f / 255.0f, 1.0f },
        { 217.0f / 255.0f, 238.0f / 255.0f, 185.0f / 255.0f, 1.0f },
        { 217.0f / 255.0f, 238.0f / 255.0f, 185.0f / 255.0f, 1.0f },
        { 185.0f / 255.0f, 217.0f / 255.0f, 238.0f / 255.0f, 1.0f },
        { 185.0f / 255.0f, 217.0f / 255.0f, 238.0f / 255.0f, 1.0f },
        { 185.0f / 255.0f, 217.0f / 255.0f, 238.0f / 255.0f, 1.0f }
    };
    std::vector<Eigen::Vector3i> indices = {
        { 0, 1, 2 },
        { 3, 4, 5 }
    };
    auto posId = rasterizer.loadPositions(position);
    auto colorID = rasterizer.loadColors(colors);
    auto indicesId = rasterizer.loadIndices(indices);
    rasterizer.setModel(getModelMatrix(20));
    rasterizer.setView(getViewMatrix(eyePos));
    rasterizer.setProjection(getProjectionMatrix(45, 1, 0.1, 50));
    rasterizer.clearColor(0.0f, 0.0f, 0.0f, 1.0f);
    rasterizer.clear(Buffers::Color | Buffers::Depth);
    rasterizer.setMsaaRatio(4.0f);
    rasterizer.draw(posId, indicesId, colorID, Primitive::Triangle);
    cv::Mat image(800, 800, CV_32FC3, rasterizer.frameBuffer().data());
    cv::imshow("triangles", image);
    cv::waitKey();
}

void assignment3()
{
    Rasterizer rasterizer(800, 800);
    Vector3f eyePos = { 0.0f, 0.0f, 5.0f };
    std::vector<Eigen::Vector3f> position = {
        { 2.0f, 0.0f, -2.0f },
        { 0.0f, 2.0f, -2.0f },
        { -2.0f, 0.0f, -2.0f }
    };
    std::vector<Eigen::Vector4f> colors = {
        { 217.0f / 255.0f, 238.0f / 255.0f, 185.0f / 255.0f, 1.0f },
        { 217.0f / 255.0f, 238.0f / 255.0f, 185.0f / 255.0f, 1.0f },
        { 217.0f / 255.0f, 238.0f / 255.0f, 185.0f / 255.0f, 1.0f }
    };
    std::vector<Eigen::Vector3i> indices = {
        { 0, 1, 2 },
    };
    auto posId = rasterizer.loadPositions(position);
    auto colorID = rasterizer.loadColors(colors);
    auto indicesId = rasterizer.loadIndices(indices);
    rasterizer.setVertexShader(baseVertexShader);
    rasterizer.setFragmentShader(baseFragShader);
    rasterizer.setModel(getModelMatrix(20));
    rasterizer.setView(getViewMatrix(eyePos));
    rasterizer.setProjection(getProjectionMatrix(45, 1, 0.1, 50));
    rasterizer.clearColor(0.0f, 0.0f, 0.0f, 1.0f);
    rasterizer.clear(Buffers::Color | Buffers::Depth);
    rasterizer.draw(posId, indicesId, colorID, Primitive::Triangle);
    cv::Mat image(800, 800, CV_32FC3, rasterizer.frameBuffer().data());
    cv::imshow("triangles", image);
    cv::waitKey();
}
} // namespace graphics
