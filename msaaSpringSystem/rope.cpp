//
// Created by william on 2022/3/26.
//

#include "rope.h"

#include "mass.h"
#include "spring.h"

Rope::Rope(Vector2 start, Vector2 end, int numNodes, float nodeMass, float k, std::vector<int>& pinnedNodes)
{
    /// Part 1: Create a rope starting at `start`, ending at `end`, and containing `num_nodes` nodes.
    Vector2 interDistance = (end - start) / (float)numNodes;
    for (int i = 0; i < numNodes + 2; ++i)
    {
        auto* node = new Mass(start + interDistance * i, nodeMass, false);
        m_masses.push_back(node);
        if (i != 0)
        {
            auto* spring = new Spring(m_masses[i - 1], m_masses[i], k);
            m_springs.emplace_back(spring);
        }
    }

    for (auto& i : pinnedNodes)
    {
        m_masses[i]->pinned = true;
    }
}

void Rope::simulateVerlet(float deltaTime, const Vector2& gravity)
{
    for (auto& s : m_springs)
    {
        auto vecAB = s->m2->position - s->m1->position;
        // 胡克定律
        auto forceB = -s->k * vecAB.normalize() * (vecAB.distance(vecAB) - s->restLength);
        s->m2->forces += forceB;
        auto forceA = -forceB;
    }

    for (auto& m : m_masses)
    {
        if (!m->pinned)
        {
            auto tempPos = m->position;
            m->forces += gravity * m->mass; // F = ma
            auto a = m->forces / m->mass;   // a = F/m;
            // x(t+1)=x(t)+(1-dampingFactor)*[x[t]-x[t-1]]+a(t)*dt*dt
            float dampingFactor = 0.00003f; // 阻尼因子
            m->position += (1 - dampingFactor) * (m->position - m->lastPosition) + a * deltaTime * deltaTime;
        }
    }
}

void Rope::simulateEuler(float deltaTime, const Vector2& gravity)
{
    for (auto& s : m_springs)
    {
    }

    for (auto& m : m_masses)
    {
    }
}
