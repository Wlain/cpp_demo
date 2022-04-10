//
// Created by william on 2022/4/9.
//

#ifndef CPP_DEMO_SKYBOX_H
#define CPP_DEMO_SKYBOX_H
#include "depthTest.h"

namespace graphicEngine::gl
{
class Skybox : public DepthTest
{
public:
    ~Skybox() override;
    void update(float elapseTime) override;
    void render() override;

protected:
    void initVertices() override;
    void initPrograms() override;
    void initVertexAttrib() override;
    void initTextures() override;

private:
    GLuint loadCubeMaps(std::vector<std::string> faces);

protected:
    std::unique_ptr<Program> m_skyboxProgram;
    std::vector<float> m_skyboxVertices;
    GLuint m_cubeMapTexture = 0;
    GLuint m_skyboxVao = 0;
    GLuint m_skyboxVbo = 0;
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_SKYBOX_H
