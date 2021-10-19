//
// Created by ytech on 2021/10/18.
//

#ifndef CPP_DEMO_RASTERIZER_H
#define CPP_DEMO_RASTERIZER_H
#include "triangle.h"

#include <algorithm>

using namespace Eigen;

/**
 * 基于CPU的一个简易渲染器
 */

namespace rst
{
enum class Buffers : uint32_t
{
    Color = 1,
    Depth = 2,
};

inline Buffers operator|(Buffers a, Buffers b)
{
    return Buffers((uint32_t)a | (uint32_t)b);
}

inline Buffers operator&(Buffers a, Buffers b)
{
    return Buffers((uint32_t)a & (uint32_t)b);
}

enum class Primitive : uint32_t
{
    Line,
    Triangle
};

struct PosBufferHandle
{
    int posHandle = 0;
};

struct IndBufferHandle
{
    int indicesHandle = 0;
};

class Rasterizer
{
public:
    Rasterizer();
    Rasterizer(int w, int h);
    inline void resize(int w, int h)
    {
        m_frameBuffer.resize(w * h);
        m_depthBuffer.resize(w * h);
    }
    PosBufferHandle loadPositions(const std::vector<Vector3f>& position);
    IndBufferHandle loadIndices(const std::vector<Vector3i>& indices);

    void setModel(const Matrix4f& m)
    {
        m_model = m;
    }
    void setView(const Matrix4f& v)
    {
        m_view = v;
    }

    void setProjection(const Matrix4f& p)
    {
        m_projection = p;
    }

    void setPixel(const Vector3f& point, const Vector3f& color);
    void clearColor(float red, float green, float blue, float alpha);
    void clear(Buffers buff);
    void draw(PosBufferHandle posBuffer, IndBufferHandle indBuffer, Primitive type);
    std::vector<Vector3f>& frameBuffer() { return m_frameBuffer; }

private:
    void drawLine(const Vector3f& begin, const Vector3f& end);
    void rasterizeWireframe(const Triangle& t);
    int getIndex(int i, int j);
    inline int getNextId() { return m_nextID++; }
    inline Vector4f toVec4(const Vector3f& v3, float w = 1.0f)
    {
        return { v3.x(), v3.y(), v3.z(), w };
    }

private:
    Matrix4f m_model;
    Matrix4f m_view;
    Matrix4f m_projection;
    std::map<int, std::vector<Vector3f>> m_positionBuf;
    std::map<int, std::vector<Vector3i>> m_indicesBuf;
    std::vector<Eigen::Vector3f> m_frameBuffer;
    std::vector<float> m_depthBuffer;
    float m_red = 0.0f;
    float m_green = 0.0f;
    float m_blue = 0.0f;
    float m_alpha = 0.0f;
    int m_width = 0;
    int m_height = 0;
    int m_nextID = 0;
};
} // namespace rst

#endif //CPP_DEMO_RASTERIZER_H
