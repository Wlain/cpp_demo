//
// Created by cwb on 2022/4/8.
//

#ifndef CPP_DEMO_DEPTHTEST_H
#define CPP_DEMO_DEPTHTEST_H
#include "commonInterface.h"
class Camera;
class TextureGL;

namespace graphicEngine::gl
{
class DepthTest : public CommonInterface
{
public:
    DepthTest();
    ~DepthTest() override;
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void render() override;
    void touchEvent(double xPos, double yPos) override;
    void scrollEvent(double xOffset, double yOffset) override;
protected:
    void processInput();

private:
    std::unique_ptr<TextureGL> m_texture;
    std::unique_ptr<TextureGL> m_texture1;
    std::unique_ptr<Camera> m_camera;
    std::vector<float> m_cubeVertices;
    std::vector<float> m_planeVertices;
    GLuint m_planeVao = 0;
    GLuint m_planeVbo = 0;
    int m_width = 0;
    int m_height = 0;

    float m_elapseTime = 0.0f;
    // timing
    float m_lastTime = 0.0f;
    float m_deltaTime = 0.0f;
    float m_cameraSpeed = 0.0f;
    // camera
    float m_lastX = s_canvasWidth / 2.0f;
    float m_lastY = s_canvasHeight / 2.0f;
    bool m_firstMouse = true;
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_DEPTHTEST_H
