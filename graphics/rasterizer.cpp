//
// Created by william on 2021/10/18.
//

#include "rasterizer.h"

#include "vector3.h"

#include <algorithm>
#include <stdexcept>

namespace graphics
{
Rasterizer::Rasterizer() = default;

Rasterizer::Rasterizer(int w, int h) :
    m_width(w), m_height(h)
{
    resize(w, h);
}

PositionBufferHandle Rasterizer::loadPositions(const std::vector<Vector4f>& position)
{
    auto id = getNextId();
    m_positionBuf.emplace(id, position);
    return { id };
}

IndicesBufferHandle Rasterizer::loadIndices(const std::vector<Vector3i>& indices)
{
    auto id = getNextId();
    m_indicesBuf.emplace(id, indices);
    return { id };
}

ColorBufferHandle Rasterizer::loadColors(std::vector<Vector4f>& colors)
{
    auto id = getNextId();
    m_colorBuf.emplace(id, colors);
    return { id };
}

void Rasterizer::setPixel(int x, int y, const Vector3f& color)
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
    {
        return;
    }
    auto index = getIndex(x, y);
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
        std::fill(m_depthBuffer.begin(), m_depthBuffer.end(), std::numeric_limits<float>::infinity());
    }
}

void Rasterizer::draw(PositionBufferHandle posBuffer, IndicesBufferHandle indBuffer, ColorBufferHandle colBuffer, Primitive type)
{
    if (type == Primitive::Line)
    {
        throw std::runtime_error("Drawing primitives Line is not implemented yet!");
    }
    auto& positionBuffer = m_positionBuf[posBuffer.posHandle];
    auto& indicesBuffer = m_indicesBuf[indBuffer.indicesHandle];
    auto& colorBuffer = m_colorBuf[colBuffer.colorHandle];
    /// 原始：局部坐标系
    /// 模型变换 -> 世界坐标系
    /// 视图变换 -> 观察者坐标系
    /// 投影变换 -> 裁剪坐标系
    auto mvp = m_projection * m_view * m_model;
    for (auto& i : indicesBuffer)
    {
        Triangle triangle;
        Vector4f position[] = {
            mvp * positionBuffer[i[0]],
            mvp * positionBuffer[i[1]],
            mvp * positionBuffer[i[2]]
        };
        /// 透视除法 -> 规范化坐标系(NDC)
        for (auto& vec : position)
        {
            vec /= vec.w();
        }
        /// 视口变换 -> 屏幕坐标系
        for (auto& vert : position)
        {
            vert.x() = 0.5f * (float)m_width * (vert.x() + 1.0f);
            vert.y() = 0.5f * (float)m_height * (vert.y() + 1.0f);
            vert.z() = vert.z();
        }
        for (int j = 0; j < 3; ++j)
        {
            triangle.setVertex(j, position[j]);
        }
        auto colA = colorBuffer[i[0]];
        auto colB = colorBuffer[i[1]];
        auto colC = colorBuffer[i[2]];
        triangle.setColor(0, colA[0], colA[1], colA[2]);
        triangle.setColor(1, colB[0], colB[1], colB[2]);
        triangle.setColor(2, colC[0], colC[1], colC[2]);
        if (type == Primitive::Triangle_Line)
        {
            rasterizeWireframe(triangle);
        }
        else
        {
            rasterizeTriangle(triangle);
        }
    }
}

void Rasterizer::draw(std::vector<std::shared_ptr<Triangle>>& triangles)
{
    float f1 = (50 - 0.1) / 2.0;
    float f2 = (50 + 0.1) / 2.0;

    auto mvp = m_projection * m_view * m_model;
    for (auto& triangle : triangles)
    {
        Vector4f viewSpacePosition[] = {
            m_view * m_model * triangle->vertex()[0],
            m_view * m_model * triangle->vertex()[1],
            m_view * m_model * triangle->vertex()[2]
        };
        /// 原始：局部坐标系
        /// 模型变换 -> 世界坐标系
        /// 视图变换 -> 观察者坐标系
        /// 投影变换 -> 裁剪坐标系
        Vector4f position[] = {
            mvp * triangle->vertex()[0],
            mvp * triangle->vertex()[1],
            mvp * triangle->vertex()[2]
        };
        /// 透视除法 -> 规范化坐标系(NDC)
        for (auto& vec : position)
        {
            vec /= vec.w();
        }
        /// 逆变换
        Matrix4f inverseTrans = (m_view * m_model).inverse().transpose();
        Eigen::Vector4f normal[] = {
            inverseTrans * toVec4(triangle->normal()[0], 0.0f),
            inverseTrans * toVec4(triangle->normal()[1], 0.0f),
            inverseTrans * toVec4(triangle->normal()[2], 0.0f)
        };
        /// 视口变换 -> 屏幕坐标系
        for (auto& vert : position)
        {
            vert.x() = 0.5f * (float)m_width * (vert.x() + 1.0f);
            vert.y() = 0.5f * (float)m_height * (vert.y() + 1.0f);
            vert.z() = vert.z() * f1 + f2; // 猜测是做扰动？防止精度问题
        }
        for (int i = 0; i < 3; ++i)
        {
            // screen space coordinates
            triangle->setVertex(i, position[i]);
        }

        for (int i = 0; i < 3; ++i)
        {
            //view space normal
            triangle->setNormal(i, normal[i].head<3>());
        }
        // Also pass view space vertice position
        rasterizeTriangle(triangle, viewSpacePosition);
    }
}

