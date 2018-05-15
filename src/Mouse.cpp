#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Mouse.hpp"
#include "Ray.hpp"

void Mouse::onRelease(GLFWwindow *window, int button)
{
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_RIGHT:
    {
        int nextMode;
        if (m_mouseLook)
        {
            nextMode = GLFW_CURSOR_NORMAL;
            m_mouseLook = false;
        }
        else
        {
            nextMode = GLFW_CURSOR_DISABLED;
            m_mouseLook = true;
        }

        glfwSetInputMode(window, GLFW_CURSOR, nextMode);
        break;
    }
    default:
    {
        //std::cout << "unhandled mouse button: " << button << '\n';
    }
    }
}

void Mouse::onPress(GLFWwindow *window, int button)
{
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
    {
        m_leftClick = true;
        m_leftClickX = m_mouseLastX;
        m_leftClickY = m_mouseLastY;
        break;
    }
    default:
    {
    }
    }
}

void Mouse::onButton(GLFWwindow *window, int button, int action)
{
    if (action == GLFW_RELEASE)
        onRelease(window, button);
    else if (action == GLFW_PRESS)
        onPress(window, button);
    else
    {
        //std::cout << "unhandled mouse action: " << action << '\n';}
    }
}

void Mouse::onMove(double x, double y)
{
    if (m_mouseLook)
    {
        double dx = x - m_mouseLastX;
        double dy = m_mouseInvertedY ? y - m_mouseLastY : m_mouseLastY - y;
        m_camera->yaw(dx * m_sens);
        m_camera->pitch(dy * m_sens);
    }
    m_mouseLastX = x;
    m_mouseLastY = y;
}

void Mouse::onScrollY(double yOffset)
{
    m_mouseWheel += yOffset;
}

bool Mouse::hasLeftClicked()
{
    bool ret = m_leftClick;
    m_leftClick = false;
    return ret;
}