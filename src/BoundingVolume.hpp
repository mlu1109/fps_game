#pragma once
#include "Transform.hpp"
#include "renderer/Drawable.hpp"

class AABB;           // Forward declared
class BoundingSphere; // Forward declared

class BoundingVolume : public Drawable
{
  protected:
    const Transform &m_boundedTransform; // The transform of the enclosed shape

  public:
    BoundingVolume(const Transform &boundedTransform) : m_boundedTransform(boundedTransform) {}
    virtual bool isIntersecting(const BoundingVolume &other) const = 0;
    virtual bool isIntersecting(const BoundingSphere &other) const = 0;
    virtual bool isIntersecting(const AABB &other) const = 0;
};
