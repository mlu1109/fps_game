#include "Heightmap.hpp"

Heightmap::Heightmap(int wdth, int hght,
                     const std::vector<unsigned char> &data, int stride)
    : width(wdth), height(hght)
{
    quadWidth = 3;
    quadHeight = 3;
    int scaleHeight = 5;
    int vertexCount = width * height;
    int triangleCount = (width - 1) * (height - 1) * 2; // quads * 2
    std::vector<Vertex> _vertices(vertexCount);
    std::vector<GLuint> _indices(triangleCount * 3);

    // _vertices and texCoords
    for (int x = 0; x < width; ++x)
    {
        for (int z = 0; z < height; ++z)
        {
            Vertex &v = _vertices[x + z * width];
            auto y = data[(x + z * width) * stride];
            v.position = {
                static_cast<float>(x) * quadWidth,
                static_cast<float>(y) / scaleHeight,
                static_cast<float>(z) * quadHeight};
            v.texCoord = {
                static_cast<float>(x),
                static_cast<float>(z)};
        }
    }

    // Normals
    for (int x = 0; x < width; ++x)
    {
        for (int z = 0; z < height; ++z)
        {
            glm::vec3 v1;
            glm::vec3 v2;
            glm::vec3 v3;

            // Handle edge cases
            if (x == 0)
                v1 = _vertices[(x - 0) + z * width].position;
            else
                v1 = _vertices[(x - 1) + z * width].position;

            if (z == height - 1)
                v2 = _vertices[x + (z + 0) * width].position;
            else
                v2 = _vertices[x + (z + 1) * width].position;

            if (z == 0)
                if (x == width - 1)
                    v3 = _vertices[(x + 0) + (z - 0) * width].position;
                else
                    v3 = _vertices[(x + 1) + (z - 0) * width].position;
            else if (x == width - 1)
                v3 = _vertices[(x + 0) + (z - 1) * width].position;
            else
                v3 = _vertices[(x + 1) + (z - 1) * width].position;

            // The vertex normal is the normal formed by its surrounding neighbours
            glm::vec3 e1 = v1 - v3;
            glm::vec3 e2 = v1 - v2;
            glm::vec3 n = glm::cross(e1, e2);
            if (n.y < 0) // Flip the normal if it points downward
                n *= -1;

            _vertices[x + z * width].normal = glm::normalize(n);
        }
    }

    // _indices
    for (int x = 0; x < width - 1; ++x)
    {
        for (int z = 0; z < height - 1; ++z)
        {
            // Triangle 1
            _indices[(x + z * (width - 1)) * 6 + 0] = x + 0 + (z + 0) * width;
            _indices[(x + z * (width - 1)) * 6 + 1] = x + 0 + (z + 1) * width;
            _indices[(x + z * (width - 1)) * 6 + 2] = x + 1 + (z + 0) * width;
            // Triangle 2
            _indices[(x + z * (width - 1)) * 6 + 3] = x + 1 + (z + 0) * width;
            _indices[(x + z * (width - 1)) * 6 + 4] = x + 0 + (z + 1) * width;
            _indices[(x + z * (width - 1)) * 6 + 5] = x + 1 + (z + 1) * width;
        }
    }
    vertices = std::move(_vertices);
    indices = std::move(_indices);
}

Heightmap::Heightmap(const TGA &tga)
    : Heightmap(tga.width, tga.height, tga.imageData, tga.bytesPerPixel)
{
}