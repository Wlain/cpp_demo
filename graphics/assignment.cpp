//
// Created by william on 2021/10/18.
//
#include "base.h"
#include "matrix4.h"
#include "objLoader.h"
#include "rasterizer.h"
#include "shaderFunc.h"
#include "utils.h"
#include "program.h"

#include <eigen3/Eigen/Eigen>
#include <opencv2/opencv.hpp>

using namespace Eigen;

namespace graphics
{
/// 实现简单的直线扫描算法，绘制三角形线框
void assignment1()
{
    Vector3f eyePos = { 0.0f, 0.0f, 5.0f };
    std::vector<Vector4f> position = { { 2.0f, 0.0f, -2.0f, 1.0f }, { 0.0f, 2.0f, -2.0f, 1.0f }, { -2.0f, 0.0f, -2.0f, 1.0f } };
    std::vector<Vector4f> colors = { { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } };
    std::vector<Eigen::Vector3i> indices = { { 0, 1, 2 } };
    std::shared_ptr<VertexShader> vertexShader = std::make_shared<VertexShader>();
    std::shared_ptr<FragmentShader> fragmentShader = std::make_shared<FragmentShader>();
    std::shared_ptr<Program> program = std::make_shared<Program>(vertexShader, fragmentShader);
    std::shared_ptr<BufferData> buffer = std::make_shared<BufferData>();
    Rasterizer rasterizer(800, 800, buffer);
    program->setVertexShaderFunc(baseVertShader);
    program->setFragmentShaderFunc(blinnPhongFragmentShader);
    auto& vertShader = *program->vertexShader();
    auto& fragShader = *program->fragmentShader();
    vertShader.uniformMatrix()["modelMatrix"] = getRotation({ 1.0f, 1.0f, 0.0f }, 0.0f);
    vertShader.uniformMatrix()["viewMatrix"] = getViewMatrix(eyePos);
    vertShader.uniformMatrix()["projectionMatrix"] = getProjectionMatrix(45.0f, 1.0f, 0.1f, 50.0f);
    auto posId  = buffer->loadPositions(position);
    auto colorID = buffer->loadColors(colors);
    auto indicesId = buffer->loadIndices(indices);
    rasterizer.setProgram(program);
    rasterizer.clearColor(1.0f, 0.0f, 0.0f, 1.0f);
    rasterizer.clear(Buffers::Color | Buffers::Depth);
    rasterizer.draw(posId, indicesId, colorID, Primitive::Triangle_Line);
    cv::Mat image(800, 800, CV_32FC3, rasterizer.bufferData()->frameBuffer().data());
    cv::flip(image, image, -1);
    cv::imshow("triangle line", image);
    cv::waitKey();
}

/// 实现三角形深度遮挡算法
void assignment2()
{
    Vector3f eyePos = { 0.0f, 0.0f, 18.0f };
    std::vector<Eigen::Vector4f> position = {
        { 2.0f, 0.0f, -2.0f, 1.0f },
        { 0.0f, 2.0f, -2.0f, 1.0f },
        { -2.0f, 0.0f, -2.0f, 1.0f },
        { 3.5f, -1.0f, -5.0f, 1.0f },
        { 2.5f, 1.5f, -5.0f, 1.0f },
        { -1.0f, 0.5f, -5.0f, 1.0f }
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
    std::shared_ptr<VertexShader> vertexShader = std::make_shared<VertexShader>();
    std::shared_ptr<FragmentShader> fragmentShader = std::make_shared<FragmentShader>();
    std::shared_ptr<Program> program = std::make_shared<Program>(vertexShader, fragmentShader);
    std::shared_ptr<BufferData> buffer = std::make_shared<BufferData>();
    Rasterizer rasterizer(800, 800, buffer);
    program->setVertexShaderFunc(baseVertShader);
    program->setFragmentShaderFunc(baseFragTriangleShader);
    auto& vertShader = *program->vertexShader();
    auto& fragShader = *program->fragmentShader();
    vertShader.uniformMatrix()["modelMatrix"] = getModelMatrix(0);
    vertShader.uniformMatrix()["viewMatrix"] = getViewMatrix(eyePos);
    vertShader.uniformMatrix()["projectionMatrix"] = getProjectionMatrix(45.0f, 1.0f, 0.1f, 50.0f);
    auto posId  = buffer->loadPositions(position);
    auto colorID = buffer->loadColors(colors);
    auto indicesId = buffer->loadIndices(indices);
    rasterizer.setProgram(program);
    rasterizer.clearColor(0.0f, 0.0f, 0.0f, 1.0f);
    rasterizer.clear(Buffers::Color | Buffers::Depth);
    rasterizer.setMsaaRatio(2.0f);
    rasterizer.draw(posId, indicesId, colorID, Primitive::Triangle);
    cv::Mat image(800, 800, CV_32FC3, rasterizer.bufferData()->frameBuffer().data());
    // opencv的原点位于左上角，而实际原点应该是位于左下角，需要做一次翻转
    cv::flip(image, image, -1);
    cv::imshow("triangles", image);
    cv::waitKey();
}

void assignment3()
{
    float angle = 140.0f;
    int width = 800, height = 800;
    Vector3f eyePos = { 0.0f, 0.0f, 10.0f };
    std::vector<std::shared_ptr<Triangle>> triangles;
    ObjLoader loader;
    std::string objPath = "../resources/models/spot/";
    auto spotPath = objPath + "spot_triangulated_good.obj";
    ASSERT(isFileExist(spotPath));
    auto ret = loader.loadFile(spotPath);
    ASSERT(ret);
    for (auto& mesh : loader.m_meshes)
    {
        for (int i = 0; i < (int)mesh.vertices.size(); i += 3)
        {
            std::shared_ptr<Triangle> t = std::make_shared<Triangle>();
            for (int j = 0; j < 3; ++j)
            {
                t->setVertex(j, { mesh.vertices[i + j].position.x, mesh.vertices[i + j].position.y, mesh.vertices[i + j].position.z, 1.0f });
                t->setNormal(j, { mesh.vertices[i + j].normal.x, mesh.vertices[i + j].normal.y, mesh.vertices[i + j].normal.z });
                t->setTexCoord(j, { mesh.vertices[i + j].texCoords.x, mesh.vertices[i + j].texCoords.y });
            }
            triangles.emplace_back(t);
        }
    }
    auto texturePath = objPath + "spot_texture.png";
    ASSERT(isFileExist(texturePath));
    std::shared_ptr<VertexShader> vertexShader = std::make_shared<VertexShader>();
    std::shared_ptr<FragmentShader> fragmentShader = std::make_shared<FragmentShader>();
    std::shared_ptr<Program> program = std::make_shared<Program>(vertexShader, fragmentShader);
    program->fragmentShader()->texture() = std::make_shared<Texture>(texturePath.c_str());
    program->setVertexShaderFunc(baseVertShader);
    program->setFragmentShaderFunc(blinnPhongFragmentShader);
    auto& vertShader = *program->vertexShader();
    auto& fragShader = *program->fragmentShader();
    vertShader.uniformMatrix()["modelMatrix"] = getModelMatrix(angle);
    vertShader.uniformMatrix()["viewMatrix"] = getViewMatrix(eyePos);
    vertShader.uniformMatrix()["projectionMatrix"] = getProjectionMatrix(45.0f, 1.0f, 0.1f, 50.0f);
    std::shared_ptr<BufferData> buffer = std::make_shared<BufferData>();
    Rasterizer rasterizer(width, height, buffer);
    rasterizer.setProgram(program);
    rasterizer.clearColor(0.0f, 0.0f, 0.0f, 1.0f);
    rasterizer.clear(Buffers::Color | Buffers::Depth);
    rasterizer.setMsaaRatio(4.0f);
    rasterizer.draw(triangles);
    cv::Mat image(width, height, CV_32FC3, rasterizer.bufferData()->frameBuffer().data());
    image.convertTo(image, CV_8UC3, 1.0f);
    cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
    cv::flip(image, image, -1);
    cv::imshow("mesh", image);
    cv::waitKey();
}
} // namespace graphics