/// 直线扫描算法
/// 1.DDA画线算法（数值微分法）：引进图形学中一个很重要的思想—增量思想。
/// 点xi，yi满足直线方程yi=kxi+b，
/// 若xi增加一个单位，则下一步点的位置（xi + 1，yi+1）满足yi+1=k（xi + 1）+ b。
/// 即yi+1=yi+k。
void Rasterizer::ddaLine(const Vector4f& begin, const Vector4f& end)
{
    auto x1 = begin.x();
    auto y1 = begin.y();

    auto x2 = end.x();
    auto y2 = end.y();
    Vector3f lineColor = { 255, 255, 255 };
    int dx = (int)x2 - (int)x1;
    int dy = (int)y2 - (int)y1;
    float k = (float)dy / (float)dx;
    float _k = (float)dx / (float)dy;
    int absDx = fabs(dx);
    int absDy = fabs(dy);
    if (absDx >= absDy) //  // 斜率 k <= 1 || 斜率 k >= -1
    {
        // 如果第二个点在第一个点的左边，需要做一个交换
        if (dx < 0)
        {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
        while (x1 < x2)
        {
            setPixel(x1, int(y1 + 0.5f), lineColor);
            y1 = y1 + k;
            x1 = x1 + 1;
        }
    }
    else // 斜率k > 1 || 斜率 k < -1
    {
        if (dy < 0)
        {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
        while (y1 < y2)
        {
            setPixel(int(x1 + 0.5f), y1, lineColor);
            x1 = x1 + _k;
            y1 = y1 + 1;
        }
    }
}

/// 2.中点画线算法
/// 直线的一般式方程：Ax + By + C = 0;其中，A = -DeltaY, B = DeltaX, C = -B * DeltaX
void Rasterizer::midLine(const Vector3f& begin, const Vector3f& end)
{
    /// TODO
}

void Rasterizer::drawLine(const Vector4f& begin, const Vector4f& end)
{
    auto x1 = begin.x();
    auto y1 = begin.y();

    auto x2 = end.x();
    auto y2 = end.y();

    Vector3f lineColor = { 255, 255, 255 };
    int x, y, xe, ye;
    int dx = (int)x2 - (int)x1;
    int dy = (int)y2 - (int)y1;

    int dx1 = fabs(dx);
    int dy1 = fabs(dy);
    int px = 2 * dy1 - dx1;
    int py = 2 * dx1 - dy1;
    // 斜率 k <= 1 || 斜率 k >= -1
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
        setPixel(x, y, lineColor);
        for (int i = 0; x < xe; i++)
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
            setPixel(x, y, lineColor);
        }
    }
    else // 斜率k > 1 || 斜率 k < -1
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
        setPixel(x, y, lineColor);
        for (int i = 0; y < ye; i++)
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
            setPixel(x, y, lineColor);
        }
    }
}

void Rasterizer::rasterizeWireframe(const Triangle& t)
{
    ddaLine(t.x(), t.y());
    ddaLine(t.x(), t.z());
    ddaLine(t.z(), t.y());
}

///  面积法：计算三角形内一个点的重心坐标
static std::tuple<float, float, float> computeBarycentric2D(float x, float y, const Vector4f* v)
{
    float alpha = (x * (v[1].y() - v[2].y()) + (v[2].x() - v[1].x()) * y + v[1].x() * v[2].y() - v[2].x() * v[1].y()) / (v[0].x() * (v[1].y() - v[2].y()) + (v[2].x() - v[1].x()) * v[0].y() + v[1].x() * v[2].y() - v[2].x() * v[1].y());
    float beta = (x * (v[2].y() - v[0].y()) + (v[0].x() - v[2].x()) * y + v[2].x() * v[0].y() - v[0].x() * v[2].y()) / (v[1].x() * (v[2].y() - v[0].y()) + (v[0].x() - v[2].x()) * v[1].y() + v[2].x() * v[0].y() - v[0].x() * v[2].y());
    float gamma = 1.0f - alpha - beta;
    return { alpha, beta, gamma };
}

