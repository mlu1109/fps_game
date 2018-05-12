#pragma once

#include <unordered_map>
#include <vector>
#include "renderer/OBJ.hpp"
#include "renderer/TGA.hpp"
#include "renderer/Shader.hpp"
#include "renderer/Texture.hpp"
#include "renderer/Cubemap.hpp"
#include "renderer/VertexArray.hpp"
#include "GameObject.hpp"
#include "Skybox.hpp"

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

    GameObject loadObject(const std::string &objPath, int meshIdx = 0);
    Skybox loadSkybox(const std::string &objPath, const std::string &cubemapPath, const std::string &shaderName);
};
