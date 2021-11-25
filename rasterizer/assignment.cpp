//
// Created by william on 2021/10/18.
//
#include "assignment.h"

#include "base.h"
#include "bezierCurve.h"
#include "light.h"
#include "matrix4.h"
#include "meshTriangle.h"
#include "objLoader.h"
#include "program.h"
#include "rasterizer.h"
#include "renderer.h"
#include "scene.h"
#include "shaderFunc.h"
#include "sphere.h"
#include "utils.h"

#include <eigen3/Eigen/Eigen>
#include <opencv2/opencv.hpp>

using namespace Eigen;

namespace graphics
{
/// 实现简单的直线扫描算法，绘制三角形线框
int width = 360, height = 360;
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
    Rasterizer rasterizer(width, height, buffer);
    program->setVertexShaderFunc(baseVertShader);
    program->setFragmentShaderFunc(blinnPhongFragmentShader);
    auto& vertShader = *program->vertexShader();
    auto& fragShader = *program->fragmentShader();
    vertShader.uniformMatrix()["modelMatrix"] = getRotation({ 1.0f, 1.0f, 0.0f }, 0.0f);
    vertShader.uniformMatrix()["viewMatrix"] = getViewMatrix(eyePos);
    vertShader.uniformMatrix()["projectionMatrix"] = getProjectionMatrix(45.0f, 1.0f, 0.1f, 50.0f);
    auto posId = buffer->loadPositions(position);
    auto colorID = buffer->loadColors(colors);
    auto indicesId = buffer->loadIndices(indices);
    rasterizer.setProgram(program);
    rasterizer.clearColor(1.0f, 0.0f, 0.0f, 1.0f);
    rasterizer.clear(Buffers::Color | Buffers::Depth);
    rasterizer.draw(posId, indicesId, colorID, Primitive::Triangle_Line);
    cv::Mat image(width, height, CV_32FC3, rasterizer.bufferData()->frameBuffer().data());
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
    Rasterizer rasterizer(width, height, buffer);
    program->setVertexShaderFunc(baseVertShader);
    program->setFragmentShaderFunc(baseFragTriangleShader);
    auto& vertShader = *program->vertexShader();
    auto& fragShader = *program->fragmentShader();
    vertShader.uniformMatrix()["modelMatrix"] = getModelMatrix(0);
    vertShader.uniformMatrix()["viewMatrix"] = getViewMatrix(eyePos);
    vertShader.uniformMatrix()["projectionMatrix"] = getProjectionMatrix(45.0f, 1.0f, 0.1f, 50.0f);
    auto posId = buffer->loadPositions(position);
    auto colorID = buffer->loadColors(colors);
    auto indicesId = buffer->loadIndices(indices);
    rasterizer.setProgram(program);
    rasterizer.clearColor(0.0f, 0.0f, 0.0f, 1.0f);
    rasterizer.clear(Buffers::Color | Buffers::Depth);
    rasterizer.setMsaaRatio(2.0f);
    rasterizer.draw(posId, indicesId, colorID, Primitive::Triangle);
    cv::Mat image(width, height, CV_32FC3, rasterizer.bufferData()->frameBuffer().data());
    // opencv的原点位于左上角，而实际原点应该是位于左下角，需要做一次翻转
    cv::flip(image, image, -1);
    cv::imshow("triangles", image);
    cv::waitKey();
}

