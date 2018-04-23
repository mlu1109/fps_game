#include <array>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#include "BoundingBoxAA.hpp"
#include "BoundingSphere.hpp"
#include "Utility.hpp"

AABB::AABB(const glm::vec3 &min, const glm::vec3 &max)
    : m_min(min), m_max(max) {}

AABB::AABB(const OBJ &obj)
{
    // Get vertices from model
    auto vertexData = getVertexDataFromOBJ(obj);
    std::vector<glm::vec3> vertices;
    for (const auto &v : vertexData.first)
        vertices.push_back(glm::vec3{v.position[0], v.position[1], v.position[2]});

    // Calculate extremes
    m_min = vertices[0];
    m_max = vertices[0];
    for (const auto &v : vertices)
        for (int i = 0; i < 3; ++i) // x, y, z
        {
            if (v[i] < m_min[i])
                m_min[i] = v[i];
            if (v[i] > m_max[i])
                m_max[i] = v[i];
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

Transform AABB::getTransform() const
{
    return Transform{getCenter(),
                     {0, 0, 0},
                     getScale()};
}

AABB AABB::getTransformed(const Transform &t) const
{
    std::array<glm::vec3, 8> vertices{
        // Bottom
        glm::vec3{m_min.x, m_min.y, m_max.z},
        glm::vec3{m_max.x, m_min.y, m_max.z},
        glm::vec3{m_max.x, m_min.y, m_min.z},
        glm::vec3{m_min.x, m_min.y, m_min.z},
        // Top
        glm::vec3{m_min.x, m_max.y, m_max.z},
        glm::vec3{m_max.x, m_max.y, m_max.z},
        glm::vec3{m_max.x, m_max.y, m_min.z},
        glm::vec3{m_min.x, m_max.y, m_min.z},
    };

    glm::mat3 rotMat = glm::orientate3(t.R);
    for (auto &v : vertices)
        v = rotMat * v;

    // Recalculate extremes
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

    glm::mat4 transform = glm::translate(t.T) * glm::scale(t.S);
    min = transform * glm::vec4(min, 1.0);
    max = transform * glm::vec4(max, 1.0);

    return AABB(min, max);
}

bool AABB::isIntersecting(const BoundingVolume &bv) const
{
    return bv.isIntersecting(*this);
}

bool AABB::isIntersecting(const AABB &other) const
{
    const glm::vec3 &a_max = m_max;
    const glm::vec3 &a_min = m_min;
    const glm::vec3 &b_max = other.getMin();
    const glm::vec3 &b_min = other.getMax();

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
    
    float distance2 = glm::distance2(a_closest, b_center);
    float radius2 = other.getRadius() * other.getRadius();
    
    return distance2 <= radius2;
}
