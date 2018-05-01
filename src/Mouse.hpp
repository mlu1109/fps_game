#pragma once

#include <GLFW/glfw3.h>
#include <unordered_map>
#include "renderer/Camera.hpp"
#include "Game.hpp"


class Mouse
{
    Camera *m_camera;
    Game *m_game;
    double m_mouseLastX;
    double m_mouseLastY;
    double m_sens = 0.005;
    bool m_mouseLook = false;
    bool m_mouseInvertedY = false;

    void castRay(int x, int y, int screenWidth, int screenHeight); // Should be moved
    void onRelease(GLFWwindow *window, int button);
    void onPress(GLFWwindow *window, int button);

  public:
    Mouse(Camera *camera, Game *game) : m_camera(camera), m_game(game) {}

    void onMove(double x, double y);
    void onButton(GLFWwindow *window, int button, int action);
};
