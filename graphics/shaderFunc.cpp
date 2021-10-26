//
// Created by ytech on 2021/10/25.
//

#include "shaderFunc.h"
using namespace Eigen;

namespace graphics
{
Vector3f baseVertShader(const VertexShader& vertShader)
{
    return vertShader.position();
}

Vector3f baseFragShader(const FragmentShader& fragShader)
{
    return { 1.0, 1.0, 1.0f };
}
} // namespace graphics