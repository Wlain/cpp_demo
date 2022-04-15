//
// Created by cwb on 2022/4/15.
//

#ifndef CPP_DEMO_ADVANCEDLIGHTING_H
#define CPP_DEMO_ADVANCEDLIGHTING_H
#include "depthTest.h"
namespace graphicEngine::gl
{
class AdvancedLighting : public DepthTest
{
public:
    void render() override;
    void update(float elapseTime) override;

protected:
    void initTextures() override;
    void processInput() override;
    void initPrograms() override;
    void initVertices() override;
    void initVertexAttrib() override;
    void initPlaneVertexAttrib() override;

private:
    glm::vec3 m_lightPos{ 0.0f, 0.0f, 0.0f };
    bool m_blinn = false;
    bool m_blinnKeyPressed = false;
};
}

#endif //CPP_DEMO_ADVANCEDLIGHTING_H
