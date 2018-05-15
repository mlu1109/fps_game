#pragma once

#include <GLFW/glfw3.h>
#include <unordered_map>
#include "Camera.hpp"

class Mouse
{
    Camera *m_camera;

    double m_mouseLastX;
    double m_mouseLastY;
    double m_sens = 0.005;
    int m_mouseWheel = 0;
    bool m_mouseLook = false;
    bool m_mouseInvertedY = false;

    bool m_leftClick = false;
    int m_leftClickX;
    int m_leftClickY;

    void onRelease(GLFWwindow *window, int button);
    void onPress(GLFWwindow *window, int button);

  public:
    Mouse(Camera *camera) : m_camera(camera) {}

    int getMouseWheel() const { return m_mouseWheel; }
    int getLeftClickX() const { return m_leftClickX; }
    int getLeftClickY() const { return m_leftClickY; }
    
    void onMove(double x, double y);
    void onButton(GLFWwindow *window, int button, int action);
    void onScrollY(double yOffset);
    bool hasLeftClicked();
};
