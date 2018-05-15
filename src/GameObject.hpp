#pragma once

#include "renderer/OBJ.hpp"
#include "BoundingBoxAA.hpp"
#include "BoundingSphere.hpp"
#include "Model.hpp"
#include "Transform.hpp"

class GameObject
{
    Model m_model;
    BoundingSphere m_boundingSphere;
    AABB m_aabb;
    std::vector<AABB> m_hitboxes;

  protected:
    Transform m_transform;

  public:
    GameObject(const Model &, const OBJ &);
    GameObject(const Model &);

    void setTransform(const Transform &t) { m_transform = t; }
    void setTranslate(const glm::vec3 &v) { m_transform.T = v; }
    void setRotate(const glm::vec3 &v) { m_transform.R = v; }
    void setScale(const glm::vec3 &v) { m_transform.S = v; }

    void setY(float y) { m_transform.T.y = y; }

    const AABB &getAABB() const { return m_aabb; }
    const BoundingSphere &getBoundingSphere() const { return m_boundingSphere; }
    const Model &getModel() const { return m_model; }
    const Transform &getTransform() const { return m_transform; }
    const std::vector<AABB> &getHitboxes() const { return m_hitboxes; }

    void translate(const glm::vec3 &v) { m_transform.T += v; }
    void rotate(const glm::vec3 &v) { m_transform.R += v; }
    void scale(const glm::vec3 &v) { m_transform.S += v; };

    void update();
};
