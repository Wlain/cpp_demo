//
// Created by william on 2021/10/18.
//

#ifndef CPP_DEMO_RASTERIZER_H
#define CPP_DEMO_RASTERIZER_H
#include "shader.h"
#include "triangle.h"

#include <algorithm>
#include <optional>

using namespace Eigen;

/**
 * 基于CPU的一个简易渲染器
 */

namespace graphics
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
    Triangle,
    Triangle_Line
};

struct PositionBufferHandle
{
    int posHandle = 0;
};

struct IndicesBufferHandle
{
    int indicesHandle = 0;
};

struct ColorBufferHandle
{
    int colorHandle = 0;
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
        m_texture = std::nullopt;
    }
    PositionBufferHandle loadPositions(const std::vector<Vector3f>& position);
    IndicesBufferHandle loadIndices(const std::vector<Vector3i>& indices);
    ColorBufferHandle loadColors(std::vector<Vector4f>& colors);

    inline void setModel(const Matrix4f& m) { m_model = m; }
    inline void setView(const Matrix4f& v) { m_view = v; }
    inline void setProjection(const Matrix4f& p) { m_projection = p; }
    void setPixel(int x, int y, const Vector3f& color);

    inline void setTexture(std::shared_ptr<Texture> tex) { m_texture = std::move(tex); }
    inline void setVertexShader(std::function<Vector3f(VertexShader)> vertShader) { m_vertexShader = std::move(vertShader); }
    inline void setFragmentShader(std::function<Vector3f(FragmentShader)> fragShader) { m_fragmentShader = std::move(fragShader); }

    void clearColor(float red, float green, float blue, float alpha);
    void clear(Buffers buff);
    inline void setMsaaRatio(float ratio) { m_msaaRatio = ratio; }
    void draw(PositionBufferHandle posBuffer, IndicesBufferHandle indBuffer, ColorBufferHandle colBuffer, Primitive type);
    void draw(std::vector<std::shared_ptr<Triangle>>& triangleList);
    std::vector<Vector3f>& frameBuffer() { return m_frameBuffer; }
    void drawLine(const Vector3f& begin, const Vector3f& end);
    void ddaLine(const Vector3f& begin, const Vector3f& end);
    void midLine(const Vector3f& begin, const Vector3f& end);

private:
    void rasterizeWireframe(const Triangle& t);
    void rasterizeTriangle(const Triangle& t);
    void rasterizeTriangle(const std::shared_ptr<Triangle>& t, const Vector3f* viewPos);
    int getIndex(int i, int j) const;
    inline int getNextId() { return m_nextID++; }
    inline Vector4f toVec4(const Vector3f& v3, float w = 1.0f)
    {
        return { v3.x(), v3.y(), v3.z(), w };
    }

    template <typename vec>
    vec interpolate(float alpha, float beta, float gamma, const vec* vert, float weight)
    {
        return (alpha * vert[0] + beta * vert[1] + gamma * vert[2]) / weight;
    }

private:
    Matrix4f m_model;
    Matrix4f m_view;
    Matrix4f m_projection;
    std::map<int, std::vector<Vector3f>> m_positionBuf;
    std::map<int, std::vector<Vector3i>> m_indicesBuf;
    std::map<int, std::vector<Vector4f>> m_colorBuf;
    std::vector<Eigen::Vector3f> m_frameBuffer;
    std::vector<float> m_depthBuffer;
    std::function<Vector3f(FragmentShader)> m_fragmentShader;
    std::function<Vector3f(VertexShader)> m_vertexShader;
    std::optional<std::shared_ptr<Texture>> m_texture;
    float m_msaaRatio = 1.0f;
    float m_red = 0.0f;
    float m_green = 0.0f;
    float m_blue = 0.0f;
    float m_alpha = 0.0f;
    int m_width = 0;
    int m_height = 0;
    int m_nextID = 0;
};
} // namespace graphics

#endif //CPP_DEMO_RASTERIZER_H
