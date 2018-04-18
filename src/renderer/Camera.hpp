#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

class Camera
{
  glm::vec3 m_position;
  glm::vec3 m_look;
  glm::vec3 m_up;

  float m_fovy = M_PI / 4.0f;
  float m_aspect = 4.0f / 3.0f;
  float m_near = 0.1f;
  float m_far = 100.0f;

  glm::mat4 m_viewScreen;

public:
  Camera(const glm::vec3 &position, const glm::vec3 &look, const glm::vec3 &up);

  const glm::mat4 &getViewScreen() const { return m_viewScreen; }
  glm::mat4 getWorldView() const { return glm::lookAt(m_position, m_look, m_up);; };

  void yaw(float rad);
  void pitch(float rad);
  void roll(float rad);
  void moveX(float amount);
  void moveZ(float amount);

  void updateAspectRatio(int windowWidth, int windowHeight);
};