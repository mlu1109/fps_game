#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <unordered_map>
#include "Model.hpp"
#include "Keyboard.hpp"
#include "BoundingSphere.hpp"
#include "callbacks.hpp"
#include "loaders/OBJ.hpp"
#include "loaders/TGA.hpp"
#include "renderer/Camera.hpp"
#include "renderer/Renderer.hpp"
#include "renderer/Shader.hpp"
#include "renderer/VertexArrayCube.hpp"
#include "utility.hpp"

#include "glm/print.hpp"

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

int main()
{
    GLFWwindow *window = initWindow();
    Camera camera;
    Renderer renderer{window, &camera};
    // Input
    Mouse mouse(&camera);
    initCallbacks(window, &mouse, &camera);
    // Shaders
    auto shaderTerrain = std::make_shared<Shader>("/home/matti/Documents/fps_game/src/shaders/terrain.vert", "/home/matti/Documents/fps_game/src/shaders/terrain.frag");
    auto shaderTexture = std::make_shared<Shader>("/home/matti/Documents/fps_game/src/shaders/plain_texture.vert", "/home/matti/Documents/fps_game/src/shaders/plain_texture.frag");
    auto shaderNormal = std::make_shared<Shader>("/home/matti/Documents/fps_game/src/shaders/color_normal.vert", "/home/matti/Documents/fps_game/src/shaders/color_normal.frag");
    // Textures
    auto texCube = newTextureFromTGA("/home/matti/Documents/fps_game/src/assets/textures/tsbk07/rutor.tga");
    auto texTerrain = newTextureFromTGA("/home/matti/Documents/fps_game/src/assets/textures/terrain50.tga");
    auto texMissing = newTextureFromTGA("/home/matti/Documents/fps_game/src/assets/textures/missing.tga");
    texMissing->setRepeat();
    texCube->setRepeat();
    texTerrain->setRepeat();
    // VertexArrays
    auto vaSphere = newVertexArrayFromOBJ("/home/matti/Documents/fps_game/src/assets/models/sphere.obj");
    auto vaCube = newVertexArrayFromOBJ("/home/matti/Documents/fps_game/src/assets/models/cube.obj");
    // HeightMap
    auto heightMap = newHeightMapFromTGA("/home/matti/Documents/fps_game/src/assets/heightmaps/fft-terrain.tga", shaderTerrain, texTerrain);
    
    // Model Teapot
    auto teapotOBJ = loadOBJ("/home/matti/Documents/fps_game/src/assets/models/tsbk07/various/cow.obj");
    //auto teapotOBJ = loadOBJ("/home/matti/Documents/fps_game/src/assets/models/cube.obj");
    auto teapotVA = newVertexArrayFromOBJ(teapotOBJ);

    auto teapotBoundingSphere = getBoundingSphere(teapotOBJ);
    AABB teapotAABB{teapotOBJ};
    Mesh teapotMesh{shaderTexture, nullptr, teapotVA};
    Model teapotModel{teapotMesh};
    teapotModel.setAABB(teapotAABB);

    teapotModel.setBoundingSphere(teapotBoundingSphere);
    
    double t0 = glfwGetTime();
    int frameCount = 0;
    
    renderer.setTextureMissing(texMissing);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        handleKeyInput(window, camera);
        renderer.pre();
        renderer.render(heightMap);
        renderer.render(teapotModel);
        auto bs = teapotModel.getTransformedBoundingSphere();
        auto aabb = teapotModel.getAABB();
        renderer.enableWireframe();
        //renderer.render(shaderNormal, vaSphere, nullptr, Transform{bs.center, {0, 0, 0}, {bs.radius, bs.radius, bs.radius}}.getTransform());
        Transform t = teapotModel.getTransform();
        auto scale = aabb.getScale();
        renderer.render(shaderNormal, vaCube, nullptr, Transform{t.T, {0, 0, 0}, scale}.getTransform());
        renderer.disableWireframe();
        renderer.post();
        teapotModel.rotate({0.01, 0.01, 0.01});
        teapotModel.updateModelWorld();
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