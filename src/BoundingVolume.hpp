#pragma once


class AABB;           // Forward declared
class BoundingSphere; // Forward declared

class BoundingVolume
{
  public:
    virtual bool isIntersecting(const BoundingVolume &other) const = 0;
    virtual bool isIntersecting(const BoundingSphere &other) const = 0;
    virtual bool isIntersecting(const AABB &other) const = 0;
};
