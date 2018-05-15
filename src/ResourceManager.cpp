#include "ResourceManager.hpp"

const std::string SHADER_DIR = "src/shaders/";
const std::string TEXTURE_DIR = "assets/textures/";
const std::string MODEL_DIR = "assets/models/";

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
    if (m_shaders.count(name))
        return &m_shaders.at(name);

    m_shaders.emplace(name, Shader(SHADER_DIR + name + ".vert", SHADER_DIR + name + ".frag"));

    return &m_shaders.at(name);
}

const Texture *ResourceManager::loadTexture(const std::string &tgaPath)
{
    TGA tga = loadTGA(tgaPath);
    GLint internalFormat = tga.bitsPerPixel == 32 ? GL_RGBA : GL_RGB;
    m_textures.emplace(tgaPath, Texture(tga.imageData, internalFormat, tga.width, tga.height));

    return &m_textures.at(tgaPath);
}

const Cubemap *ResourceManager::loadCubemap(const std::string &tgaPath)
{
    if (m_Cubemaps.count(tgaPath))
        return &m_Cubemaps.at(tgaPath);

    std::array<std::string, 6> paths = {
        tgaPath + "_ft.tga", tgaPath + "_bk.tga",
        tgaPath + "_up.tga", tgaPath + "_dn.tga",
        tgaPath + "_rt.tga", tgaPath + "_lf.tga"};

    std::array<TGA, 6> tgas;
    for (int i = 0; i < 6; ++i)
        tgas[i] = loadTGA(paths[i]);

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
    std::string id = mesh.path + mesh.name;
    return loadVertexArray(id, mesh.vertices, mesh.indices);
}

const VertexArray *ResourceManager::loadVertexArray(const std::string &id, const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices)
{
    if (m_vertexArrays.count(id))
        return &m_vertexArrays.at(id);

    m_vertexArrays.emplace(id, VertexArray(vertices, indices));

    return &m_vertexArrays.at(id);
}

GameObject ResourceManager::loadObject(const std::string &objPath, int meshIdx)
{
    Mesh mesh = OBJ(objPath).getMeshes()[meshIdx];
    const VertexArray *vaMesh = loadVertexArray(mesh);

    return GameObject(Model(vaMesh, mesh.materials, mesh.indexMaterialId));
}

GameObject ResourceManager::loadObject(const std::string &objPath, const std::string &objHitboxPath, int meshIdx)
{
    Mesh mesh = OBJ(objPath).getMeshes()[meshIdx];
    OBJ hitboxes(objHitboxPath);
    const VertexArray *vaMesh = loadVertexArray(mesh);

    return GameObject(Model(vaMesh, mesh.materials, mesh.indexMaterialId), hitboxes);
}

Skybox ResourceManager::loadSkybox(const std::string &objPath, const std::string &cubemapPath, const std::string &shaderName)
{
    Mesh mesh = OBJ(objPath).getMeshes()[0];
    const VertexArray *va = loadVertexArray(mesh);
    const Cubemap *cm = loadCubemap(cubemapPath);
    const Shader *s = loadShader(shaderName);

    return Skybox(s, cm, va);
}

Terrain ResourceManager::loadTerrain(const std::string &tgaHeightmapPath)
{
    TGA tgaHeightmap = loadTGA(tgaHeightmapPath);
    Heightmap hm(tgaHeightmap);
    const VertexArray *va = loadVertexArray(tgaHeightmapPath, hm.vertices, hm.indices);

    return Terrain(hm, va);
}

Player ResourceManager::loadPlayer(const std::string &objPath, const std::string &objHitboxPath)
{
    Mesh mesh = OBJ(objPath).getMeshes()[0];
    const VertexArray *va = loadVertexArray(mesh);
    OBJ hitboxes(objHitboxPath);

    return Player(Model(va, mesh.materials, mesh.indexMaterialId), hitboxes);
}

Enemy ResourceManager::loadEnemy(const std::string &objPath, const std::string &objHitboxPath)
{
    Mesh mesh = OBJ(objPath).getMeshes()[0];
    const VertexArray *va = loadVertexArray(mesh);
    OBJ hitboxes(objHitboxPath);

    return Enemy(Model(va, mesh.materials, mesh.indexMaterialId), hitboxes);
}