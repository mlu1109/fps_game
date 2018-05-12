#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "BoundingVolume.hpp"
#include <string>

class AABB : public BoundingVolume
{
    // The bounding box without any transformations applied
    glm::vec3 m_min_0;
    glm::vec3 m_max_0;
    // The bounding box with the bounded shape's transformations applied
    glm::vec3 m_min;
    glm::vec3 m_max;

  public:
    AABB( const glm::vec3 &min, const glm::vec3 &max);
    AABB( const std::vector<glm::vec3> &vertices);

    const glm::vec3 &getMin() const { return m_min; }
    const glm::vec3 &getMax() const { return m_max; }

    void update(const Transform &);

    glm::vec3 getCenter() const;
    glm::vec3 getScale() const;

    // BoundingVolume
    bool isIntersecting(const BoundingVolume &) const override;
    bool isIntersecting(const AABB &) const override;
    bool isIntersecting(const BoundingSphere &) const override;
    bool isIntersecting(const Ray &) const override;
    bool hasPoint(const glm::vec3 &) const;
};
