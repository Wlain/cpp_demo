//
// Created by cwb on 2022/2/21.
//

#include "imageWarping/rbfWarping.h"

#include <iostream>
#include <utility>

RbfWarping::RbfWarping(std::vector<Vector2> p, std::vector<Vector2> q) :
    BaseWarping(std::move(p), std::move(q))
{
    m_exponent = -1;
    calcRadius();
    calcAi();
}

RbfWarping::~RbfWarping() = default;

Vector2 RbfWarping::targetFunction(const Vector2& input)
{
    float x = 0.0f, y = 0.0f;
    if (m_pointSize <= 1)
    {
        x = m_pointQ[0].x - m_pointP[0].x;
        y = m_pointQ[0].y - m_pointP[0].y;
    }
    for (int i = 0; i < m_pointSize; ++i)
    {
        auto gi = std::pow(m_pointP[i].distance(input) + m_radius[i], m_exponent / 2);
        x += m_Ai.x(i) * gi;
        y += m_Ai.y(i) * gi;
    }
    return { x + input.x, y + input.y };
}

void RbfWarping::calcRadius()
{
    m_radius.reserve(m_pointSize);
    for (int i = 0; i < m_pointSize; ++i)
    {
        float ri = INT_MAX;
        for (int j = 0; j < m_pointSize; ++j)
        {
            if (i == j) break;
            auto distance = m_pointP[i].distance(m_pointP[j]);
            if (distance < ri)
            {
                ri = distance;
            }
        }
        m_radius.emplace_back(ri);
    }
}

void RbfWarping::calcAi()
{
    Eigen::MatrixXd A(m_pointSize, m_pointSize);
    Eigen::VectorXd Bx(m_pointSize);
    Eigen::VectorXd By(m_pointSize);
    for (int i = 0; i < m_pointSize; ++i)
    {
        Bx[i] = m_pointQ[i].x - m_pointP[i].x;
        By[i] = m_pointQ[i].y - m_pointP[i].y;
        for (int j = 0; j < m_pointSize; ++j)
        {
            A(i, j) = std::pow(m_pointP[i].distance(m_pointP[j]) + m_radius[j], m_exponent / 2);
        }
    }
    // 解线性方程组 Ax = B；
    m_Ai.x = A.colPivHouseholderQr().solve(Bx);
    m_Ai.y = A.colPivHouseholderQr().solve(By);
}
