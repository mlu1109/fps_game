#include <array>
#include <glm/gtx/euler_angles.hpp>
#include "AABB.hpp"
#include "utility.hpp"

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

AABB AABB::getTransformed(const glm::vec3 &T, const glm::vec3 &R, const glm::vec3 &S) const
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

    glm::mat3 rotMat = glm::orientate3(R);
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
    
    glm::mat4 transform = glm::translate(T) * glm::scale(S);
    min = transform * glm::vec4(min, 1.0);
    max = transform * glm::vec4(max, 1.0);
    return AABB(min, max);
}
