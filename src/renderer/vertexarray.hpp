#pragma once

#include <GL/glew.h>
#include <vector>
#include "vertex.hpp"

class VertexArray
{
private:
  GLuint m_id;
  GLuint m_vertexBufferId;
  GLuint m_indexBufferId;
  GLsizei m_vertexCount;
  GLsizei m_indexCount;

public:
  VertexArray(const std::vector<Vertex> &vertices);
  VertexArray(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices);
  VertexArray(const VertexArray &) = delete;
  VertexArray(VertexArray &&) = delete;
  ~VertexArray();

  GLsizei getVertexCount() const { return m_vertexCount; }
  GLsizei getIndexCount() const { return m_indexCount; }

  void bind() const { glBindVertexArray(m_id); }
};
