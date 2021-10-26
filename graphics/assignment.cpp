//
// Created by william on 2021/10/18.
//
#include "base.h"
#include "matrix4.h"
#include "objLoader.h"
#include "rasterizer.h"
#include "utils.h"
#include "shaderFunc.h"

#include <eigen3/Eigen/Eigen>
#include <opencv2/opencv.hpp>

using namespace Eigen;

namespace graphics
{
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
    rasterizer.setModel(getRotation({ 1.0f, 1.0f, 0.0f }, 0.0f));
    rasterizer.setView(getViewMatrix(eyePos));
    rasterizer.setProjection(getProjectionMatrix(45.0f, 1.0f, 0.10f, 50.0f));
    rasterizer.clearColor(1.0f, 0.0f, 0.0f, 1.0f);
    rasterizer.clear(Buffers::Color | Buffers::Depth);
    rasterizer.draw(posId, indicesId, colorID, Primitive::Triangle_Line);
    cv::Mat image(800, 800, CV_32FC3, rasterizer.frameBuffer().data());
    cv::flip(image, image, -1);
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
        { 2.5f, 1.5f, -5.0f },
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
    rasterizer.setModel(getModelMatrix(20.0f));
    rasterizer.setView(getViewMatrix(eyePos));
    rasterizer.setProjection(getProjectionMatrix(45.0f, 1.0f, 0.1f, 50.0f));
    rasterizer.clearColor(0.0f, 0.0f, 0.0f, 1.0f);
    rasterizer.clear(Buffers::Color | Buffers::Depth);
    rasterizer.setMsaaRatio(4.0f);
    rasterizer.draw(posId, indicesId, colorID, Primitive::Triangle);
    cv::Mat image(800, 800, CV_32FC3, rasterizer.frameBuffer().data());
    // opencv的原点位于左上角，而实际原点应该是位于左下角，需要做一次翻转
    cv::flip(image, image, -1);
    cv::imshow("triangles", image);
    cv::waitKey();
}

void assignment3()
{
    Rasterizer rasterizer(800, 800);
    float angle = 140.0f;
    Vector3f eyePos = { 0.0f, 0.0f, 5.0f };
    std::vector<std::shared_ptr<Triangle>> triangles;
    ObjLoader loader;
    std::string objPath = "../resources/models/spot/";
    auto spotPath = objPath + "spot_triangulated_good.obj";
    ASSERT(isFileExist(spotPath));
    auto ret = loader.loadFile(spotPath);
    ASSERT(ret);
    for (auto& mesh : loader.m_meshes)
    {
        for (int i = 0; i < (int)mesh.vertices.size(); ++i)
        {
            std::shared_ptr<Triangle> t = std::make_shared<Triangle>();
            for (int j = 0; j < 3; ++j)
            {
                t->setVertex(j, { mesh.vertices[i + j].position.x, mesh.vertices[i + j].position.y, mesh.vertices[i + j].position.z });
                t->setNormal(j, { mesh.vertices[i + j].normal.x, mesh.vertices[i + j].normal.y, mesh.vertices[i + j].normal.z });
                t->setTexCoord(j, { mesh.vertices[i + j].texCoords.x, mesh.vertices[i + j].texCoords.y });
            }
            triangles.emplace_back(t);
        }
    }
    auto texturePath = objPath + "hmap.jpg";
    ASSERT(isFileExist(texturePath));
    rasterizer.setVertexShader(baseVertexShader);
    rasterizer.setFragmentShader(baseFragShader);
    rasterizer.setTexture(Texture(texturePath.c_str()));
    rasterizer.setModel(getModelMatrix(angle));
    rasterizer.setView(getViewMatrix(eyePos));
    rasterizer.setProjection(getProjectionMatrix(45.0f, 1.0f, 0.1f, 50.0f));
    rasterizer.clearColor(1.0f, 0.0f, 0.0f, 1.0f);
    rasterizer.clear(Buffers::Color | Buffers::Depth);
    rasterizer.draw(triangles);
    cv::Mat image(800, 800, CV_32FC3, rasterizer.frameBuffer().data());
    image.convertTo(image, CV_8UC3, 1.0f);
    cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
    cv::flip(image, image, -1);
    cv::imshow("triangles", image);
    cv::waitKey();
}
} // namespace graphics
