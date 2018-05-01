#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "BoundingVolume.hpp"
#include "Transform.hpp"

class BoundingSphere : public BoundingVolume
{
    // The bounding sphere without any transformations applied
    glm::vec3 m_center_0;
    float m_radius_0;
    // The bounding sphere with the bounded shape's transformations applied
    glm::vec3 m_center;
    float m_radius;
    // Drawable
    std::string m_shader = "color_solid";
    std::string m_texture = "";
    std::string m_vertexArray = "sphere.obj";
    glm::mat4 m_modelWorld;
  
  public:
    BoundingSphere(const Transform &boundedTransform, const glm::vec3 &center, float radius);
    BoundingSphere(const Transform &boundedTransform, const std::vector<glm::vec3> &vertices);

    const glm::vec3 &getCenter() const { return m_center; };
    float getRadius() const { return m_radius; }
    // Drawable
    const std::string &getShader() const { return m_shader; }
    const std::string &getTexture() const { return m_texture; }
    const std::string &getVertexArray() const { return m_vertexArray; }
    const glm::mat4 &getModelWorld() const { return m_modelWorld; }

    void update();

    // BoundingVolume interface
    bool isIntersecting(const BoundingVolume &) const override;
    bool isIntersecting(const AABB &) const override;
    bool isIntersecting(const BoundingSphere &) const override;
    bool isIntersecting(const Ray&) const override;
    bool hasPoint(const glm::vec3 &) const override;

};
