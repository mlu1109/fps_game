#include "Keyboard.hpp"
#include <iostream>
#include "glm/print.hpp"


void handleKeyInput(GLFWwindow *window, Camera &camera)
{
    const float speed = 0.2;
    glm::vec3 translate{0};
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.moveD(-speed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.moveR(-speed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.moveD(speed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.moveR(speed);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.pitch(speed * 0.1);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.yaw(-speed * 0.1);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.pitch(-speed * 0.1);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.yaw(speed * 0.1);
    
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        std::cout << "Camera position: " << camera.getPosition() << '\n';
}
