#include "Callbacks.hpp"

Mouse *g_mouse;
Camera *g_camera;

void errorCallback(int, const char *description)
{
    std::cout << "Error: " << description << '\n';
}

void windowResizeCallback(GLFWwindow *, int width, int height)
{
    glViewport(0, 0, width, height);
    g_camera->updateAspectRatio(width, height);
}

static void cursorPositionCallback(GLFWwindow *, double xpos, double ypos)
{
    g_mouse->onMove(xpos, ypos);
}

/*
void cursorEnterCallback(GLFWwindow *window, int entered)
{
    if (entered)
    {
        // The cursor entered the client area of the window
    }
    else
    {
        // The cursor left the client area of the window
    }
}
*/

void mouseButtonCallback(GLFWwindow *window, int button, int action, int /* mods */)
{
    g_mouse->onButton(window, button, action);
}

void initCallbacks(Window *window, Mouse *mouse, Camera *camera)
{
    g_mouse = mouse;
    g_camera = camera;
    // Error
    window->setErrorCallback(errorCallback);
    // Window
    window->setWindowResizeCallback(windowResizeCallback);
    // Cursor/Mouse
    //glfwSetCursorEnterCallback(g_window, cursorEnterCallback);
    window->setCursorPositionCallback(cursorPositionCallback);
    window->setMouseButtonCallback(mouseButtonCallback);
}
