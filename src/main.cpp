#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <unordered_map>
#include "Model.hpp"
#include "callbacks.hpp"
#include "loaders/OBJ.hpp"
#include "loaders/TGA.hpp"
#include "renderer/Camera.hpp"
#include "renderer/Renderer.hpp"
#include "renderer/Shader.hpp"
#include "renderer/VertexArrayCube.hpp"
#include "utility.hpp"

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

    //glfwSwapInterval(0);

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
    Camera camera{{0.0f, 0.0f, -5.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}};
    Renderer renderer{window, &camera};
    // Input
    Mouse mouse(&camera);
    initCallbacks(window, &mouse, &camera);
    
    auto shader = std::make_shared<Shader>("/home/matti/Documents/fps_game/src/shaders/plain_texture.vert", "/home/matti/Documents/fps_game/src/shaders/plain_texture.frag");
    auto tex = newTextureFromTGA("/home/matti/Documents/fps_game/src/assets/textures/tsbk07/rutor.tga");
    //auto va = newVertexArrayFromOBJ("/home/matti/Documents/fps_game/src/assets/models/tsbk07/bunnyplus.obj");
    auto va = newCubeIdx();
    Model model{Mesh{shader, tex, va}};

    double t0 = glfwGetTime();
    int frameCount = 0;
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        handleKeyInput(window, camera);
        model.rotate({0.01, 0.01, 0.01});
        model.updateModelWorld();
        renderer.pre();
        renderer.render(model);
        renderer.post();
        ++frameCount;
        double t1 = glfwGetTime();
        if (t1 - t0 >= 1)
        {
            std::cout << "FPS: " << frameCount << '\n';
            frameCount = 0;
            t0 = t1;
        }
    }
}