//
// Created by ytech on 2021/10/25.
//

#ifndef CPP_DEMO_SHADERFUNC_H
#define CPP_DEMO_SHADERFUNC_H
#include "shader.h"

#include <eigen3/Eigen/Eigen>

namespace graphics
{
struct Light
{
    Eigen::Vector3f position;
    Eigen::Vector3f intensity;
};

/// 基础顶点着色器
Eigen::Vector4f baseVertShader(const VertexShader& vertShader);

/// 基础片元着色器
Eigen::Vector3f baseFragShader(const FragmentShader& fragShader);

/// 绘制法线
Eigen::Vector3f normalFragShader(const FragmentShader& fragShader);

/// 绘制纹理
Eigen::Vector3f textureFragmentShader(const FragmentShader& fragShader);

/// ground shadering
Eigen::Vector4f groundVertexShader(const VertexShader& vertShader);

/// blinn-phong shadering
Eigen::Vector3f blinnPhongFragmentShader(const FragmentShader& fragShader);

/// https://zhuanlan.zhihu.com/p/137449232
/// bump mapping shadering(凹凸贴图)
Eigen::Vector3f bumpFragmentShader(const FragmentShader& fragShader);

/// displacement mapping shadering(位移贴图)
Eigen::Vector3f displacementFragmentShader(const FragmentShader& fragShader);

/// Normal mapping shadering(法线贴图)
Eigen::Vector3f normalFragmentShader(const FragmentShader& fragShader);

} // namespace graphics
#endif //CPP_DEMO_SHADERFUNC_H
