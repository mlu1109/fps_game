#pragma once
#include <vector>
#include "BoundingSphere.hpp"
#include "AABB.hpp"
#include "Mesh.hpp"
#include "Transform.hpp"
#include "renderer/Drawable.hpp"

class Model : public Drawable
{
    Mesh m_mesh;
    Transform m_transform;
    glm::mat4 m_modelWorld;

    BoundingSphere m_boundingSphere;
    AABB m_aabb;

  public:
    Model(const Mesh &mesh, const Transform &transform = Transform{})
        : m_mesh{mesh}, m_transform{transform} { updateModelWorld(); }

    // Drawable
    const std::shared_ptr<Shader> &getShader() const override { return m_mesh.shader; }
    const std::shared_ptr<Texture> &getTexture() const override { return m_mesh.texture; }
    const std::shared_ptr<VertexArray> &getVertexArray() const override { return m_mesh.vertexArray; }
    const glm::mat4 &getModelWorld() const override { return m_modelWorld; }

    const Transform &getTransform() const { return m_transform; }

    void setAABB(const AABB &aabb) { m_aabb = aabb; };
    void setBoundingSphere(const BoundingSphere& bs) { m_boundingSphere = bs; }
    void setTranslate(const glm::vec3 &v) { m_transform.T = v; }
    void setRotate(const glm::vec3 &v) { m_transform.R = v; }
    void setScale(const glm::vec3 &v) { m_transform.S = v; }

    const BoundingSphere &getBoundingSphere() const { return m_boundingSphere; }
    BoundingSphere getTransformedBoundingSphere() const;
    AABB getAABB() const { return m_aabb.getRotated(m_transform.R); }

    void translate(const glm::vec3 &v) { m_transform.T += v; }
    void rotate(const glm::vec3 &v) { m_transform.R += v; }
    void scale(const glm::vec3 &v) { m_transform.S += v; };
    void updateModelWorld() { m_modelWorld = m_transform.getTransform(); }
};
