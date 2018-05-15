#include <array>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include "BoundingBoxAA.hpp"
#include "BoundingSphere.hpp"

AABB::AABB(const glm::vec3 &min, const glm::vec3 &max)
    : m_min_0(min), m_max_0(max) {}

AABB::AABB(const std::vector<glm::vec3> &vertices)
{
    // Calculate extremes
    m_min_0 = vertices[0];
    m_max_0 = vertices[0];
    for (const auto &v : vertices)
        for (int i = 0; i < 3; ++i) // x, y, z
        {
            if (v[i] < m_min_0[i])
                m_min_0[i] = v[i];
            if (v[i] > m_max_0[i])
                m_max_0[i] = v[i];
        }
}

glm::vec3 AABB::getCenter() const
{
    return 0.5f * glm::vec3(m_max.x + m_min.x,
                            m_max.y + m_min.y,
                            m_max.z + m_min.z);
}

glm::vec3 AABB::getScale() const
{
    return 0.5f * glm::vec3(m_max.x - m_min.x,
                            m_max.y - m_min.y,
                            m_max.z - m_min.z);
}

void AABB::update(const Transform &t)
{
    // Rotate vertices according to bounded transform
    glm::mat3 rotMat = glm::rotate(glm::mat4(1.0f), t.R.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
                       glm::rotate(glm::mat4(1.0f), t.R.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
                       glm::rotate(glm::mat4(1.0f), t.R.z, glm::vec3(0.0f, 0.0f, 1.0f));
    std::array<glm::vec3, 8> vertices{
        // Bottom
        rotMat * glm::vec3{m_min_0.x, m_min_0.y, m_max_0.z},
        rotMat * glm::vec3{m_max_0.x, m_min_0.y, m_max_0.z},
        rotMat * glm::vec3{m_max_0.x, m_min_0.y, m_min_0.z},
        rotMat * glm::vec3{m_min_0.x, m_min_0.y, m_min_0.z},
        // Top
        rotMat * glm::vec3{m_min_0.x, m_max_0.y, m_max_0.z},
        rotMat * glm::vec3{m_max_0.x, m_max_0.y, m_max_0.z},
        rotMat * glm::vec3{m_max_0.x, m_max_0.y, m_min_0.z},
        rotMat * glm::vec3{m_min_0.x, m_max_0.y, m_min_0.z},
    };

    // Recalculate extremes using the rotated box
    glm::vec3 min = vertices[0];
    glm::vec3 max = vertices[0];
    for (const auto &v : vertices)
        for (int i = 0; i < 3; ++i) // x, y, z
        {
            if (v[i] < min[i])
                min[i] = v[i];
            if (v[i] > max[i])
                max[i] = v[i];
        }

    // Scale and translate according to bounded transform
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), t.T) *
                          glm::scale(glm::mat4(1.0f), t.S);
    m_min = transform * glm::vec4(min, 1.0);
    m_max = transform * glm::vec4(max, 1.0);

    m_modelWorld = glm::translate(glm::mat4(1.0f), getCenter()) * glm::scale(glm::mat4(1.0f), getScale());
}

bool AABB::isIntersecting(const BoundingVolume &bv) const
{
    return bv.isIntersecting(*this);
}

bool AABB::isIntersecting(const AABB &other) const
{
    const glm::vec3 &a_max = m_max;
    const glm::vec3 &a_min = m_min;
    const glm::vec3 &b_max = other.getMax();
    const glm::vec3 &b_min = other.getMin();

    return !(a_max.x < b_min.x || b_max.x < a_min.x ||
             a_max.y < b_min.y || b_max.y < a_min.y ||
             a_max.z < b_min.z || b_max.z < a_min.z);
}

bool AABB::isIntersecting(const BoundingSphere &other) const
{
    const glm::vec3 &a_max = m_max;
    const glm::vec3 &a_min = m_min;
    const glm::vec3 &b_center = other.getCenter();

    // Closest vertex of the AABB to the center of the sphere
    glm::vec3 a_closest;

    for (int i = 0; i < 3; ++i) // x, y, z
    {
        if (a_min[i] > b_center[i])
            a_closest[i] = a_min[i];
        else if (a_max[i] < b_center[i])
            a_closest[i] = a_max[i];
        else
            a_closest[i] = b_center[i];
    }

    float distance = glm::distance(a_closest, b_center);
    float radius = other.getRadius();

    return distance <= radius;
}

bool AABB::isIntersecting(const Ray &) const
{
    return false;
}

bool AABB::hasPoint(const glm::vec3 &point) const
{
    return !(m_max.x < point.x || point.x < m_min.x ||
             m_max.y < point.y || point.y < m_min.y ||
             m_max.z < point.z || point.z < m_min.z);
}

glm::vec3 AABB::getMTV(const AABB &other) const
{
    const glm::vec3 &a_max = m_max;
    const glm::vec3 &a_min = m_min;
    const glm::vec3 &b_max = other.getMax();
    const glm::vec3 &b_min = other.getMin();

    float x = 0.02f;
    float y = 0.02f;
    float z = 0.02f;

    if (a_min.x < b_max.x && a_max.x > b_max.x)
        x += std::min(b_max.x - a_min.x, a_max.x - b_max.x);
    else
        x += std::min(a_max.x - b_min.x, b_min.x - a_min.x);

    if (a_min.y < b_max.y && a_max.y > b_max.y)
        y += std::min(b_max.y - a_min.y, a_max.y - b_max.y);
    else
        y += std::min(a_max.y - b_min.y, b_min.y - a_min.y);

    if (a_min.z < b_max.z && a_max.z > b_max.z)
        z += std::min(b_max.z - a_min.z, a_max.z - b_max.z);
    else
        z += std::min(a_max.z - b_min.z, b_min.z - a_min.z);
    
    if (x < y)
        if (x < z)
            return glm::vec3(x, 0, 0);
        else // z <= x
            return glm::vec3(0, 0, z);
    else // y >= x
        if (y < z)
            return glm::vec3(0, y, 0);
        else // z <= y
            return glm::vec3(0, 0, z);
}

glm::vec3 AABB::getMTV(const BoundingSphere &other) const
{
    const glm::vec3 &a_max = m_max;
    const glm::vec3 &a_min = m_min;
    const glm::vec3 &b_center = other.getCenter();

    // Closest vertex of the AABB to the center of the sphere
    glm::vec3 a_closest;

    for (int i = 0; i < 3; ++i) // x, y, z
    {
        if (a_min[i] > b_center[i])
            a_closest[i] = a_min[i];
        else if (a_max[i] < b_center[i])
            a_closest[i] = a_max[i];
        else
            a_closest[i] = b_center[i];
    }

    float distance = glm::distance(a_closest, b_center);
    if (a_closest == b_center)
        return glm::vec3(0, 0, 0);
        
    glm::vec3 mtv = glm::normalize(a_closest - b_center);

    return mtv * (other.getRadius() - distance);
}