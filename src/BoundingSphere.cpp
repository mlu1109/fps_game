#include <algorithm>
#include <glm/gtx/norm.hpp>
#include "BoundingSphere.hpp"
#include "utility.hpp"

/*
 * Ritter's bounding sphere. Reference:
 * https://www.researchgate.net/publication/242453691_An_Efficient_Bounding_Sphere
 */

BoundingSphere getBoundingSphere(const OBJ &obj)
{
    // Get vertices from model
    auto vertexData = getVertexDataFromOBJ(obj);
    std::vector<glm::vec3> vertices;
    for (const auto &v : vertexData.first)
        vertices.push_back(glm::vec3{v.position[0], v.position[1], v.position[2]});

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
            if (glm::distance2(a, b) < glm::distance2(_a, _b))
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

    return BoundingSphere{center, radius};
}