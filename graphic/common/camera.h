//
// Created by cwb on 2022/2/7.
//

#ifndef CPP_DEMO_CAMERA_H
#define CPP_DEMO_CAMERA_H

#include "base.h"
#include "glew/include/GL/glew.h"
#include "glm/glm/gtc/type_ptr.hpp" // glm::value_ptr
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/glm.hpp"

enum class CameraMovement
{
    Forward,
    Backward,
    Left,
    Right
};

// Default camera values
constexpr static const float YAW = -90.0f;
constexpr static const float PITCH = 0.0f;
constexpr static const float SPEED = 2.5f;
constexpr static const float SENSITIVITY = 0.1f;
constexpr static const float ZOOM = 45.0f;

class Camera
{
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
    void processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);
    void processKeyboard(CameraMovement direction, float deltaTime);
    void processMouseScroll(float yOffset);
    glm::mat4 calcViewMatrix(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp);
    inline glm::mat4 viewMatrix()
    {
        return glm::lookAt(m_position, m_position + m_front, m_up);
    }
    inline glm::mat4 projectionMatrix(int width, int height, float near, float far)
    {
        return glm::perspective(glm::radians(m_fovY), (float)width / (float)height, near, far);
    }

private:
    void updateCameraVectors();

public:
    // camera Attributes
    glm::vec3 m_position{};
    glm::vec3 m_front;
    glm::vec3 m_up{};
    glm::vec3 m_right{};
    glm::vec3 m_worldUp{};
    // euler Angles
    float m_yaw{};
    float m_pitch{};
    float m_movementSpeed{};
    float m_mouseSensitivity{};
    float m_fovY{};
};

#endif //CPP_DEMO_CAMERA_H
