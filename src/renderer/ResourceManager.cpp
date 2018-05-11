#include "ResourceManager.hpp"

const std::string SHADER_DIR = "src/shaders/";
const std::string TEXTURE_DIR = "assets/textures/";
const std::string MODEL_DIR = "assets/models/";
const std::string CUBEMAP_DIR = "assets/cubemaps/";

ResourceManager::~ResourceManager()
{
    for (auto &p : m_shaders)
        p.second.destroy();

    for (auto &p : m_textures)
        p.second.destroy();

    for (auto &p : m_vertexArrays)
        p.second.destroy();

    for (auto &p : m_Cubemaps)
        p.second.destroy();
}

const Shader *ResourceManager::loadShader(const std::string &name)
{
    m_shaders.emplace(name, Shader(SHADER_DIR + name + ".vert", SHADER_DIR + name + ".frag"));
    return &m_shaders.at(name);
}

const Texture *ResourceManager::loadTexture(const std::string &tgaPath)
{
    TGA tga = loadTGA(TEXTURE_DIR + tgaPath);
    GLint internalFormat = tga.bitsPerPixel == 32 ? GL_RGBA : GL_RGB;
    m_textures.emplace(tgaPath, Texture(tga.imageData, internalFormat, tga.width, tga.height));
    return &m_textures.at(tgaPath);
}

const Cubemap *ResourceManager::loadCubemap(const std::string &tgaPath)
{
    std::array<std::string, 6> paths = {
        tgaPath + "_ft.tga", tgaPath + "_bk.tga",
        tgaPath + "_up.tga", tgaPath + "_dn.tga",
        tgaPath + "_rt.tga", tgaPath + "_lf.tga"};

    std::array<TGA, 6> tgas;
    for (int i = 0; i < 6; ++i)
        tgas[i] = loadTGA(CUBEMAP_DIR + paths[i]);

    std::array<GLenum, 6> sideTargets = {
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_X};

    m_Cubemaps.emplace(tgaPath, Cubemap());
    Cubemap &texture = m_Cubemaps.at(tgaPath);
    for (int i = 0; i < 6; ++i)
    {
        GLint internalFormat = tgas[i].bytesPerPixel == 4 ? GL_RGBA : GL_RGB;
        texture.loadSide(sideTargets[i], tgas[i].imageData, internalFormat, tgas[i].width, tgas[i].height);
    }
    return &m_Cubemaps.at(tgaPath);
}

const VertexArray *ResourceManager::loadVertexArray(const Mesh &mesh)
{
    m_vertexArrays.emplace(mesh.name, VertexArray(mesh.vertices, mesh.indices));
    return &m_vertexArrays.at(mesh.name);
}
