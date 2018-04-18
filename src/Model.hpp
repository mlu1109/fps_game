#pragma once
#include <vector>
#include "Mesh.hpp"
#include "Transform.hpp"
#include "renderer/Drawable.hpp"

class Model : public Drawable
{
    Mesh m_mesh;
    Transform m_transform;
    glm::mat4 m_modelWorld;

  public:
    Model(const Mesh &mesh, const Transform &transform = Transform{})
        : m_mesh{mesh}, m_transform{transform} { updateModelWorld(); }

    // Drawable
    const std::shared_ptr<Shader> &getShader() const override { return m_mesh.shader; }
    const std::shared_ptr<Texture> &getTexture() const override { return m_mesh.texture; }
    const std::shared_ptr<VertexArray> &getVertexArray() const override { return m_mesh.vertexArray; }
    const glm::mat4 &getModelWorld() const override { return m_modelWorld; }

    void setTranslate(const glm::vec3 &v) { m_transform.T = v; }
    void setRotate(const glm::vec3 &v) { m_transform.R = v; }
    void setScale(const glm::vec3 &v) { m_transform.S = v; }

    void translate(const glm::vec3 &v) { m_transform.T += v; }
    void rotate(const glm::vec3 &v) { m_transform.R += v; }
    void scale(const glm::vec3 &v) { m_transform.S += v; };
    void updateModelWorld() { m_modelWorld = m_transform.getTransform(); }
};
