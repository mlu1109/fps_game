#include "glm/print.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.hpp"
#include "renderer/Constants.hpp"

Camera::Camera()
{
    updateDirection();
    updateWorldView();
    updateViewScreen();
}

void Camera::updateDirection()
{
    float angleYaw =  m_transform.R.y;
    float anglePitch = m_transform.R.z;

    m_d.x = sin(angleYaw) * cos(anglePitch);
    m_d.y = sin(anglePitch);
    m_d.z = cos(anglePitch) * cos(angleYaw);
    m_r.x = sin(angleYaw - M_HALF_PI);
    m_r.y = 0;
    m_r.z = cos(angleYaw - M_HALF_PI);
    m_u = glm::cross(m_r, m_d);
    updateWorldView();
}

glm::mat4 Camera::getWorldViewNormal() const
{
    return glm::transpose(glm::inverse(m_worldView));
}

void Camera::yaw(float rad)
{
    m_transform.R.y -= rad;
    updateDirection();
}

void Camera::pitch(float rad)
{
    float &anglePitch = m_transform.R.z;
    anglePitch += rad;
    if (anglePitch < -M_HALF_PI)
        anglePitch = -M_HALF_PI;
    else if (anglePitch > M_HALF_PI)
        anglePitch = M_HALF_PI;
    updateDirection();
}

void Camera::moveR(float amount)
{
    m_transform.T += amount * m_r;
    updateWorldView();
}

void Camera::moveD(float amount)
{
    m_transform.T -= amount * m_d;
    updateWorldView();
}

void Camera::updateAspectRatio(int width, int height)
{
    m_windowWidth = width;
    m_windowHeight = height;
    m_aspect = static_cast<float>(width) / static_cast<float>(height);
    m_viewScreen = glm::perspective(m_fovy, m_aspect, m_near, m_far);
}