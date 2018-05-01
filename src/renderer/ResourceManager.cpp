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

    for (auto &p : m_textureCubemaps)
        p.second.destroy();
}

std::string ResourceManager::loadShader(const std::string &name)
{
    m_shaders.emplace(name, Shader(SHADER_DIR + name + ".vert", SHADER_DIR + name + ".frag"));
    return name;
}

std::string ResourceManager::loadTexture(const std::string &tgaPath)
{
    TGA tga = loadTGA(TEXTURE_DIR + tgaPath);
    GLint internalFormat = tga.bitsPerPixel == 32 ? GL_RGBA : GL_RGB;
    m_textures.emplace(tgaPath, Texture(tga.imageData, internalFormat, tga.width, tga.height));
    return tgaPath;
}

std::string ResourceManager::loadTextureCubemap(const std::string &tgaPath)
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

    m_textureCubemaps.emplace(tgaPath, TextureCubemap());
    TextureCubemap &texture = m_textureCubemaps[tgaPath];
    for (int i = 0; i < 6; ++i)
    {
        GLint internalFormat = tgas[i].bytesPerPixel == 4 ? GL_RGBA : GL_RGB;
        texture.loadSide(sideTargets[i], tgas[i].imageData, internalFormat, tgas[i].width, tgas[i].height);
    }
    return tgaPath;
}

std::string ResourceManager::loadVertexArray(const std::string &objPath)
{
    OBJ obj(MODEL_DIR + objPath);
    auto pair = obj.getVerticesIndices();
    m_vertexArrays.emplace(objPath, VertexArray(pair.first, pair.second));
    return objPath;
}

std::string ResourceManager::loadVertexArray(const std::string &id, const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices)
{
    m_vertexArrays.emplace(id, VertexArray(vertices, indices));
    return id;
}