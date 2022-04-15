//
// Created by cwb on 2022/4/14.
//

#ifndef CPP_DEMO_MSAATESTOffscreen_H
#define CPP_DEMO_MSAATESTOffscreen_H
#include "msaaTest.h"
namespace graphicEngine::gl
{
class MsaaOffscreen : public MsaaTest
{
public:
    ~MsaaOffscreen() override;
    void initialize() override;
    void render() override;

protected:
    void initVertices() override;
    void initVertexAttrib() override;
    void initPrograms() override;
    void initTextures() override;

private:
    void initFbo();

private:
    std::unique_ptr<Program> m_quadProgram;
    std::vector<float> m_quadVertices;
    GLuint m_quadVao = 0;
    GLuint m_quadVbo = 0;
    GLuint m_quadFbo;
    GLuint m_quadTextureID;

    GLuint m_multiFbo;
    GLuint m_multiRbo;
    GLuint m_multiTextureID;
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_MSAATESTOffscreen_H
