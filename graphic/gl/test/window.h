//
// Created by cwb on 2022/2/7.
//

#ifndef CPP_DEMO_WINDOW_H
#define CPP_DEMO_WINDOW_H
#include "commonInterface.h"

namespace graphicEngine::gl
{
class Window : public CommonInterface
{
public:
    using CommonInterface::CommonInterface;
    ~Window() override;

public:
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void display() override;

private:
    float m_offsetX = 0.0f;
    float m_inc = 0.01f;
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_WINDOW_H
