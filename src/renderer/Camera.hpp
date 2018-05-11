#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

class Camera
{
    glm::vec3 m_p{0.0f, 0.0f, 2.0f}; // Position
    glm::vec3 m_r; // Right                 (Unit)
    glm::vec3 m_u; // Up                    (Unit)
    glm::vec3 m_d; // Direction/Forward     (Unit)

    glm::mat4 m_worldView;
    glm::mat4 m_viewScreen;

    float m_angleYaw = -M_PI;
    float m_anglePitch = 0;

    float m_fovy = M_PI / 4.0f;
    float m_aspect = 4.0f / 3.0f;
    float m_near = 0.1f;
    float m_far = 1000.0f;

    void updateDirection();
    void updatePosition();
    void updateWorldView() { m_worldView = glm::lookAt(m_p, m_p + m_d, m_u); };
    void updateViewScreen() { m_viewScreen = glm::perspective(m_fovy, m_aspect, m_near, m_far); }

  public:
    Camera();

    float getX() const { return m_p.x; }
    float getZ() const { return m_p.z; }

    void setPosition(const glm::vec3 &p) { m_p = p; updateWorldView();}
    void setY(float y) { m_p.y = y; updateWorldView(); }
    
    const glm::vec3 &getPosition() const { return m_p; }
    const glm::vec3 &getDirection() const { return m_d; }
    const glm::mat4 &getViewScreen() const { return m_viewScreen; }
    const glm::mat4 &getWorldView() const { return m_worldView; }

    void yaw(float rad);
    void pitch(float rad);
    void moveD(float amount);
    void moveR(float amount);
    void updateAspectRatio(int windowWidth, int windowHeight);
};
