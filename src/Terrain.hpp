#pragma once

#include "renderer/Shader.hpp"
#include "renderer/Texture.hpp"
#include "renderer/VertexArray.hpp"
#include "Heightmap.hpp"

class Terrain
{
    int m_width;
    int m_height;
    int m_quadWidth;
    int m_quadHeight;

    const VertexArray *m_vertexArray;

    std::vector<Vertex> m_vertices;

    const glm::mat4 m_modelView{1.0f}; // Moving/scaling/rotating terrain unsupported

  public:
    Terrain(const Heightmap &, const VertexArray *);

    const VertexArray *getVertexArray() const { return m_vertexArray; }
    float getY(float x, float z) const;
    int getWidth() const { return m_width * m_quadWidth; }
    int getHeight() const { return m_height * m_quadHeight; }
};