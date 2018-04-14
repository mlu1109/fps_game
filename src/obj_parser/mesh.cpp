#include <iostream>
#include <map>
#include <set>
#include "mesh.hpp"

bool operator<(const Vertex &lhs, const Vertex &rhs)
{
    if (lhs.position != rhs.position)
        return lhs.position < rhs.position;
    if (lhs.normal != rhs.normal)
        return lhs.normal < rhs.normal;
    if (lhs.texCoord != rhs.texCoord)
        return lhs.texCoord < rhs.texCoord;
    if (lhs.color != rhs.color)
        return lhs.color < rhs.color;

    return false; // equal
}

/* Only supports three element faces at this time */

Mesh getMesh(const WavefrontObject &obj)
{
    // Because .obj face elements and vertex/texcoord/normal triplets are not guaranteed to have a
    // one-to-one mapping new vertex/texcoord/normal triplets might need to be generated.

    std::map<Vertex, GLuint> vertexToIndex;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    for (const auto &face : obj.f)
    {
        if (face.size() != 3)
            throw std::runtime_error("Can not parse non-triangular face.");

        for (const auto &f : face)
        {
            // .obj files have 1-based array indices.
            int vIdx = f.v - 1;
            int vnIdx = f.vn - 1;
            int vtIdx = f.vt - 1;
            glm::vec3 v = obj.v[vIdx];
            glm::vec3 vn{0};
            if (vnIdx != -1)
                vn = obj.vn[vnIdx];
            glm::vec2 vt{0};
            if (vtIdx != -1)
                vt = obj.vt[vtIdx];

            Vertex vertex{
                {v[0], v[1], v[2]},
                {vn[0], vn[1], vn[2]},
                {0, 0, 0, 0}, // No colors
                {vt[0], vt[1]}};

            if (!vertexToIndex.count(vertex)) // New vertex
            {
                vertexToIndex[vertex] = vertices.size();
                indices.push_back(vertices.size());
                vertices.push_back(vertex);
            }
            else
            {
                indices.push_back(vertexToIndex[vertex]);
            }
        }
    }

    return {vertices, indices};
}