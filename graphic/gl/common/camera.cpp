//
// Created by cwb on 2022/4/7.
//
#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
    m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(SPEED), m_mouseSensitivity(SENSITIVITY), m_fovY(ZOOM)
{
    m_position = position;
    m_worldUp = up;
    m_yaw = yaw;
    m_pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
    m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(SPEED), m_mouseSensitivity(SENSITIVITY), m_fovY(ZOOM)
{
    m_position = glm::vec3(posX, posY, posZ);
    m_worldUp = glm::vec3(upX, upY, upZ);
    m_yaw = yaw;
    m_pitch = pitch;
    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    m_right = glm::normalize(glm::cross(m_front, m_worldUp)); // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

void Camera::processMouseScroll(float yOffset)
{
    m_fovY -= (float)yOffset;
    m_fovY = std::clamp(m_fovY, 1.0f, 45.0f);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime)
{
    float velocity = m_movementSpeed * deltaTime;
    if (direction == CameraMovement::Forward)
        m_position += m_front * velocity;
    if (direction == CameraMovement::Backward)
        m_position -= m_front * velocity;
    if (direction == CameraMovement::Left)
        m_position -= m_right * velocity;
    if (direction == CameraMovement::Right)
        m_position += m_right * velocity;
    m_position.y = 0.0f; // <-- this one-liner keeps the user at the ground level (xz plane)
}

glm::mat4 Camera::calcViewMatrix(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp)
{
    auto zaxis = glm::normalize(position - target);
    auto xaxis = glm::normalize(glm::cross(glm::normalize(worldUp), zaxis));
    auto yaxis = glm::cross(zaxis, xaxis);
    glm::mat4 translation = glm::mat4(1.0f); // Identity matrix by default
    translation[3][0] = -position.x; // Third column, first row
    translation[3][1] = -position.y;
    translation[3][2] = -position.z;
    glm::mat4 rotation = glm::mat4(1.0f);
    rotation[0][0] = xaxis.x; // First column, first row
    rotation[1][0] = xaxis.y;
    rotation[2][0] = xaxis.z;
    rotation[0][1] = yaxis.x; // First column, second row
    rotation[1][1] = yaxis.y;
    rotation[2][1] = yaxis.z;
    rotation[0][2] = zaxis.x; // First column, third row
    rotation[1][2] = zaxis.y;
    rotation[2][2] = zaxis.z;
    return rotation * translation;
}

void Camera::processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch)
{
    xOffset *= m_mouseSensitivity;
    yOffset *= m_mouseSensitivity;

    m_yaw += xOffset;
    m_pitch += yOffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);
    }
    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}