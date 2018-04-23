#include "Heightmap.hpp"

HeightMap::HeightMap(int width, int height, const std::vector<unsigned char> &data, int stride, const std::shared_ptr<Shader> &shader, const std::shared_ptr<Texture> &texture)
    : m_width{width}, m_height{height}
{
    int quadWidth = 2;
    int quadHeight = 2;
    int scaleHeight = 5;
    int vertexCount = width * height;
    int triangleCount = (width - 1) * (height - 1) * 2; // quads * 2
    std::vector<Vertex> vertices(vertexCount);
    std::vector<GLuint> indices(triangleCount * 3);

    // Vertices and texCoords
    for (int x = 0; x < width; ++x)
    {
        for (int z = 0; z < height; ++z)
        {
            Vertex &v = vertices[x + z * width];
            auto y = data[(x + z * width) * stride];
            v.position = {
                static_cast<float>(x) * quadWidth,
                static_cast<float>(y) / scaleHeight,
                static_cast<float>(z) * quadHeight};
            v.texCoord = {
                static_cast<float>(x),
                static_cast<float>(z)};
            v.color = {static_cast<GLubyte>(x), y, static_cast<GLubyte>(z), 255};
        }
    }

    // Normals
    for (int x = 0; x < width; ++x)
    {
        for (int z = 0; z < height; ++z)
        {
            std::array<GLfloat, 3> v1;
            std::array<GLfloat, 3> v2;
            std::array<GLfloat, 3> v3;

            // Handle edge cases
            if (x == 0)
                v1 = vertices[(x - 0) + z * width].position;
            else
                v1 = vertices[(x - 1) + z * width].position;

            if (z == height - 1)
                v2 = vertices[x + (z + 0) * width].position;
            else
                v2 = vertices[x + (z + 1) * width].position;

            if (z == 0)
                if (x == width - 1)
                    v3 = vertices[(x + 0) + (z - 0) * width].position;
                else
                    v3 = vertices[(x + 1) + (z - 0) * width].position;
            else if (x == width - 1)
                v3 = vertices[(x + 0) + (z - 1) * width].position;
            else
                v3 = vertices[(x + 1) + (z - 1) * width].position;

            // The vertex normal is the normal formed by its surrounding neighbours
            glm::vec3 e1 = {v1[0] - v3[0], v1[1] - v3[1], v1[2] - v3[2]};
            glm::vec3 e2 = {v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]};
            glm::vec3 n = glm::cross(e1, e2);
            if (n.y < 0) // Flip the normal if it points downward
                n *= -1;

            n = glm::normalize(n);
            vertices[x + z * width].normal = {n[0], n[1], n[2]};
        }
    }

    // Indices
    for (int x = 0; x < width - 1; ++x)
    {
        for (int z = 0; z < height - 1; ++z)
        {
            // Triangle 1
            indices[(x + z * (width - 1)) * 6 + 0] = x + 0 + (z + 0) * width;
            indices[(x + z * (width - 1)) * 6 + 1] = x + 0 + (z + 1) * width;
            indices[(x + z * (width - 1)) * 6 + 2] = x + 1 + (z + 0) * width;
            // Triangle 2
            indices[(x + z * (width - 1)) * 6 + 3] = x + 1 + (z + 0) * width;
            indices[(x + z * (width - 1)) * 6 + 4] = x + 0 + (z + 1) * width;
            indices[(x + z * (width - 1)) * 6 + 5] = x + 1 + (z + 1) * width;
        }
    }

    auto vertexArray = std::make_shared<VertexArray>(vertices, indices);
    m_mesh = {shader, texture, vertexArray};
}