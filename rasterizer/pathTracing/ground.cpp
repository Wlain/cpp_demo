//
// Created by william on 2021/11/24.
//

#include "ground.h"
namespace graphics::pathTracing
{
Ground::Ground(const std::vector<Vector3>& vertexs, const std::vector<uint32_t>& vertexsIndex, uint32_t numTriangles, const std::vector<Vector2>& st)
{
}

Ground::~Ground() = default;

bool Ground::intersect(const Vector3& originPoint, const Vector3& dir, float& timeNear, uint32_t& index, Vector2& uv)
{
    return false;
}
void Ground::getSurfaceProperties(const Vector3& hitPoint, const Vector3& dir, const uint32_t& index, const Vector2& uv, Vector3& normal, Vector2& st) const
{
}
}