//
// Created by william on 2021/11/24.
//

#ifndef CPP_DEMO_GROUND_H
#define CPP_DEMO_GROUND_H
#include "object.h"
namespace graphics::pathTracing
{
class Ground : public Object
{
public:
    Ground(const std::vector<Vector3>& vertexs, const std::vector<uint32_t>& vertexsIndex, uint32_t numTriangles, const std::vector<Vector2>& st);
    ~Ground() override;
    bool intersect(const Vector3& originPoint, const Vector3& dir, float& timeNear, uint32_t& index, Vector2& uv) override;
    void getSurfaceProperties(const Vector3& hitPoint, const Vector3& dir, const uint32_t& index, const Vector2& uv, Vector3& normal, Vector2& st) const override;

private:
    std::unique_ptr<std::vector<Vector3>> m_vertices;
    std::unique_ptr<std::vector<uint32_t>> m_vertexIndex;
    std::unique_ptr<std::vector<Vector2>> m_stCoordinates;
    uint32_t m_numTriangles;
};
} // namespace graphics

#endif //CPP_DEMO_GROUND_H
