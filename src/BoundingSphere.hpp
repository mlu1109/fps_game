#pragma once

#include <glm/glm.hpp>
#include "BoundingVolume.hpp"
#include "Transform.hpp"
#include "loaders/OBJ.hpp"


class BoundingSphere : public BoundingVolume
{
    glm::vec3 m_center;
    float m_radius;

  public:
    BoundingSphere() = default;
    BoundingSphere(const glm::vec3 &center, float radius);
    BoundingSphere(const OBJ &);

    const glm::vec3 &getCenter() const { return m_center; }
    float getRadius() const { return m_radius; }

    glm::vec3 getScale() const;
    BoundingSphere getTransformed(const Transform &) const;
    Transform getTransform() const;
    // BoundingVolume interface
    bool isIntersecting(const BoundingVolume &) const override;
    bool isIntersecting(const AABB &) const override;
    bool isIntersecting(const BoundingSphere &) const override;
};
