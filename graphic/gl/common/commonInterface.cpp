//
// Created by william on 2021/4/5.
//

#include "commonInterface.h"
#include "camera.h"
#include "utils.h"
constexpr const unsigned int SCR_WIDTH = 640;
constexpr const unsigned int SCR_HEIGHT = 480;

float lastX = (float)SCR_WIDTH  / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

namespace graphicEngine::gl
{
CommonInterface::CommonInterface()
{
    std::cout << "CommonInterface constructor called" << std::endl;
}

CommonInterface::~CommonInterface()
{
    std::cout << "~CommonInterface destructor called" << std::endl;
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void CommonInterface::mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    float xpos = static_cast<float>(xPos);
    float ypos = static_cast<float>(yPos);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}


void CommonInterface::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(CameraMovement::Forward, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(CameraMovement::Backward, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(CameraMovement::Left, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(CameraMovement::Right, deltaTime);
}

void CommonInterface::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void CommonInterface::errorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void CommonInterface::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void CommonInterface::initWithProperty(const char* title)
{
    glfwSetErrorCallback(errorCallback);
    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    m_window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title, nullptr, nullptr);
    glfwSetKeyCallback(m_window, keyCallback);
    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);
    glfwSetCursorPosCallback(m_window, mouseCallback);
    if (glewInit() != GLEW_OK)
    {
        exit(EXIT_FAILURE);
    }
    glfwSwapInterval(1);
}

void CommonInterface::initWithProperty(const std::tuple<const char*, std::string, std::string>& property)
{
    initWithProperty(std::get<0>(property));
    // init program
    m_program = std::make_unique<graphicEngine::Program>(std::get<1>(property), std::get<2>(property));
}

void CommonInterface::update(float elapseTime)
{
}

void CommonInterface::resize(int width, int height)
{

}

void CommonInterface::display()
{
}

void CommonInterface::draw()
{
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
    if (width != m_currentWidth || height != m_currentHeight)
    {
        resize(width, height);
        m_currentWidth = width;
        m_currentHeight = height;
    }
    while (!glfwWindowShouldClose(m_window))
    {
        processInput(m_window);
        update((float)glfwGetTime());
        display();
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}
} // namespace graphicEngine