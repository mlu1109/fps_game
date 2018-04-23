#include <map>
#include "Utility.hpp"


// Vertex/Indices
// -------------------------------------------------- 

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

std::pair<std::vector<Vertex>, std::vector<GLuint>> getVertexDataFromOBJ(const OBJ &obj)
{
    // Because OBJ face elements and vertex/texcoord/normal triplets are not guaranteed to have a
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
            // OBJ have 1-based array indices.
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

    return std::make_pair(std::move(vertices), std::move(indices));
}


// VertexArray
// --------------------------------------------------

std::shared_ptr<VertexArray> newVertexArrayFromOBJ(const std::string &objPath)
{
    OBJ obj = loadOBJ(objPath);
    return newVertexArrayFromOBJ(obj);
}

std::shared_ptr<VertexArray> newVertexArrayFromOBJ(const OBJ &obj)
{
    auto vertexData = getVertexDataFromOBJ(obj);
    return std::make_shared<VertexArray>(std::move(vertexData.first), std::move(vertexData.second));
}


// Texture
// --------------------------------------------------

std::shared_ptr<Texture> newTextureFromTGA(const std::string &tgaPath)
{
    TGA tga = loadTGA(tgaPath);
    return std::make_shared<Texture>(tga.imageData, tga.bitsPerPixel == 32 ? GL_RGBA : GL_RGB, tga.width, tga.height);
}


// HeightMap
// --------------------------------------------------

HeightMap newHeightMapFromTGA(const std::string &tgaPath, const std::shared_ptr<Shader> &shader, const std::shared_ptr<Texture> &texture)
{
    TGA tga = loadTGA(tgaPath);
    return {tga.width, tga.height, tga.imageData, tga.bytesPerPixel, shader, texture};
}
