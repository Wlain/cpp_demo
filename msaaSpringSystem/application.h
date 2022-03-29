//
// Created by william on 2022/3/25.
//

#ifndef CPP_DEMO_APPLICATION_H
#define CPP_DEMO_APPLICATION_H

#include "commonInterface.h"
#include "vector2.h"

class Application : public graphicEngine::gl::CommonInterface
{
public:
    struct Config
    {
        float mass = 1.0f;
        float ks = 100.0f;
        float stepsPerFrame = 64.0f;
        Vector2 gravity = { 0.0f, -1.0f };
    };

public:
    using CommonInterface::CommonInterface;
    explicit Application(Config  config);
    ~Application() override;
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void display() override;
    void draw() override;

private:
    Config m_config;
};

#endif //CPP_DEMO_APPLICATION_H
