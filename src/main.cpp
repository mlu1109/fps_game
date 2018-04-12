#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <unordered_map>
#include "renderer/shader.hpp"
#include "renderer/model.hpp"
#include "renderer/renderer.hpp"
#include "renderer/camera.hpp"
#include "renderer/meshes/cube.hpp"
#include "renderer/vertexarray.hpp"
#include "mouse.hpp"
#include "callbacks.hpp"
#include <memory>

GLFWwindow *initWindow()
{
    const int WIDTH = 800;
    const int HEIGHT = 600;
    const char *TITLE = "ENGINE_3D";

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    if (!glfwInit())
        throw std::runtime_error("glfwInit failed");

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
    if (window == nullptr)
    {
        glfwTerminate();
        throw std::runtime_error("glfwCreateWindow failed");
    }

    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        throw std::runtime_error("glewInit failed");
    }

    return window;
}

void handleKeyInput(GLFWwindow *window, Camera &camera)
{
    const float speed = 0.02;
    glm::vec3 translate{0};
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.moveZ(speed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.moveX(-speed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.moveZ(-speed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.moveX(speed);
}

int main()
{
    GLFWwindow *window = initWindow();
    Shader shader = initShaders();
    Camera camera{{0.0f, 0.0f, -5.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}};
    Renderer renderer{window, &camera, shader.program};
    // VertexArray cube{Meshes::CubeIdx::vertices, Meshes::CubeIdx::indices};
    VertexArray cube{Meshes::Cube::vertices};
    Model modelCube{{&cube}, glm::mat4{1.f}, glm::mat4{1.f}, glm::mat4{1.f}};
    std::vector<Model> models;
    models.push_back(modelCube);

    Mouse mouse(&camera);
    initCallbacks(window, &mouse, &camera);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        handleKeyInput(window, camera);
        //models[0].rotate(glm::vec3{1, 1, 1}, 0.001);
        renderer.render(models);
    }
}