#pragma once

#include <glm/glm.hpp>
#include "Transform.hpp"
#include "loaders/OBJ.hpp"

class BoundingSphere
{
    glm::vec3 m_center;
    float m_radius;

  public:
    BoundingSphere() = default;
    BoundingSphere(const glm::vec3 &center, float radius) : m_center(center), m_radius(radius) {}
    BoundingSphere(const OBJ &);

    const glm::vec3 &getCenter() const { return m_center; }
    float getRadius() const { return m_radius; }
    
    const glm::vec3 getScale() const { return glm::vec3(m_radius, m_radius, m_radius); }
    BoundingSphere getTransformed(const glm::vec3 &T, const glm::vec3 &S) const;
    Transform getTransform() const { return Transform{getCenter(), {0, 0, 0}, getScale()}; }
};