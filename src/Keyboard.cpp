#include "Keyboard.hpp"
#include <iostream>
#include "glm/print.hpp"


void handleKeyInput(const Window &window, Camera &camera)
{
    const float speed = 0.4;
    glm::vec3 translate{0};
    if (window.isKeyPressed(GLFW_KEY_W))
        camera.moveD(-speed);
    if (window.isKeyPressed(GLFW_KEY_A))
        camera.moveR(-speed);
    if (window.isKeyPressed(GLFW_KEY_S))
        camera.moveD(speed);
    if (window.isKeyPressed(GLFW_KEY_D))
        camera.moveR(speed);

    if (window.isKeyPressed(GLFW_KEY_UP))
        camera.pitch(speed * 0.1);
    if (window.isKeyPressed(GLFW_KEY_LEFT))
        camera.yaw(-speed * 0.1);
    if (window.isKeyPressed(GLFW_KEY_DOWN))
        camera.pitch(-speed * 0.1);
    if (window.isKeyPressed(GLFW_KEY_RIGHT))
        camera.yaw(speed * 0.1);
    
    if (window.isKeyPressed(GLFW_KEY_P))
        std::cout << "Camera position: " << camera.getPosition() << '\n';
}
