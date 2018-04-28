#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "Camera.hpp"
#include "constants.hpp"
#include "Drawable.hpp"
#include "ResourceManager.hpp"

class Renderer
{
    GLFWwindow *m_window;
    Camera *m_camera;
    ResourceManager m_resourceManager;

    GLenum m_primitive = GL_TRIANGLES;
    std::string m_textureMissing;
    // Bound objects
    std::string m_boundShader;

  public:
    Renderer(GLFWwindow *window, Camera *camera);

    void setTextureMissing(const std::string &tex) { m_textureMissing = tex; }
    void setPrimitive(GLenum mode) { m_primitive = mode; }
    void setShader(const std::string &shader);

    void setUniformColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { glUniform4f(UNIFORM_COLOR, r / 255.0, g / 255.0, b / 255.0, a / 255.0); }

    void enableWireframe() const { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
    void disableWireframe() const { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
    void enableBlend() const { glEnable(GL_BLEND); }
    void disableBlend() const { glDisable(GL_BLEND); }

    // Resources
    std::string loadShader(const std::string &name) { return m_resourceManager.loadShader(name); }
    std::string loadTexture(const std::string &tgaPath) { return m_resourceManager.loadTexture(tgaPath); }
    std::string loadTextureCubemap(const std::string &tgaPath) { return m_resourceManager.loadTextureCubemap(tgaPath); }
    std::string loadVertexArray(const std::string &objPath) { return m_resourceManager.loadVertexArray(objPath); }
    std::string loadVertexArray(const std::string &id, const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices) { return m_resourceManager.loadVertexArray(id, vertices, indices); }
    // Drawing
    void pre();
    void renderSkybox(const std::string &shader, const std::string &cubemap, const std::string &vertexArray);
    void render(const std::string &shader, const std::string &texture, const std::string &vertexArray, const glm::mat4 &modelWorld);
    void render(const Drawable &);
    void post();
};