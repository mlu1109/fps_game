#include <glm/gtc/matrix_transform.hpp>
#include "Camera.hpp"

Camera::Camera(const glm::vec3 &position, const glm::vec3 &look, const glm::vec3 &up)
    : m_position(position), m_look(look), m_up(up)
{
    m_viewScreen = glm::perspective(m_fovy, m_aspect, m_near, m_far);
}

void Camera::yaw(float rad)
{
    glm::vec3 z = m_look - m_position;
    // Rotation around the up vector
    glm::mat4 transform{1};
    transform = glm::rotate(transform, -rad, m_up);
    m_look = glm::mat3{transform} * z;
}

void Camera::pitch(float rad)
{
    glm::vec3 z = m_look - m_position;
    // Rotation around z x up
    glm::vec3 axis = glm::cross(z, m_up);
    glm::mat4 transform{1};
    transform = glm::rotate(glm::mat4{1}, rad, axis);
    m_look = glm::mat3{transform} * m_look;
}

void Camera::roll(float rad)
{
    // Rotation around m_look - m_position
    glm::vec3 axis = m_look - m_position;
    glm::mat3 transform(glm::rotate(glm::mat4{1}, rad, axis));
    m_look = transform * m_look;
}

void Camera::moveX(float amount)
{
    glm::vec3 z = m_look - m_position;
    glm::vec3 moveX = amount * glm::normalize(glm::cross(z, m_up));
    m_position += moveX;
}

void Camera::moveZ(float amount)
{
    glm::vec3 z = m_look - m_position;
    glm::vec3 moveZ = amount * glm::normalize(z);
    m_position += moveZ;
}

void Camera::updateAspectRatio(int width, int height)
{
    m_aspect = static_cast<float>(width) / static_cast<float>(height);
    m_viewScreen = glm::perspective(m_fovy, m_aspect, m_near, m_far);
}