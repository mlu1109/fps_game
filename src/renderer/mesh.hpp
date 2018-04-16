#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "texture.hpp"
#include "vertexarray.hpp"
#include "vertex.hpp"

class Mesh
{
    VertexArray *m_vertexArray;
    Texture *m_texture; // Optional
    // Mesh to model transforms
    glm::mat4 m_translate{1.0f};
    glm::mat4 m_rotate{1.0f};
    glm::mat4 m_scale{1.0f};
    glm::mat4 m_meshModel{1.0f}; // translate * rotate * scale
    glm::mat4 m_meshWorld{1.0f}; // modelWorld * meshModel

  public:
    Mesh(VertexArray *, Texture *, glm::mat4 translate = glm::mat4{1.0f}, glm::mat4 rotate = glm::mat4{1.0f}, glm::mat4 scale = glm::mat4{1.0f});

    const glm::mat4 &getMeshWorld() const { return m_meshWorld; }
    const VertexArray *getVertexArray() const { return m_vertexArray; }
    const Texture *getTexture() const { return m_texture; }

    void updateMeshWorld(const glm::mat4 &modelWorld) { m_meshWorld = modelWorld * m_meshModel; };
};