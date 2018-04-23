#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "Camera.hpp"
#include "Drawable.hpp"

class Renderer
{
    GLFWwindow *m_window;
    Camera *m_camera;

    GLenum m_primitive = GL_TRIANGLES;
    std::shared_ptr<Texture> m_textureMissing;
    // Bound objects
    std::shared_ptr<Shader> m_boundShader;

  public:
    Renderer(GLFWwindow *window, Camera *camera);

    void setTextureMissing(const std::shared_ptr<Texture> &tex) { m_textureMissing = tex; }
    void setPrimitive(GLenum mode) { m_primitive = mode; }

    void enableWireframe() const { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
    void disableWireframe() const { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }

    void pre();
    void render(const std::shared_ptr<Shader> &, const std::shared_ptr<VertexArray> &, const std::shared_ptr<Texture> &, const glm::mat4 &modelWorld); 
    void render(const Drawable &);
    void post();
};