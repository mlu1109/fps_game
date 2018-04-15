#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "vertexarray.hpp"

class Model
{
  std::vector<VertexArray *> m_meshes;
  glm::mat4 m_translate;
  glm::mat4 m_rotate;
  glm::mat4 m_scale;

public:
  Model(const std::vector<VertexArray *> &meshes, const glm::mat4 &translate, const glm::mat4 &rotate, const glm::mat4 &scale);

  const glm::mat4 &getTranslate() const { return m_translate; }
  const glm::mat4 &getRotate() const { return m_rotate; }
  const glm::mat4 &getScale() const { return m_scale; }

  glm::mat4 getModelToWorld() const { return m_translate * m_rotate * m_scale; }
  void draw() const;
};
