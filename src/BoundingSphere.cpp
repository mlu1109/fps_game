#include <vector>
#include <array>
#include "BoundingBoxAA.hpp"
#include "BoundingSphere.hpp"

BoundingSphere::BoundingSphere(const Transform &boundedTransform, const glm::vec3 &center, float radius)
    : BoundingVolume(boundedTransform), m_center_0(center), m_radius_0(radius) {}

// Ritter's Bounding Sphere
// Reference: https://www.researchgate.net/publication/242453691_An_Efficient_Bounding_Sphere
BoundingSphere::BoundingSphere(const Transform &boundedTransform, const std::vector<glm::vec3> &vertices)
    : BoundingVolume(boundedTransform)
{
    // Find six points where min_x has the smallest x-component, min_y has the smallest y-component and so forth
    std::array<glm::vec3, 6> points; // [min_x, min_y, min_z, max_x, max_y, max_z]
    std::fill(points.begin(), points.end(), vertices[0]);
    for (const auto &v : vertices)
        for (int i = 0; i < 3; ++i) // 0: x, 1: y, 2: z
        {
            if (points[i][i] > v[i])
                points[i] = v;
            if (points[i + 3][i] < v[i])
                points[i + 3] = v;
        }

    // Pick the pair that has the maximum point-to-point separation
    glm::vec3 a = points[0];
    glm::vec3 b = points[0];
    for (size_t i = 0; i < points.size(); ++i)
    {
        glm::vec3 _a = points[i];
        for (size_t j = i + 1; j < points.size(); ++j)
        {
            glm::vec3 _b = points[j];
            if (glm::distance(a, b) < glm::distance(_a, _b))
            {
                a = _a;
                b = _b;
            }
        }
    }

    glm::vec3 center = (a + b) / 2.f;
    float radius = glm::distance(center, a);
    // Check if all points fall into the ball.
    for (const auto &v : vertices)
    {
        glm::vec3 direction = v - center;
        float _radius = glm::length(direction);
        if (_radius <= radius)
            continue;

        // Point does not fall into the ball, update
        radius = (radius + _radius) * 0.5f;
        center = (radius * center + (_radius - radius) * v) / _radius;
    }

    m_center_0 = center;
    m_radius_0 = radius;
}

void BoundingSphere::update()
{
    const Transform &t = *m_boundedTransform;
    float maxScale = std::max(t.S.x, std::max(t.S.y, t.S.z));
    m_radius = m_radius_0 * maxScale;
    m_center = m_center_0 + t.T;
    m_modelWorld = glm::translate(glm::mat4(1.0f), m_center) *
                   glm::scale(glm::mat4(1.0f), glm::vec3(m_radius, m_radius, m_radius));
}

bool BoundingSphere::isIntersecting(const BoundingVolume &other) const
{
    return other.isIntersecting(*this);
}

bool BoundingSphere::isIntersecting(const AABB &other) const
{
    return other.isIntersecting(*this);
}

bool BoundingSphere::isIntersecting(const BoundingSphere &other) const
{
    float radius = m_radius + other.m_radius;
    return glm::distance(m_center, other.m_center) < radius;
}

bool BoundingSphere::isIntersecting(const Ray &ray) const
{
    float t = glm::distance(m_center, ray.origin);
    glm::vec3 d = ray.direction * t;
    return glm::distance(ray.origin + d, m_center) < m_radius;
}

bool BoundingSphere::hasPoint(const glm::vec3 &point) const
{
    float radius2 = m_radius;
    return glm::distance(point, m_center) < radius2;
}