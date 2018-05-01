#pragma once
#include <vector>
#include <algorithm>
#include "BoundingSphere.hpp"
#include "BoundingBoxAA.hpp"
#include "Transform.hpp"
#include "renderer/Drawable.hpp"

class Model : public Drawable
{
    Transform m_transform;

    // Bounding Volumes
    BoundingSphere m_boundingSphere;
    AABB m_aabb;

    // Drawable
    std::string m_shader;
    std::string m_texture;
    std::string m_vertexArray;
    glm::mat4 m_modelWorld{};

  public:
    Model(const std::vector<glm::vec3> vertices, const std::string &shader, const std::string &texture, const std::string &vertexArray)
        : m_boundingSphere(m_transform, vertices), m_aabb(m_transform, vertices),
          m_shader(shader), m_texture(texture), m_vertexArray(vertexArray) { update(); }

    const Transform &getTransform() const { return m_transform; }
    // Drawable
    const std::string &getShader() const override { return m_shader; }
    const std::string &getTexture() const override { return m_texture; }
    const std::string &getVertexArray() const override { return m_vertexArray; }
    const glm::mat4 &getModelWorld() const override { return m_modelWorld; }

    void setTransform(const Transform &t) { m_transform = t; }
    void setTranslate(const glm::vec3 &v) { m_transform.T = v; }
    void setRotate(const glm::vec3 &v) { m_transform.R = v; }
    void setScale(const glm::vec3 &v) { m_transform.S = v; }

    const AABB &getAABB() const { return m_aabb; }
    const BoundingSphere &getBoundingSphere() const { return m_boundingSphere; }

    void translate(const glm::vec3 &v) { m_transform.T += v; }
    void rotate(const glm::vec3 &v) { m_transform.R += v; }
    void scale(const glm::vec3 &v) { m_transform.S += v; };

    void update()
    {
        m_modelWorld = m_transform.getMatrix();
        m_aabb.update();
        m_boundingSphere.update();
    }
};
