//
// Created by william on 2022/4/11.
//

#ifndef CPP_DEMO_ADVANCEDUBO_H
#define CPP_DEMO_ADVANCEDUBO_H
#include "depthTest.h"

namespace graphicEngine::gl
{
class AdvancedUbo : public DepthTest
{
public:
    ~AdvancedUbo() override;
    void initialize() override;
    void update(float elapseTime) override;
    void render() override;

protected:
    void initPrograms() override;
    void drawCubes() override;
    void initUbo();

private:
    std::unique_ptr<graphicEngine::ProgramGL> m_redProgram;
    std::unique_ptr<graphicEngine::ProgramGL> m_blueProgram;
    std::unique_ptr<graphicEngine::ProgramGL> m_greenProgram;
    std::unique_ptr<graphicEngine::ProgramGL> m_yellowProgram;
    GLuint m_uboMatrices = 0;
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_ADVANCEDUBO_H
