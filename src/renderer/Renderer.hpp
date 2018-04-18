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

  public:
    Renderer(GLFWwindow *window, Camera *camera);

    void pre();
    void render(const Drawable &);
    void post();
};