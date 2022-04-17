//
// Created by cwb on 2022/4/7.
//

#ifndef CPP_DEMO_COLORS_H
#define CPP_DEMO_COLORS_H
#include "gl/common/commonInterface.h"
class Camera;
namespace graphicEngine::gl
{
class Colors : public CommonInterface
{
public:
    Colors();
    ~Colors() override;
    void render() override;
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void touchEvent(double xPos, double yPos) override;
    void scrollEvent(double xOffset, double yOffset) override;
    virtual void initLighting();
    virtual void initCube();
protected:
    void processInput();

protected:
    std::unique_ptr<graphicEngine::ProgramGL> m_lightingProgram;
    std::unique_ptr<graphicEngine::ProgramGL> m_lightCubeProgram;
    std::unique_ptr<Camera> m_camera;
    std::vector<float> m_verticesCube = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };
    glm::vec3 m_lightPos{ 1.2f, 1.0f, 2.0f };
    GLuint m_lightVao = 0;
    int32_t m_width{};
    int32_t m_height{};
    float m_lastX = s_canvasWidth / 2.0f;
    float m_lastY = s_canvasHeight / 2.0f;
    // timing
    float m_deltaTime = 0.0f;
    float m_lastTime = 0.0f;
    float m_cameraSpeed = 0.0f;
    bool m_firstMouse = true;
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_COLORS_H