/// 对每个像素进行ratio x ratio采样，msaa 是对ssaa的一个改进（super sampling antiAliasing）
static float msaa(float ratio, float x, float y, const Vector4f* _v)
{
    float percentage = 0;
    float samplerTimes = 1.0f / (ratio * ratio);
    float dUnit = 1.0f / ratio; // 每一次的步进距离
    x += dUnit / 2.0f;
    y += dUnit / 2.0f;
    // x, y设置为中心点
    for (int i = 0; i < (int)ratio; ++i)
    {
        for (int j = 0; j < (int)ratio; ++j)
        {
            if (inTriangle({ x + (float)i * dUnit, y + (float)j * dUnit, 1.0f }, { _v[0].x(), _v[0].y(), _v[0].z() }, { _v[1].x(), _v[1].y(), _v[1].z() }, { _v[2].x(), _v[2].y(), _v[2].z() }))
            {
                percentage += samplerTimes;
            }
        }
    }
    return percentage;
}

void Rasterizer::rasterizeTriangle(const Triangle& triangle)
{
    auto v = triangle.toVector4();
    const auto vertexX = { v[0].x(), v[1].x(), v[2].x() };
    const auto vertexY = { v[0].y(), v[1].y(), v[2].y() };
    auto [minX, maxX] = std::minmax_element(std::begin(vertexX), std::end(vertexX));
    auto [minY, maxY] = std::minmax_element(std::begin(vertexY), std::end(vertexY));
    /// 包围盒逻辑
    for (int x = std::floor(*minX); x < std::ceil(*maxX); ++x)
    {
        for (int y = std::floor(*minY); y < std::ceil(*maxY); ++y)
        {
            float percentage = msaa(m_msaaRatio, (float)x, (float)y, triangle.vertex());
            if (percentage == 0) continue;
            {
                auto [alpha, beta, gamma] = computeBarycentric2D((float)x, (float)y, triangle.vertex());
                /// 透视插值矫正
                float reciprocalCorrect = 1.0f / (alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
                float zInterpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();

                zInterpolated *= reciprocalCorrect;
                // z-Buffer算法
                if (zInterpolated >= m_depthBuffer[y * m_width + x])
                {
                    continue;
                }
                if (percentage == 1.0f)
                {
                    m_depthBuffer[y * m_width + x] = zInterpolated;
                }
                setPixel(x, y, triangle.color()[0] * percentage);
            }
        }
    }
}

void Rasterizer::rasterizeTriangle(const std::shared_ptr<Triangle>& triangle, const Vector4f* viewPos)
{
    auto v = triangle->toVector4();
    const auto vertexX = { v[0].x(), v[1].x(), v[2].x() };
    const auto vertexY = { v[0].y(), v[1].y(), v[2].y() };
    auto [minX, maxX] = std::minmax_element(std::begin(vertexX), std::end(vertexX));
    auto [minY, maxY] = std::minmax_element(std::begin(vertexY), std::end(vertexY));
    /// 包围盒逻辑
    for (int x = std::floor(*minX); x < std::ceil(*maxX); ++x)
    {
        for (int y = std::floor(*minY); y < std::ceil(*maxY); ++y)
        {
            float percentage = msaa(m_msaaRatio, (float)x, (float)y, triangle->vertex());
            if (percentage == 0) continue;
            auto [alpha, beta, gamma] = computeBarycentric2D((float)x, (float)y, triangle->vertex());
            float reciprocalCorrect = 1.0f / (alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
            float interpolatedZValue = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
            interpolatedZValue *= reciprocalCorrect;
            /// z-Buffer算法
            if (interpolatedZValue >= m_depthBuffer[y * m_width + x])
            {
                continue;
            }
            m_depthBuffer[y * m_width + x] = interpolatedZValue;
            const auto interpolatedColor = interpolate(alpha, beta, gamma, triangle->color(), 1.0f);
            auto interpolatedNormal = interpolate(alpha, beta, gamma, triangle->normal(), 1.0f);
            const auto interpolatedTexCoords = interpolate(alpha, beta, gamma, triangle->texCoords(), 1.0f);
            auto interpolatedViewPosition = interpolate(alpha, beta, gamma, viewPos, 1.0f);
            FragmentShader fragShader(interpolatedColor, interpolatedNormal.normalized(), interpolatedTexCoords, m_texture.value_or(nullptr));
            fragShader.viewPosition() = interpolatedViewPosition;
            VertexShader vertexShader;
            vertexShader.setPosition({ x, y, interpolatedZValue, 1.0f });
            auto pixelColor = m_fragmentShader(fragShader);
            auto vPosition = m_vertexShader(vertexShader);

            setPixel(vPosition.x(), vPosition.y(), pixelColor);
        }
    }
}

int Rasterizer::getIndex(int i, int j) const
{
    return (m_height - 1 - j) * m_width + i;
}
} // namespace graphics
