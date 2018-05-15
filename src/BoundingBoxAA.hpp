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

    glm::mat4 m_modelWorld;

  public:
    AABB(const glm::vec3 &min, const glm::vec3 &max);
    AABB(const std::vector<glm::vec3> &vertices);
    AABB() = default;

    const glm::vec3 &getMin() const { return m_min; }
    const glm::vec3 &getMax() const { return m_max; }
    const glm::mat4 &getModelWorld() const { return m_modelWorld; }

    void update(const Transform &);

    glm::vec3 getCenter() const;
    glm::vec3 getScale() const;

    // BoundingVolume
    bool isIntersecting(const BoundingVolume &) const override;
    bool isIntersecting(const AABB &) const override;
    bool isIntersecting(const BoundingSphere &) const override;
    bool isIntersecting(const Ray &) const override;
    glm::vec3 getMTV(const AABB &) const;
    glm::vec3 getMTV(const BoundingSphere &) const;


    bool hasPoint(const glm::vec3 &) const;
};
