#include "../glm/print.hpp"
#include "Camera.hpp"
#include "Constants.hpp"

Camera::Camera()
{
    updateDirection();
    updateWorldView();
    updateViewScreen();
}

void Camera::updateDirection()
{
    m_d.x = cos(m_anglePitch) * sin(m_angleYaw);
    m_d.y = sin(m_anglePitch);
    m_d.z = cos(m_anglePitch) * cos(m_angleYaw);
    m_r.x = sin(m_angleYaw - M_HALF_PI);
    m_r.y = 0;
    m_r.z = cos(m_angleYaw - M_HALF_PI);
    m_u = glm::cross(m_r, m_d);
    updateWorldView();
}

glm::mat4 Camera::getWorldViewNormal() const
{
    return glm::transpose(glm::inverse(m_worldView));
}

void Camera::yaw(float rad)
{
    m_angleYaw -= rad;
    updateDirection();
}

void Camera::pitch(float rad)
{
    m_anglePitch += rad;
    if (m_anglePitch < -M_HALF_PI)
        m_anglePitch = -M_HALF_PI;
    else if (m_anglePitch > M_HALF_PI)
        m_anglePitch = M_HALF_PI;
    updateDirection();
}

void Camera::moveR(float amount)
{
    m_p += amount * m_r;
    updateWorldView();
}

void Camera::moveD(float amount)
{
    m_p -= amount * m_d;
    updateWorldView();
}

void Camera::updateAspectRatio(int width, int height)
{
    m_aspect = static_cast<float>(width) / static_cast<float>(height);
    m_viewScreen = glm::perspective(m_fovy, m_aspect, m_near, m_far);
}