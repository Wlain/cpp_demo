//
// Created by william on 2022/3/26.
//

#include "rope.h"

#include "mass.h"
#include "spring.h"
Rope::Rope(Vector2 start, Vector2 end, int numNodes, float nodeMass, float k, std::vector<int> pinnedNodes)
{
    if (numNodes <= 1)
        return;
    for (int i = 0; i < numNodes; ++i)
    {
        auto position = start + (end - start) * (float)i / ((float)numNodes - 1.0f);
        m_masses.push_back(new Mass(position, nodeMass, false));
    }
    // Comment-in this part when you implement the constructor
    for (auto& i : pinnedNodes)
    {
        m_masses[i]->pinned = true;
    }
    for (int i = 0; i < numNodes - 1; ++i)
    {
        m_springs.push_back(new Spring(m_masses[i], m_masses[i + 1], k));
    }
}

// 求解离散时间上的积分
void Rope::simulateVerlet(float deltaTime, const Vector2& gravity)
{
    for (auto& s : m_springs)
    {
        auto ab = s->m2->position - s->m1->position;
        auto abMod = ab.norm();
        // 胡克定律
        auto f = s->k * ab / abMod * (abMod / s->restLength - 1);
        s->m1->forces += f;
        s->m2->forces -= f;
    }

    for (auto& m : m_masses)
    {
        if (!m->pinned)
        {
            m->forces += gravity * m->mass; // f = mg;
            auto a = m->forces / m->mass;   // a = f/m;
            auto lastPosition = m->position;
            // 阻尼因子
            float dampFactor = 0.00005;
            m->position = m->position + (1 - dampFactor) * (m->position - m->lastPosition) + a * deltaTime * deltaTime;
            m->lastPosition = lastPosition;
        }
        m->forces.set(0.0f, 0.0f);
    }
}

// 欧拉求解
void Rope::simulateEuler(float deltaTime, const Vector2& gravity)
{
    for (auto& s : m_springs)
    {
        auto ab = s->m2->position - s->m1->position;
        auto abMod = ab.norm();
        // 胡克定律
        auto f = s->k * ab / abMod * (abMod / s->restLength - 1);
        s->m1->forces += f;
        s->m2->forces -= f;
    }
    bool isExplicit = false;
    for (auto& m : m_masses)
    {
        if(!m->pinned)
        {
            m->forces += gravity * m->mass; // f = mg;
            // 阻尼因子
            float dampFactor = 0.01;
            m->forces += -dampFactor * m->velocity;
            auto a = m->forces / m->mass;
            auto v0 = m->velocity;
            auto vt = v0 + a * deltaTime;
            m->velocity = m->velocity + a * deltaTime;
            m->position = m->position + vt * deltaTime;
        }
        m->forces.set(0.0f, 0.0f);
    }
}
