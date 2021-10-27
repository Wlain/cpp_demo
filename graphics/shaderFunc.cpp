//
// Created by ytech on 2021/10/25.
//

#include "shaderFunc.h"
using namespace Eigen;

namespace graphics
{
Vector4f baseVertShader(const VertexShader& vertShader)
{
    return vertShader.position();
}

Vector3f baseFragShader(const FragmentShader& fragShader)
{
    return { 148.0f ,121.0f, 92.0f };
}

Vector3f normalFragShader(const FragmentShader& fragShader)
{
    Vector3f returnColor = (fragShader.normal().normalized() + Vector3f(1.0f, 1.0f, 1.0f)) / 2.f;
    return {returnColor.x() * 255, returnColor.y() * 255, returnColor.z() * 255};
}
} // namespace graphics