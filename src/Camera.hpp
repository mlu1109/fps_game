#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Transform.hpp"

class Camera
{
    Transform m_transform;

    glm::vec3 m_r; // Right                 (Unit)
    glm::vec3 m_u; // Up                    (Unit)
    glm::vec3 m_d; // Direction/Forward     (Unit)
    
    glm::mat4 m_worldView;
    glm::mat4 m_viewScreen;

    int m_windowWidth;
    int m_windowHeight;

    float m_fovy = M_PI / 4.0f;
    float m_aspect = 4.0f / 3.0f;
    float m_near = 0.1f;
    float m_far = 1000.0f;

    void updateDirection();
    void updateWorldView() { m_worldView = glm::lookAt(m_transform.T, m_transform.T + m_d, m_u); };
    void updateViewScreen() { m_viewScreen = glm::perspective(m_fovy, m_aspect, m_near, m_far); }

  public:
    Camera();

    float getX() const { return m_transform.T.x; }
    float getZ() const { return m_transform.T.z; }

    void setTranslate(const glm::vec3 &p) { m_transform.T = p; updateWorldView();}
    //void setY(float y) { m_transform.T.y = y; updateWorldView(); }
    void setTransform(const Transform &t) { m_transform = t; }
    
    const glm::vec3 &getPosition() const { return m_transform.T; }
    const glm::vec3 &getDirection() const { return m_d; }
    const glm::mat4 &getViewScreen() const { return m_viewScreen; }
    const glm::mat4 &getWorldView() const { return m_worldView; }
    const Transform &getTransform() const { return m_transform; }
    int getWindowWidth() const { return m_windowWidth; }
    int getWindowHeight() const { return m_windowHeight; }

    void setY(float y) { m_transform.T.y = y; updateWorldView(); }
    
    glm::mat4 getWorldViewNormal() const;

    void yaw(float rad);
    void pitch(float rad);
    void moveD(float amount);
    void moveR(float amount);
    void updateAspectRatio(int windowWidth, int windowHeight);
    void translate(const glm::vec3 &t) { m_transform.T += t; updateWorldView(); }
};
