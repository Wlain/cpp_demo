//
// Created by william on 2021/4/5.
//

#include "commonInterface.h"

#include "utils.h"

namespace graphicEngine::gl
{
CommonInterface::CommonInterface()
{
    std::cout << "CommonInterface constructor called" << std::endl;
}

CommonInterface::~CommonInterface()
{
    std::cout << "~CommonInterface destructor called" << std::endl;
    if (m_vbo > 0)
        glDeleteBuffers(1, &m_vbo);
    if (m_ebo > 0)
        glDeleteBuffers(1, &m_ebo);
    if (m_vao > 0)
        glDeleteVertexArrays(1, &m_vao);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void CommonInterface::initWithProperty(const std::tuple<const char*, std::string, std::string>& property)
{
    glfwSetWindowTitle(m_window, std::get<0>(property));
    // init program
    m_program = std::make_unique<graphicEngine::Program>(std::get<1>(property), std::get<2>(property));
}

void CommonInterface::initWithProperty()
{
    glfwSetWindowTitle(m_window, m_title.c_str());
    // init program
    m_program = std::make_unique<graphicEngine::Program>(m_vertPath, m_fragPath);
}

void CommonInterface::run()
{
    glfwSetErrorCallback(errorCallback);
    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // glfw window creation
    m_window = glfwCreateWindow(s_canvasWidth, s_canvasHeight, m_title.c_str(), nullptr, nullptr);
    if (m_window == nullptr)
    {
        LOG_ERROR("Failed to create GLFW window");
        ASSERT(0);
        glfwTerminate();
        return;
    }
    glfwSetWindowUserPointer(m_window, this);
    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto* demo = reinterpret_cast<CommonInterface*>(glfwGetWindowUserPointer(window));
        demo->keyEvent(key, scancode, action, mods);
    });
    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        auto* demo = reinterpret_cast<CommonInterface*>(glfwGetWindowUserPointer(window));
        demo->resize(width, height);
    });
    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos) {
        auto* demo = reinterpret_cast<CommonInterface*>(glfwGetWindowUserPointer(window));
        demo->touchEvent(xPos, yPos);
    });
    if (glewInit() != GLEW_OK)
    {
        exit(EXIT_FAILURE);
    }
    initialize();
    int width = 0, height = 0;
    glfwGetFramebufferSize(m_window, &width, &height);
    resize(width, height);
    while (!glfwWindowShouldClose(m_window))
    {
        update((float)glfwGetTime());
        /// 渲染
        render();
        /// swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void CommonInterface::keyEvent(int key, int scancode, int action, int modifiers)
{
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(m_window, true);
            break;
        case GLFW_KEY_W:
            break;
        case GLFW_KEY_S:
            break;
        case GLFW_KEY_A:
            break;
        case GLFW_KEY_D:
            break;
        default:
            break;
        }
    }
}

void CommonInterface::touchEvent(double xPos, double yPos)
{
}

void CommonInterface::errorCallback(int error, const char* description)
{
    LOG_ERROR("generate type: %d, glError: %s", error, description);
}
} // namespace graphicEngine::gl