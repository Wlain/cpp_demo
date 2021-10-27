//
// Created by ytech on 2021/10/25.
//

#ifndef CPP_DEMO_SHADERFUNC_H
#define CPP_DEMO_SHADERFUNC_H
#include "shader.h"

#include <eigen3/Eigen/Eigen>

namespace graphics
{
// 基础顶点着色器
Eigen::Vector4f baseVertShader(const VertexShader& vertShader);

// 基础片元着色器
Eigen::Vector3f baseFragShader(const FragmentShader& fragShader);

// 绘制法线
Eigen::Vector3f normalFragShader(const FragmentShader& fragShader);

} // namespace graphics
#endif //CPP_DEMO_SHADERFUNC_H
