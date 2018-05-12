#pragma once

#include "renderer/OBJ.hpp"
#include "BoundingBoxAA.hpp"
#include "BoundingSphere.hpp"
#include "Model.hpp"
#include "Transform.hpp"

class GameObject
{

    Model m_model;
    Transform m_transform;
//    BoundingSphere m_boundingSphere;
//    AABB m_aabb;

  public:
    GameObject(const Model &);

    void setTransform(const Transform &t) { m_transform = t; }
    void setTranslate(const glm::vec3 &v) { m_transform.T = v; }
    void setRotate(const glm::vec3 &v) { m_transform.R = v; }
    void setScale(const glm::vec3 &v) { m_transform.S = v; }

//    const AABB &getAABB() const { return m_aabb; }
//    const BoundingSphere &getBoundingSphere() const { return m_boundingSphere; }
    const Model &getModel() const { return m_model; }

    void translate(const glm::vec3 &v) { m_transform.T += v; }
    void rotate(const glm::vec3 &v) { m_transform.R += v; }
    void scale(const glm::vec3 &v) { m_transform.S += v; };

    void update();
};