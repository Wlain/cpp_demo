//
// Created by cwb on 2022/2/7.
//

#ifndef CPP_DEMO_WINDOWCLEAR_H
#define CPP_DEMO_WINDOWCLEAR_H

#include "window.h"

namespace graphicEngine::gl
{
class WindowClear : public Window
{
public:
    ~WindowClear() override;
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void display() override;
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_WINDOWCLEAR_H
