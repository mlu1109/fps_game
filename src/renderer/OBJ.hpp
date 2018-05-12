#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Vertex.hpp"
#include "external/tiny_obj_loader.h"

struct Material
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shine;
};

struct Mesh
{
    std::string path;
    std::string name;
    std::vector<GLuint> indices;
    std::vector<Vertex> vertices;
    std::vector<Material> materials;
    std::vector<int> indexMaterialId;
};

class OBJ
{
    std::string m_path;
    std::vector<Mesh> m_meshes;

  public:
    OBJ(const std::string &path);

    const std::string &getPath() const { return m_path; }
    const std::vector<Mesh> &getMeshes() const { return m_meshes; }
};
