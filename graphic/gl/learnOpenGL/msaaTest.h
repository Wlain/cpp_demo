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
    void render() override;

protected:
    void drawCubes(const ProgramGL* program) override;
    void initPrograms() override;

};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_MSAATEST_H
