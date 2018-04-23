#pragma once

#include <glm/glm.hpp>
#include "loaders/OBJ.hpp"
#include "BoundingVolume.hpp"
#include "Transform.hpp"


class AABB : public BoundingVolume
{
    glm::vec3 m_min;
    glm::vec3 m_max;

  public:
    AABB() = default;
    AABB(const glm::vec3 &min, const glm::vec3 &max);
    AABB(const OBJ &);

    const glm::vec3 &getMin() const { return m_min; }
    const glm::vec3 &getMax() const { return m_max; }
    
    glm::vec3 getCenter() const;
    glm::vec3 getScale() const;
    Transform getTransform() const;
    AABB getTransformed(const Transform &) const;
    // BoundingVolume interface
    bool isIntersecting(const BoundingVolume&) const override;
    bool isIntersecting(const AABB&) const override;
    bool isIntersecting(const BoundingSphere&) const override;

};
