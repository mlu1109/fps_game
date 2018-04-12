#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "camera.hpp"
#include "model.hpp"

class Renderer
{
    GLFWwindow *m_window;
    Camera *m_camera;
    glm::vec4 m_clearColor{0.2f, 0.2f, 0.5f, 1.0f};
    GLuint m_program; // Shader program (temp)
public:
    Renderer(GLFWwindow *window, Camera *camera, GLuint program);
    void render(const std::vector<Model> &models);
};