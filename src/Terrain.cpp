#include "Terrain.hpp"

Terrain::Terrain(const Heightmap &hm, const VertexArray *va)
    : m_width(hm.width), m_height(hm.height),
      m_quadWidth(hm.quadWidth), m_quadHeight(hm.quadHeight),
      m_vertexArray(va),
      m_vertices(hm.vertices)
{
}

float Terrain::getY(float x, float z) const
{
    // Quad coordinates
    int q_x = static_cast<int>(x) / m_quadWidth;
    int q_z = static_cast<int>(z) / m_quadHeight;

    // Outside
    if (q_x >= m_width - 1 || q_z >= m_height - 1 || x < 0 || z < 0)
        return 0;

    const auto &p1 = m_vertices[(q_x + 0) + (q_z + 0) * m_width].position;
    const auto &p2 = m_vertices[(q_x + 1) + (q_z + 1) * m_width].position;
    glm::vec3 v1 = {p1[0], p1[1], p1[2]}; // Quad top left position
    glm::vec3 v2 = {p2[0], p2[1], p2[2]}; // Quad bottom right position

    // Pick triangle
    int q_px = static_cast<int>(x) % m_quadWidth;
    int q_pz = static_cast<int>(z) % m_quadHeight;
    int i = q_px > q_pz ? 1 : 0;
    const auto &p3 = i == 1
                         ? m_vertices[(q_x + 1) + (q_z + 0) * m_width].position  // Quad top right corner
                         : m_vertices[(q_x + 0) + (q_z + 1) * m_width].position; // Quad bottom left corner
    glm::vec3 v3 = {p3[0], p3[1], p3[2]};

    // Calculate normal
    glm::vec3 e1 = v1 - v2;
    glm::vec3 e2 = v1 - v3;
    glm::vec3 n = glm::normalize(glm::cross(e1, e2));
    if (n.y < 0) // If normal points downward, flip
        n *= -1;

    // Use the plain equation to find y
    return (n.x * (v1.x - x) + n.y * v1.y + n.z * (v1.z - z)) / n.y;
}