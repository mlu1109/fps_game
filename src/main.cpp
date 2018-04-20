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
    const float speed = 0.4;
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
}

int main()
{
    GLFWwindow *window = initWindow();
    //Camera camera{{-30.0f, -20.0f, -50.0f}, {20.0f, 0.0f, 20.0f}};
    //Camera camera{{30.0f, -20.0f, -50.0f}, {20.0f, 0.0f, 20.0f}};
    Camera camera{{-30.0f, -20.0f, 50.0f}, {20.0f, 0.0f, 20.0f}};
    //Camera camera{{-30.0f, -20.0f, -50.0f}, {20.0f, 0.0f, 20.0f}};
    Renderer renderer{window, &camera};
    // Input
    Mouse mouse(&camera);
    initCallbacks(window, &mouse, &camera);

    auto shaderTerrain = std::make_shared<Shader>("/home/matti/Documents/fps_game/src/shaders/terrain.vert", "/home/matti/Documents/fps_game/src/shaders/terrain.frag");
    auto shaderCube = std::make_shared<Shader>("/home/matti/Documents/fps_game/src/shaders/plain_texture.vert", "/home/matti/Documents/fps_game/src/shaders/plain_texture.frag");
    //auto shader = std::make_shared<Shader>("/home/matti/Documents/fps_game/src/shaders/color_normal.vert", "/home/matti/Documents/fps_game/src/shaders/color_normal.frag");
    auto texCube = newTextureFromTGA("/home/matti/Documents/fps_game/src/assets/textures/tsbk07/rutor.tga");
    texCube->setRepeat();
    auto texTerrain = newTextureFromTGA("/home/matti/Documents/fps_game/src/assets/textures/terrain50.tga");
    texTerrain->setRepeat();
    //auto va = newVertexArrayFromOBJ("/home/matti/Documents/fps_game/src/assets/models/tsbk07/bunnyplus.obj");
    auto va = newCubeIdx();

    std::array<Model, 6> cube = {
        Model{Mesh{shaderCube, texCube, va}, Transform{{5, 0, 5}}},
        Model{Mesh{shaderCube, texCube, va}, Transform{{-5, 0, -5}}},
        Model{Mesh{shaderCube, texCube, va}, Transform{{-5, 0, 5}}},
        Model{Mesh{shaderCube, texCube, va}, Transform{{5, 0, -5}}},
        Model{Mesh{shaderCube, texCube, va}, Transform{{0, 0, 0}}},
        Model{Mesh{shaderCube, texCube, va}, Transform{{20, 0, 20}, {0, 0, 0}, {3, 3, 3}}},
    };

    auto heightMap = newHeightMapFromTGA("/home/matti/Documents/fps_game/src/assets/heightmaps/fft-terrain.tga", shaderTerrain, texTerrain);

    double t0 = glfwGetTime();
    int frameCount = 0;
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        handleKeyInput(window, camera);
        for (auto &c : cube)
        {
            c.rotate({0.01, 0.01, 0.01});
            c.updateModelWorld();
        }
        renderer.pre();
        renderer.render(heightMap);
        for (auto &c : cube)
            renderer.render(c);
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