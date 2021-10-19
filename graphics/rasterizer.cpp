//
// Created by ytech on 2021/10/18.
//

#include "rasterizer.h"

#include <algorithm>
#include <opencv2/opencv.hpp>
#include <stdexcept>

namespace rst
{
Rasterizer::Rasterizer() = default;

Rasterizer::Rasterizer(int w, int h) :
    m_width(w), m_height(h)
{
    resize(w, h);
}

PosBufferHandle Rasterizer::loadPositions(const std::vector<Vector3f>& position)
{
    auto id = getNextId();
    m_positionBuf.emplace(id, position);
    return { id };
}

IndBufferHandle Rasterizer::loadIndices(const std::vector<Vector3i>& indices)
{
    auto id = getNextId();
    m_indicesBuf.emplace(id, indices);
    return { id };
}

void Rasterizer::setPixel(const Vector3f& point, const Vector3f& color)
{
    if (point.x() < 0.0f || point.x() >= (float)m_width ||
        point.y() < 0.0f || point.y() >= (float)m_height)
    {
        return;
    }
    auto index = getIndex(point.x(), point.y());
    m_frameBuffer[index] = color;
}

void Rasterizer::clearColor(float red, float green, float blue, float alpha)
{
    m_red = red;
    m_green = green;
    m_blue = blue;
    m_alpha = alpha;
}

void Rasterizer::clear(Buffers buff)
{
    if ((buff & Buffers::Color) == Buffers::Color)
    {
        std::fill(m_frameBuffer.begin(), m_frameBuffer.end(), Vector3f(m_blue, m_green, m_red));
    }
    if ((buff & Buffers::Depth) == Buffers::Depth)
    {
        std::fill(m_depthBuffer.begin(), m_depthBuffer.end(), 0.0f);
    }
}

void Rasterizer::draw(PosBufferHandle posBuffer, IndBufferHandle indBuffer, Primitive type)
{
    if (type != Primitive::Triangle)
    {
        throw std::runtime_error("Drawing primitives other than triangle is not implemented yet!");
    }
    auto& positionBuffer = m_positionBuf[posBuffer.posHandle];
    auto& indicesBuffer = m_indicesBuf[indBuffer.indicesHandle];
    Matrix4f mvp = m_projection * m_view * m_model;
    for (auto& i : indicesBuffer)
    {
        Triangle triangle;
        Vector4f v[] = {
            mvp * toVec4(positionBuffer[i[0]], 1.0f),
            mvp * toVec4(positionBuffer[i[1]], 1.0f),
            mvp * toVec4(positionBuffer[i[2]], 1.0f)
        };
        for (auto& vec : v)
        {
            vec /= vec.w();
        }
        for (auto& vert : v)
        {
            vert.x() = 0.5f * (float)m_width * (vert.x() + 1.0f);
            vert.y() = 0.5f * (float)m_height * (vert.y() + 1.0f);
            vert.z() = vert.z();
        }
        for (int j = 0; j < 3; ++j)
        {
            triangle.setVertex(j, v[j].head<3>());
        }
        triangle.setColor(0, 255.0, 0.0, 0.0);
        triangle.setColor(1, 0.0, 255.0, 0.0);
        triangle.setColor(2, 0.0, 0.0, 255.0);
        rasterizeWireframe(triangle);
    }
}

/// 直线扫描算法
void Rasterizer::drawLine(const Vector3f& begin, const Vector3f& end)
{
    auto x1 = begin.x();
    auto y1 = begin.y();
    auto x2 = end.x();
    auto y2 = end.y();
    Vector3f lineColor = { 255, 255, 255 };
    int x, y, xe, ye, i;
    int dx = x2 - x1;
    int dy = y2 - y1;
    int dx1 = fabs(dx);
    int dy1 = fabs(dy);
    int px = 2 * dy1 - dx1;
    int py = 2 * dx1 - dy1;
    if (dy1 <= dx1)
    {
        if (dx >= 0)
        {
            x = x1;
            y = y1;
            xe = x2;
        }
        else
        {
            x = x2;
            y = y2;
            xe = x1;
        }
        Vector3f point = Vector3f(x, y, 1.0f);
        setPixel(point, lineColor);
        for (i = 0; x < xe; i++)
        {
            x = x + 1;
            if (px < 0)
            {
                px = px + 2 * dy1;
            }
            else
            {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
                {
                    y = y + 1;
                }
                else
                {
                    y = y - 1;
                }
                px = px + 2 * (dy1 - dx1);
            }
            Eigen::Vector3f point = Vector3f(x, y, 1.0f);
            setPixel(point, lineColor);
        }
    }
    else
    {
        if (dy >= 0)
        {
            x = x1;
            y = y1;
            ye = y2;
        }
        else
        {
            x = x2;
            y = y2;
            ye = y1;
        }
        Eigen::Vector3f point = Vector3f(x, y, 1.0f);
        setPixel(point, lineColor);
        for (i = 0; y < ye; i++)
        {
            y = y + 1;
            if (py <= 0)
            {
                py = py + 2 * dx1;
            }
            else
            {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
                {
                    x = x + 1;
                }
                else
                {
                    x = x - 1;
                }
                py = py + 2 * (dx1 - dy1);
            }
            Vector3f point = Vector3f(x, y, 1.0f);
            setPixel(point, lineColor);
        }
    }
}

void Rasterizer::rasterizeWireframe(const Triangle& t)
{
    drawLine(t.x(), t.y());
    drawLine(t.x(), t.z());
    drawLine(t.z(), t.y());
}

int Rasterizer::getIndex(int i, int j)
{
    return (m_height - 1 - j) * m_width + i;
}

} // namespace rst
  // namespace rst