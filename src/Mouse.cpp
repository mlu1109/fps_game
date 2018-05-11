#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Mouse.hpp"
#include "Ray.hpp"

void Mouse::castRay(int x, int y, int screenWidth, int screenHeight)
{
    // Reference: http://antongerdelan.net/opengl/raycasting.html
    
    // Find direction in world coordinates [-1, 1] for x, y, z
    glm::vec4 ray_nds(
        (2.0f * x) / screenWidth - 1.0f,
        1.0f - (2.0f * y) / screenHeight,
        -1.0f, // OpenGL forward
        1.0f);

    // 4d Camera coordinates
    glm::vec4 ray_cam = glm::inverse(m_camera->getViewScreen()) * ray_nds;
    ray_cam.w = 0.0f;
    glm::vec3 ray_wor = glm::inverse(m_camera->getWorldView()) * ray_cam;
    Ray ray{m_camera->getPosition(), glm::normalize(ray_wor)};
    //m_game->pickByRay(ray);
}

void Mouse::onRelease(GLFWwindow *window, int button)
{
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
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
    case GLFW_MOUSE_BUTTON_RIGHT:
    {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        castRay(m_mouseLastX, m_mouseLastY, width, height);

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
