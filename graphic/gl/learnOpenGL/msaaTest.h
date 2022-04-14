//
// Created by cwb on 2022/4/14.
//

#ifndef CPP_DEMO_MSAATEST_H
#define CPP_DEMO_MSAATEST_H
#include "depthTest.h"
namespace graphicEngine::gl
{
class MsaaTest : public DepthTest
{
public:
    MsaaTest();

protected:
    void drawCubes() override;
    void initPrograms() override;

public:
    void render() override;
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_MSAATEST_H
