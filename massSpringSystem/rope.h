//
// Created by william on 2022/3/26.
//

#ifndef CPP_DEMO_ROPE_H
#define CPP_DEMO_ROPE_H
/// 绳索类
#include "vector2.h"

#include <vector>

class Mass;
class Spring;

class Rope
{
public:
    Rope(std::vector<Mass*> masses, std::vector<Spring*> springs) :
        m_masses(std::move(masses)), m_springs(std::move(springs))
    {
    }

    Rope(Vector2 start, Vector2 end, int numNodes, float nodeMass, float k,
         std::vector<int>& pinnedNodes);

    void simulateVerlet(float deltaTime, const Vector2& gravity);
    void simulateEuler(float deltaTime, const Vector2& gravity);

    std::vector<Mass*> m_masses;
    std::vector<Spring*> m_springs;
};

#endif //CPP_DEMO_ROPE_H
