//
// Created by william on 2021/4/5.
//

#ifndef CROSS_PLATFORM_DEMO_COMMONINTERFACE_H
#define CROSS_PLATFORM_DEMO_COMMONINTERFACE_H

#include "base.h"
#include "commonMacro.h"
#include "program.h"

namespace graphicEngine::gl
{
class CommonInterface
{
public:
    CommonInterface();
    virtual ~CommonInterface();

public:
    virtual void initialize() = 0;
    virtual void update(float elapseTime) = 0;
    virtual void resize(int width, int height) = 0;
    virtual void render() = 0;
    virtual void touchEvent(double xPos, double yPos);
    virtual void scrollEvent(double xOffset, double yOffset);
    virtual void initImageUi();
    virtual void renderImageUi();
    void run();

protected:
    static void errorCallback(int error, const char* description);
    void initWithProperty(const std::tuple<const char*, std::string, std::string>& property);
    void initWithProperty();

protected:
    std::unique_ptr<graphicEngine::Program> m_program;
    GLFWwindow* m_window = nullptr;
    const char* m_glslVersion = nullptr;
    std::string m_title;
    std::string m_vertPath;
    std::string m_fragPath;
    GLuint m_vbo = 0;
    GLuint m_ebo = 0;
    GLuint m_vao = 0;
    bool m_openMsaa = false;
    constexpr static const unsigned int s_canvasWidth = 640;
    constexpr static const unsigned int s_canvasHeight = 640;
};
} // namespace graphicEngine::gl

#endif //CROSS_PLATFORM_DEMO_COMMONINTERFACE_H
