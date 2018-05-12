#include <iostream>
#include <map>
#include "OBJ.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "external/tiny_obj_loader.h"
#include "../glm/print.hpp"

OBJ::OBJ(const std::string &path)
{
    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;
    std::string baseDir = path.substr(0, path.find_last_of('/') + 1);
    bool ok = tinyobj::LoadObj(&attributes, &shapes, &materials, &err, path.c_str(), baseDir.c_str());
    if (err.size())
    {
        std::cout << err << '\n';
    }
    if (!ok)
    {
        std::cout << "Failed to load " << path << '\n';
        exit(1);
    }

    for (const auto &shape : shapes)
    {
        const auto &objMesh = shape.mesh;
        Mesh mesh;
        mesh.path = path;
        mesh.name = shape.name;

        for (const auto &shapeMaterial : materials)
        {
            glm::vec3 ambient{shapeMaterial.ambient[0], shapeMaterial.ambient[1], shapeMaterial.ambient[2]};
            glm::vec3 diffuse{shapeMaterial.diffuse[0], shapeMaterial.diffuse[1], shapeMaterial.diffuse[2]};
            glm::vec3 specular{shapeMaterial.specular[0], shapeMaterial.specular[1], shapeMaterial.specular[2]};
            float shine = shapeMaterial.shininess;
            mesh.materials.push_back(Material{ambient, diffuse, specular, shine});
        }

        // Find unique vertices. Map indices to vertices
        std::map<Vertex, GLuint> vertexToIndex;
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        // Loop over faces
        size_t idxOffset = 0;
        size_t idxFace = 0;
        for (auto fv : objMesh.num_face_vertices)
        {
            // Loop over vertices in face f
            for (size_t v = 0; v < fv; ++v)
            {
                auto idx = idxOffset + v;
                auto vIdx = objMesh.indices[idx].vertex_index;
                auto vnIdx = objMesh.indices[idx].normal_index;
                auto vtIdx = objMesh.indices[idx].texcoord_index;
                const auto &aVertices = attributes.vertices;
                const auto &aNormals = attributes.normals;
                const auto &aTexCoords = attributes.texcoords;

                glm::vec3 position{
                    aVertices[vIdx * 3 + 0],
                    aVertices[vIdx * 3 + 1],
                    aVertices[vIdx * 3 + 2]};

                glm::vec3 normal{0.0f, 0.0f, 0.0f};
                if (vnIdx != -1)
                    normal = {
                        aNormals[vnIdx * 3 + 0],
                        aNormals[vnIdx * 3 + 1],
                        aNormals[vnIdx * 3 + 2]};

                glm::vec2 texCoord{0.0f, 0.0f};
                if (vtIdx != -1)
                    texCoord = {
                        aTexCoords[vtIdx * 2 + 0],
                        aTexCoords[vtIdx * 2 + 1]};

                Vertex vertex{position, normal, texCoord};
                if (vertexToIndex.count(vertex) == 0)
                {
                    vertexToIndex[vertex] = vertices.size();
                    vertices.push_back(vertex);
                }

                indices.push_back(vertexToIndex[vertex]);
                mesh.indexMaterialId.push_back(objMesh.material_ids[idxFace]);
            }
            idxFace += 1;
            idxOffset += fv;
        }

        mesh.vertices = vertices;
        mesh.indices = indices;
        m_meshes.push_back(mesh);
    }
}
