//
// Created by cwb on 2022/4/8.
//

#ifndef CPP_DEMO_StencilTest_H
#define CPP_DEMO_StencilTest_H
#include "depthTest.h"

namespace graphicEngine::gl
{
class StencilTest : public DepthTest
{
public:
    using DepthTest::DepthTest;
    ~StencilTest() override;
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void render() override;
protected:
    void initPrograms() override;
    void initGLStatus() override;

private:
    std::unique_ptr<ProgramGL> m_singleColorProgram;
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_StencilTest_H
