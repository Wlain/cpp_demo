//
// Created by william on 2021/10/18.
//

#ifndef CPP_DEMO_RASTERIZER_H
#define CPP_DEMO_RASTERIZER_H
#include "../utils/timer.h"
#include "buffer.h"
#include "program.h"
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
class Rasterizer
{
public:
    Rasterizer(std::shared_ptr<BufferData> buffer);
    Rasterizer(int w, int h, std::shared_ptr<BufferData> buffer);
    inline void resize(int w, int h)
    {
        m_bufferData->frameBuffer().resize(w * h);
        m_bufferData->depthBuffer().resize(w * h);
    }
    void setPixel(int x, int y, const Vector3f& color);
    void setPixel(int index, const Vector3f& color);
    inline void setProgram(const std::shared_ptr<Program>& program) { m_program = program; }
    inline void setBufferData(const std::shared_ptr<BufferData>& buffer) { m_bufferData = buffer; }
    void clearColor(float red, float green, float blue, float alpha);
    void clear(Buffers buff);
    inline void setMsaaRatio(float ratio)
    {
        m_msaaRatio = ratio;
        m_squareMsaaRatio = std::pow(ratio, 2);
        m_bufferData->depthBuffer().resize(m_bufferData->depthBuffer().size() * m_squareMsaaRatio);
        std::fill(m_bufferData->depthBuffer().begin(), m_bufferData->depthBuffer().end(), std::numeric_limits<float>::infinity());
        m_bufferData->frameBufferForMsaa().resize(m_width * m_height * m_squareMsaaRatio);
    }
    void draw(PositionBufferHandle posBuffer, IndicesBufferHandle indBuffer, ColorBufferHandle colBuffer, Primitive type);
    void draw(std::vector<std::shared_ptr<Triangle>>& triangles);
    void drawLine(const Vector4f& begin, const Vector4f& end);
    void ddaLine(const Vector4f& begin, const Vector4f& end);
    void midLine(const Vector3f& begin, const Vector3f& end);
    inline std::shared_ptr<BufferData>& bufferData() { return m_bufferData; };

private:
    void rasterizeWireframe(const Triangle& t);
    void rasterizeTriangle(const Triangle& triangle);
    void rasterizeTriangle(const std::shared_ptr<Triangle>& triangle, const Vector4f* viewPos);
    bool msaa(float x, float y, const Vector4f* v, const Vector3f& color = {});
    inline int getFrameBufferIndex(int i, int j) const
    {
        ASSERT(i >= 0 && i < m_width && j >= 0 && j < m_height);
        return (m_height - j) * m_width + i;
    }
    inline int getMsaaBufferIndex(int i, int j) const
    {
        ASSERT(i >= 0 && i < m_width * m_msaaRatio && j >= 0 && j < m_height * m_msaaRatio);
        return (m_height * m_msaaRatio - j) * m_width * m_msaaRatio + i;
    }
    inline Vector4f toVec4(const Vector3f& v3, float w = 1.0f)
    {
        return { v3.x(), v3.y(), v3.z(), w };
    }
    template <typename vec>
    vec interpolate(float alpha, float beta, float gamma, const vec* vert, const std::array<Vector4f, 3>& zValue)
    {
        return std::move((alpha * vert[0] / zValue[0].w() + beta * vert[1] / zValue[1].w() + gamma * vert[2] / zValue[2].w()));
    }

private:
    std::shared_ptr<BufferData> m_bufferData;
    std::shared_ptr<Program> m_program;
    Timer m_timer;
    float m_msaaRatio = 1.0f;
    float m_squareMsaaRatio = 1.0f;
    float m_red = 0.0f;
    float m_green = 0.0f;
    float m_blue = 0.0f;
    float m_alpha = 0.0f;
    int m_width = 0;
    int m_height = 0;
};
} // namespace graphics

#endif //CPP_DEMO_RASTERIZER_H
