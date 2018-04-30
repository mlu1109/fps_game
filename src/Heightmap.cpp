#include "Heightmap.hpp"

Heightmap::Heightmap(int width, int height,
                     const std::vector<unsigned char> &data, int stride,
                     const std::string &shader,
                     const std::string &texture)
    : m_width(width), m_height(height), m_shader(shader), m_texture(texture)
{
    m_quadWidth = 4;
    m_quadHeight = 4;
    int quadWidth = m_quadWidth;
    int quadHeight = m_quadHeight;
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
    m_vertices = std::move(vertices);
    m_indices = std::move(indices);
}

Heightmap::Heightmap(const TGA &tga, const std::string &shader, const std::string &texture)
    : Heightmap(tga.width, tga.height, tga.imageData, tga.bytesPerPixel, shader, texture)
{
}

float Heightmap::getY(float x, float z) const
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