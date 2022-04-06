//
// Created by william on 2021/4/5.
//

#ifndef CROSS_PLATFORM_DEMO_COMMONINTERFACE_H
#define CROSS_PLATFORM_DEMO_COMMONINTERFACE_H

#include "base.h"
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
    virtual void keyEvent(int key, int scancode, int action, int modifiers);
    void run();

protected:
    static void errorCallback(int error, const char* description);
    void initWithProperty(const std::tuple<const char*, std::string, std::string>& property);

protected:
    std::unique_ptr<graphicEngine::Program> m_program;
    GLFWwindow* m_window = nullptr;
    std::string m_title;
    GLuint m_vbo = 0;
    GLuint m_ebo = 0;
    GLuint m_vao = 0;
    constexpr static const unsigned int s_canvasWidth = 640;
    constexpr static const unsigned int s_canvasHeight = 640;
};
} // namespace graphicEngine::gl

#endif //CROSS_PLATFORM_DEMO_COMMONINTERFACE_H
