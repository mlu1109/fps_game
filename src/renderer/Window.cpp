#include <stdexcept>
#include "Error.hpp"
#include "Window.hpp"

Window::Window()
{
    const int WIDTH = 800;
    const int HEIGHT = 600;
    const char *TITLE = "Title";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    if (!glfwInit())
        throw std::runtime_error("glfwInit failed");

    m_window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
    if (m_window == nullptr)
    {
        glfwTerminate();
        throw std::runtime_error("glfwCreateWindow failed");
    }

    glfwMakeContextCurrent(m_window);
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        throw std::runtime_error("glewInit failed");
    }
}

void Window::destroy()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Window::swapBuffers() const
{
    glfwSwapBuffers(m_window);
    printError("Window::swapBuffers");
}

void Window::setErrorCallback(void (*funcPtr)(int code, const char *description))
{
    glfwSetErrorCallback(funcPtr);
}

void Window::setCursorPositionCallback(void (*funcPtr)(GLFWwindow *, double xpos, double ypos))
{
    glfwSetCursorPosCallback(m_window, funcPtr);
}

void Window::setWindowResizeCallback(void (*funcPtr)(GLFWwindow *, int width, int height))
{
    glfwSetWindowSizeCallback(m_window, funcPtr);
}

void Window::setMouseButtonCallback(void (*funcPtr)(GLFWwindow *, int button, int action, int mods))
{
    glfwSetMouseButtonCallback(m_window, funcPtr);
}

void Window::setMouseScrollCallback(void (*funcPtr)(GLFWwindow *, double /*xoffset */, double yoffset))
{
    glfwSetScrollCallback(m_window, funcPtr);
}