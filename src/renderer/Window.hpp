#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
    GLFWwindow *m_window;

  public:
    Window();

    bool shouldClose() const { return glfwWindowShouldClose(m_window); }
    void pollEvents() const { glfwPollEvents(); }
    void swapBuffers() const;

    void setErrorCallback(void (*funcPtr)(int code, const char *description));
    void setCursorPositionCallback(void (*funcPtr)(GLFWwindow *, double xpos, double ypos));
    void setWindowResizeCallback(void (*funcPtr)(GLFWwindow *, int width, int height));
    void setMouseButtonCallback(void (*funcPtr)(GLFWwindow *, int button, int action, int mods));

    bool isKeyPressed(int key) const { return glfwGetKey(m_window, key) == GLFW_PRESS; }

    void destroy();
};
