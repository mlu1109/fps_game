#pragma once

#include <unordered_map>
#include <vector>
#include "OBJ.hpp"
#include "TGA.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "TextureCubemap.hpp"
#include "VertexArray.hpp"

class ResourceManager
{
    std::unordered_map<std::string, Shader> m_shaders;
    std::unordered_map<std::string, Texture> m_textures;
    std::unordered_map<std::string, TextureCubemap> m_textureCubemaps;
    std::unordered_map<std::string, VertexArray> m_vertexArrays;

  public:
    ~ResourceManager();
    
    const Shader &getShader(const std::string &name) { return m_shaders.at(name); }
    const Texture &getTexture(const std::string &path) { return m_textures.at(path); }
    const VertexArray &getVertexArray(const std::string &path) { return m_vertexArrays.at(path); }
    const TextureCubemap &getCubemap(const std::string &path) { return m_textureCubemaps.at(path); }
    std::string loadShader(const std::string &name);
    std::string loadTexture(const std::string &tgaPath);
    std::string loadTextureCubemap(const std::string &tgaPath);
    std::string loadVertexArray(const std::string &objPath);
    std::string loadVertexArray(const std::string &id, const std::vector<Vertex> &, const std::vector<GLuint> &indices);
};
