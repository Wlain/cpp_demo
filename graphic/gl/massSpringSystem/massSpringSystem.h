//
// Created by william on 2022/3/25.
//

#ifndef CPP_DEMO_MASSSPRINGSYSTEM_H
#define CPP_DEMO_MASSSPRINGSYSTEM_H

#include "test/triangle.h"
#include "vector2.h"

class Rope;

namespace graphicEngine::gl
{
class massSpringSystem : public CommonInterface
{
public:
    struct Config
    {
        float mass = 1.0f;
        float ks = 10000.0f;
        float stepsPerFrame = 64.0f;
        Vector2 gravity = { 0.0f, -1.0f };
    };

public:
    using CommonInterface::CommonInterface;
    explicit massSpringSystem(Config config);
    ~massSpringSystem() override;
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void display() override;

private:
    Config m_config;
    Rope* m_verletRope{};
    Rope* m_eulerRope{};
    Triangle::Vertex m_vertices[10]{};
};

} // namespace graphicEngine::gl
#endif //CPP_DEMO_MASSSPRINGSYSTEM_H
