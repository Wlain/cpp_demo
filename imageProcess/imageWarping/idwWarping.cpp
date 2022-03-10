//
// Created by cwb on 2022/2/21.
//

#include "idwWarping.h"

#include <cmath>

IdwWarping::~IdwWarping() = default;

Vector2 IdwWarping::targetFunction(const Vector2& input)
{
    float x = 0.0f, y = 0.0f;
    for (int i = 0; i < m_pointSize; ++i)
    {
        auto weight = weightFunction(input, m_pointP[i]);
        x += weight * basicFunction(input, m_pointP[i], m_pointQ[i]).x;
        y += weight * basicFunction(input, m_pointP[i], m_pointQ[i]).y;
    }
    return { x, y };
}

float IdwWarping::weightFunction(const Vector2& point, const Vector2& start)
{
    /// 如果是输出点，权重为1
    if (point == start)
    {
        return 1;
    }
    float sumOfDistance = 0.0f;
    for (const auto& p : m_pointP)
    {
        sumOfDistance += smoothnessFunction(point, p);
    }
    return smoothnessFunction(point, start) / sumOfDistance;
}

Vector2 IdwWarping::basicFunction(const Vector2& point, const Vector2& start, const Vector2& end)
{
    return { end.x + point.x - start.x, end.y + point.y - start.y };
}

float IdwWarping::smoothnessFunction(const Vector2& pointP, const Vector2& pointPi) const
{
    return (float)std::pow(1.0f / pointPi.distance(pointP), m_exponent);
}