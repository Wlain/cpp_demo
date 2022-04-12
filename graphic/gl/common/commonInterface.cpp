//
// Created by william on 2021/4/5.
//

#include "commonInterface.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "utils.h"

namespace graphicEngine::gl
{
CommonInterface::CommonInterface()
{
    m_glslVersion = "#version 330";
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
    m_program = std::make_unique<graphicEngine::Program>(std::get<1>(property).c_str(), std::get<2>(property).c_str());
}

void CommonInterface::initWithProperty()
{
    glfwSetWindowTitle(m_window, m_title.c_str());
    // init program
    m_program = std::make_unique<graphicEngine::Program>(m_vertPath.c_str(), m_fragPath.c_str());
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
    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        auto* demo = reinterpret_cast<CommonInterface*>(glfwGetWindowUserPointer(window));
        demo->resize(width, height);
    });
    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos) {
        auto* demo = reinterpret_cast<CommonInterface*>(glfwGetWindowUserPointer(window));
        demo->touchEvent(xPos, yPos);
    });
    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset) {
        auto* demo = reinterpret_cast<CommonInterface*>(glfwGetWindowUserPointer(window));
        demo->scrollEvent(xOffset, yOffset);
    });
    // tell GLFW to capture our mouse
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glewInit() != GLEW_OK)
    {
        exit(EXIT_FAILURE);
    }
    initialize();
    initImageUi();
    int width = 0, height = 0;
    glfwGetFramebufferSize(m_window, &width, &height);
    resize(width, height);
    while (!glfwWindowShouldClose(m_window))
    {
        update((float)glfwGetTime());
        if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(m_window, true);
        /// 渲染
        render();
        renderImageUi();
        /// swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void CommonInterface::touchEvent(double xPos, double yPos)
{
}

void CommonInterface::errorCallback(int error, const char* description)
{
    LOG_ERROR("generate type: %d, glError: %s", error, description);
}

void CommonInterface::scrollEvent(double xOffset, double yOffset)
{
}

void CommonInterface::initImageUi()
{
    ImGui::CreateContext();
    // 设置样式
    ImGui::StyleColorsDark();
    // 设置平台和渲染器
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init(m_glslVersion);
}

void CommonInterface::renderImageUi()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("imgui_window");
    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
} // namespace graphicEngine::gl