void assignment3(ShardingType type)
{
    float angle = 140.0f;
    Vector3f eyePos = { 0.0f, 0.0f, 10.0f };
    std::vector<std::shared_ptr<Triangle>> triangles;
    std::string objPath = "../resources/models/spot/";
    auto texturePath = objPath + "spot_texture.png";
    std::function<Vector3f(const FragmentShader&)> fragShaderFun;
    switch (type)
    {
    case ShardingType::textureSamplerShading:
        fragShaderFun = textureFragmentShader;
        break;
    case ShardingType::normalTest:
        fragShaderFun = normalFragShader;
        break;
    case ShardingType::blinnPhongModelShading:
        fragShaderFun = blinnPhongFragmentShader;
        break;
    case ShardingType::bumpShading:
        fragShaderFun = bumpFragmentShader;
        texturePath = objPath + "hmap.jpg";
        break;
    case ShardingType::displacementShading:
        fragShaderFun = displacementFragmentShader;
        texturePath = objPath + "hmap.jpg";
        break;
    }
    ObjLoader loader;
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
    ASSERT(isFileExist(texturePath));
    std::shared_ptr<VertexShader> vertexShader = std::make_shared<VertexShader>();
    std::shared_ptr<FragmentShader> fragmentShader = std::make_shared<FragmentShader>();
    std::shared_ptr<Program> program = std::make_shared<Program>(vertexShader, fragmentShader);
    program->fragmentShader()->texture() = std::make_shared<Texture>(texturePath.c_str());
    program->setVertexShaderFunc(baseVertShader);
    program->setFragmentShaderFunc(fragShaderFun);
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
    //    rasterizer.setMsaaRatio(4.0f);
    rasterizer.draw(triangles);
    cv::Mat image(width, height, CV_32FC3, rasterizer.bufferData()->frameBuffer().data());
    image.convertTo(image, CV_8UC3, 1.0f);
    cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
    cv::flip(image, image, -1);
    cv::imshow("mesh", image);
    cv::waitKey();
}

void assignment4()
{
    BezierCurve bezierCurve(700, 700);
    auto& controlPoints = bezierCurve.controlPoints();
    auto& window = *bezierCurve.window();
    int key = -1;
    while (key != 27)
    {
        for (auto& point : controlPoints)
        {
            //在img原始图片中划圈, 圈的中心点为point，半径=3，颜色为（255，0，0），粗细=3
            cv::circle(window, point, 3, { { 0, 0, 255 } }, 3);
        }
        if (controlPoints.size() == 4)
        {
            bezierCurve.nativeBezier();
            bezierCurve.fillBezier();
            cv::imshow("Bezier Curve", window);
            cv::imwrite("my_bezier_curve.png", window);
            key = cv::waitKey(0);
        }
        cv::imshow("Bezier Curve", window);
        cv::waitKey(1);
    }
}

void assignment5()
{
    rayTracing::Scene scene(1280, 960);
    auto sphere1 = std::make_unique<rayTracing::Sphere>(Vector3(-1.0f, 0.0f, -12.0f), 2.0f);
    sphere1->m_materialType = rayTracing::Object::MaterialType::DIFFUSE_AND_GLOSSY;
    sphere1->m_diffuseColor = { 0.6f, 0.7, 0.8 };

    auto sphere2 = std::make_unique<rayTracing::Sphere>(Vector3(0.5f, 0.0f, -12.0f), 2.0f);
    sphere2->m_materialType = rayTracing::Object::MaterialType::REFLECTION_AND_REFRACTION;
    sphere2->m_ior = 1.5f;
    scene.add(std::move(sphere1));
    scene.add(std::move(sphere2));
    std::vector<Vector3> verts = { { -5.0f, -3.0f, -6.0f }, { 5.0f, -3.0f, -6.0f }, { 5.0f, -3.0f, -16.0f }, { -5.0f, -3.0f, -16.0f } };
    std::vector<uint32_t> vertIndex = { 0, 1, 3, 1, 2, 3 };
    std::vector<Vector2> st = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
    auto mesh = std::make_unique<rayTracing::MeshTriangle>(verts, vertIndex, 2, st);
    mesh->m_materialType = rayTracing::Object::MaterialType::DIFFUSE_AND_GLOSSY;
    scene.add(std::move(mesh));
    scene.add(std::make_unique<rayTracing::Light>(Vector3(-20, 70, 20), 0.5));
    scene.add(std::make_unique<rayTracing::Light>(Vector3(30, 50, -12), 0.5));
    rayTracing::Renderer renderer;
    renderer.Render(scene);
    cv::Mat image(width, height, CV_32FC3);
    cv::flip(image, image, -1);
    cv::imshow("path Tracing", image);
    cv::waitKey();
}
} // namespace graphics
