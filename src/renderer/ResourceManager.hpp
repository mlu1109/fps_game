#pragma once

#include <unordered_map>
#include <vector>
#include "OBJ.hpp"
#include "TGA.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Cubemap.hpp"
#include "VertexArray.hpp"

class ResourceManager
{
    std::unordered_map<std::string, Cubemap> m_Cubemaps;
    std::unordered_map<std::string, Shader> m_shaders;
    std::unordered_map<std::string, Texture> m_textures;
    std::unordered_map<std::string, VertexArray> m_vertexArrays;

  public:
    ~ResourceManager();

    const Shader *loadShader(const std::string &name);
    const Texture *loadTexture(const std::string &tgaPath);
    const Cubemap *loadCubemap(const std::string &tgaPath);
    const VertexArray *loadVertexArray(const Mesh &mesh);
};
