#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "mesh.hpp"
#include "texture.hpp"
#include "vertexarray.hpp"

class Model
{
  std::vector<Mesh> m_meshes;
  glm::mat4 m_translate;
  glm::mat4 m_rotate;
  glm::mat4 m_scale;
  glm::mat4 m_modelWorld;

public:
  Model(const glm::mat4 &translate, const glm::mat4 &rotate, const glm::mat4 &scale);

  const glm::mat4 &getTranslate() const { return m_translate; }
  const glm::mat4 &getRotate() const { return m_rotate; }
  const glm::mat4 &getScale() const { return m_scale; }

  void updateModelWorld();
  void addMesh(const Mesh &mesh) { m_meshes.push_back(mesh); }
  void draw() const;
};
