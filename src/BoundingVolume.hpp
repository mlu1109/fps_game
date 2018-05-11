#pragma once
#include "Transform.hpp"
#include "Ray.hpp"

class AABB;           // Forward declared
class BoundingSphere; // Forward declared

class BoundingVolume
{
  protected:
    const Transform *m_boundedTransform; // The transform of the enclosed shape

  public:
    BoundingVolume(const Transform &boundedTransform) : m_boundedTransform(&boundedTransform) {}
    
    virtual bool isIntersecting(const BoundingVolume &) const = 0;
    virtual bool isIntersecting(const BoundingSphere &) const = 0;
    virtual bool isIntersecting(const AABB &) const = 0;
    virtual bool isIntersecting(const Ray &) const = 0;
    virtual bool hasPoint(const glm::vec3 &) const = 0;
};
