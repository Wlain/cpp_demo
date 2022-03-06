//
// Created by cwb on 2022/2/21.
//

#include "imageWarping/rbfWarping.h"

#include <iostream>
#include <utility>

RbfWarping::RbfWarping(std::vector<Vector2> p, std::vector<Vector2> q) :
    BaseWarping(std::move(p), std::move(q))
{
    m_exponent = 0.5;
    calcRadius();
    calcWeight();
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
        x += m_weightX(i) * std::pow(std::pow(m_pointP[i].distance(input), 2) + std::pow(m_radius[i], 2), m_exponent);
        y += m_weightY(i) * std::pow(std::pow(m_pointP[i].distance(input), 2) + std::pow(m_radius[i], 2), m_exponent);
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
            auto distance = m_pointP[i].distance(m_pointP[j]);
            if (distance < ri)
            {
                ri = distance;
            }
            m_radius.emplace_back(ri);
        }
    }
}

void RbfWarping::calcWeight()
{
    Eigen::MatrixXd A(m_pointSize, m_pointSize);
    A.setZero();
    Eigen::VectorXd Bx(m_pointSize);
    Bx.setZero();
    Eigen::VectorXd By(m_pointSize);
    By.setZero();
    for (int i = 0; i < m_pointSize; ++i)
    {
        Bx[i] = m_pointQ[i].x - m_pointP[i].x;
        By[i] = m_pointQ[i].y - m_pointP[i].y;
        for (int j = 0; j < m_pointSize; ++j)
        {
            A(i, j) = std::pow(std::pow(m_pointP[i].distance(m_pointP[j]), 2) + pow(m_radius[j], 2), m_exponent);
        }
    }
    // 解线性方程组 Ax = B；
    m_weightX = A.colPivHouseholderQr().solve(Bx);
    m_weightY = A.colPivHouseholderQr().solve(By);
}
