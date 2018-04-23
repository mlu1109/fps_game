#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <unordered_map>
#include "BoundingSphere.hpp"
#include "Callbacks.hpp"
#include "Keyboard.hpp"
#include "Model.hpp"
#include "Utility.hpp"
#include "loaders/OBJ.hpp"
#include "loaders/TGA.hpp"
#include "renderer/Camera.hpp"
#include "renderer/Renderer.hpp"
#include "renderer/Shader.hpp"
#include "renderer/VertexArrayCube.hpp"
#include "glm/print.hpp"


const std::string SHADER_DIR = "src/shaders/";
const std::string TEXTURE_DIR = "src/assets/textures/";
const std::string MODEL_DIR = "src/assets/models/";
const std::string HEIGHTMAP_DIR = "src/assets/heightmaps/";


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
    auto shaderTerrain = std::make_shared<Shader>(SHADER_DIR + "terrain.vert", SHADER_DIR + "terrain.frag");
    auto shaderTexture = std::make_shared<Shader>(SHADER_DIR + "plain_texture.vert", SHADER_DIR + "plain_texture.frag");
    auto shaderNormal = std::make_shared<Shader>(SHADER_DIR + "color_normal.vert", SHADER_DIR + "color_normal.frag");
    auto shaderColor = std::make_shared<Shader>(SHADER_DIR + "color_solid.vert", SHADER_DIR + "color_solid.frag");
    // Textures
    auto texCube = newTextureFromTGA(TEXTURE_DIR + "tsbk07/rutor.tga");
    auto texTerrain = newTextureFromTGA(TEXTURE_DIR + "terrain50.tga");
    auto texMissing = newTextureFromTGA(TEXTURE_DIR + "missing.tga");
    texMissing->setRepeat();
    texCube->setRepeat();
    texTerrain->setRepeat();
    // VertexArrays
    auto vaSphere = newVertexArrayFromOBJ(MODEL_DIR + "sphere.obj");
    auto vaCube = newVertexArrayFromOBJ(MODEL_DIR + "cube.obj");
    // HeightMap
    auto heightMap = newHeightMapFromTGA(HEIGHTMAP_DIR + "fft-terrain.tga", shaderTerrain, texTerrain);

    // Model Teapot
    auto modelOBJ = loadOBJ(MODEL_DIR + "tsbk07/various/cow.obj");
    //auto teapotOBJ = loadOBJ(MODEL_DIR + "cube.obj");
    auto modelVA = newVertexArrayFromOBJ(modelOBJ);

    BoundingSphere teapotBoundingSphere(modelOBJ);
    AABB modelAABB(modelOBJ);
    Mesh modelMesh{shaderNormal, nullptr, modelVA};
    Model model{modelMesh};
    model.setAABB(modelAABB);
    model.setBoundingSphere(teapotBoundingSphere);
    auto modelCopy = model;
    model.scale({2, 2, 2});

    double t0 = glfwGetTime();
    int frameCount = 0;

    renderer.setTextureMissing(texMissing);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        handleKeyInput(window, camera);
        renderer.pre();
        renderer.render(heightMap);
        renderer.render(model);
        renderer.render(modelCopy);
        BoundingSphere bs0 = model.getBoundingSphere();
        BoundingSphere bs1 = modelCopy.getBoundingSphere();
        AABB aabb0 = model.getAABB();
        AABB aabb1 = modelCopy.getAABB();
        renderer.setShader(shaderColor);

        if (aabb0.isIntersecting(bs1))
            renderer.setUniformColor(255, 0, 25, 20);
        else
            renderer.setUniformColor(0, 255, 25, 20);

        renderer.enableBlend();
        //renderer.render(shaderColor, vaSphere, nullptr, bs0.getTransform().getMatrix());
        renderer.render(shaderColor, vaSphere, nullptr, bs1.getTransform().getMatrix());
        renderer.render(shaderColor, vaCube, nullptr, aabb0.getTransform().getMatrix());
        //renderer.render(shaderColor, vaCube, nullptr, aabb1.getTransform().getMatrix());
        renderer.disableBlend();
        renderer.post();
        model.rotate({0.01, 0.01, 0.01});
        model.translate({0.001, 0.001, 0.001});
        //model.scale({0.005, 0.005, 0.005});
        model.updateModelWorld();
        modelCopy.rotate({0.01, 0.01, 0.01});
        modelCopy.updateModelWorld();
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
