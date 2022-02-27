//
// Created by cwb on 2022/2/21.
//

#include "baseWarping.h"
BaseWarping::BaseWarping() = default;

BaseWarping::~BaseWarping() = default;

BaseWarping::BaseWarping(std::vector<Vector2> p, std::vector<Vector2> q) :
    m_pointP(std::move(p)), m_pointQ(std::move(q))
{
}

Vector2 BaseWarping::targetFunction(const Vector2& input)
{
    return {};
}

float BaseWarping::weightFunction(const Vector2& point, const Vector2& start)
{
    return {};
}

Vector2 BaseWarping::basicFunction(const Vector2& point, const Vector2& start, const Vector2& end)
{
    return {};
}

void BaseWarping::setPointP(const std::vector<Vector2>& mPointP)
{
    m_pointP = mPointP;
}
void BaseWarping::setPointQ(const std::vector<Vector2>& mPointQ)
{
    m_pointQ = mPointQ;
}
