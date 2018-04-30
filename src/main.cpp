#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <unordered_map>
#include "BoundingSphere.hpp"
#include "Callbacks.hpp"
#include "Keyboard.hpp"
#include "Model.hpp"
#include "Heightmap.hpp"
#include "renderer/Camera.hpp"
#include "renderer/Renderer.hpp"
#include "glm/print.hpp"
#include <cmath>

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

const std::string SHADER_DIR = "src/shaders/";
const std::string TEXTURE_DIR = "assets/textures/";
const std::string MODEL_DIR = "assets/models/";
const std::string HEIGHTMAP_DIR = "assets/heightmaps/";
const std::string CUBEMAP_DIR = "assets/cubemaps/";

Model *picked = nullptr;

int main()
{
    GLFWwindow *window = initWindow();
    Camera camera;
    Renderer renderer{window, &camera};
    // Input
    Mouse mouse(&camera);
    initCallbacks(window, &mouse, &camera);
    // Resources
    const std::string modelPlane = "grid.obj";
    const std::string modelPath = "cube.obj";
    renderer.loadShader("color_normal");
    renderer.loadShader("color_solid");
    renderer.loadShader("plain_texture");
    renderer.loadShader("terrain");
    renderer.loadVertexArray("sphere.obj");
    renderer.loadVertexArray("cube.obj");
    renderer.loadVertexArray(modelPath);
    auto missingTex = renderer.loadTexture("terrain50.tga");
    OBJ objModel = OBJ(MODEL_DIR + modelPath);
    Model model0(objModel.getVertices(), "color_normal", "", modelPath);
    Model model1(objModel.getVertices(), "color_normal", "", modelPath);

    // Terrain
    TGA tgaHeightmap = loadTGA(HEIGHTMAP_DIR + "fft-terrain.tga");
    Heightmap heightmap(tgaHeightmap, "terrain", missingTex);
    renderer.loadVertexArray("fft-terrain", heightmap.getVertices(), heightmap.getIndices());
    heightmap.setVertexArray("fft-terrain");

    // Vegetation

    std::vector<Model> vegetation;
    auto lowPolyBush = renderer.loadVertexArray("Follage_Pack/OBJ Files/Low_Poly_Bush_001.obj");
    auto lowPolyTree = renderer.loadVertexArray("Follage_Pack/OBJ Files/Low_Poly_Tree_003.obj");
    objModel = OBJ(MODEL_DIR + lowPolyTree);
    Model vegetationTree(objModel.getVertices(), "color_normal", "", lowPolyTree);
    objModel = OBJ(MODEL_DIR + lowPolyBush);
    Model vegetationBush(objModel.getVertices(), "color_normal", "", lowPolyBush);

    for (int i = 0; i < 1000; ++i)
    {
        float x = rand() % heightmap.getWidth();
        float z = rand() % heightmap.getHeight();
        float y = heightmap.getY(x, z);
        if (y < 13)
            continue;
        if (i % 2 == 0)
        {
            vegetationTree.setTranslate({x, y, z});
            vegetationTree.setRotate({0, 0, rand()});
            vegetationTree.update();
            vegetation.push_back(vegetationTree);
        }
        else
        {
            vegetationBush.setTranslate({x, y, z});
            vegetationBush.update();
            vegetation.push_back(vegetationBush);
        }
    }

    // Skybox
    auto skyboxShader = renderer.loadShader("skybox");
    auto skyboxTex = renderer.loadTextureCubemap(CUBEMAP_DIR + "hw_desertnight/desert_night");
    auto skyboxVA = renderer.loadVertexArray("skybox.obj");

    // Water
    auto waterShader = renderer.loadShader("water");
    auto waterTex = skyboxTex;
    auto waterVA = renderer.loadVertexArray("grid.obj");
    auto waterOBJ = OBJ(MODEL_DIR + waterVA);
    Model modelWater(waterOBJ.getVertices(), waterShader, waterTex, waterVA);

    double t0 = glfwGetTime();
    int frameCount = 0;

    modelWater.scale({heightmap.getWidth(), 0, heightmap.getHeight()});
    modelWater.translate(heightmap.getCenter());
    modelWater.translate({0, 12, 0});
    modelWater.update();
    camera.setPosition({heightmap.getWidth() / 4, 0, heightmap.getHeight() / 4});
    model0.update();

    model1.update();
    while (!glfwWindowShouldClose(window))
    {
        camera.setY(heightmap.getY(camera.getX(), camera.getZ()) + 5);
        glfwPollEvents();
        handleKeyInput(window, camera);
        renderer.pre();
        renderer.renderSkybox(skyboxShader, skyboxTex, skyboxVA);
        renderer.render(heightmap);
        for (const auto &v : vegetation)
        {
            renderer.render(v);
        }
        renderer.enableBlend();
        //renderer.enableWireframe();
        renderer.renderCubemap(modelWater);
        renderer.setUniformTime(glfwGetTime());
        //renderer.disableWireframe();
        for (const auto &v : vegetation)
        {
            renderer.setShader("color_solid");
            if (&v == picked)
                renderer.setUniformColor(255, 255, 255, 125);
            else
                renderer.setUniformColor(0, 200, 20, 25);

            auto &bs = v.getBoundingSphere();
            renderer.render(bs);
            renderer.render(v.getAABB());
        }
        renderer.disableBlend();
        renderer.post();
        //model0.translate({0.003, 0.003, 0.003});
        //model0.rotate({0.001, 0.001, 0.001});
        //model0.scale({0.002, 0.002, 0.002});
        model0.update();
        model1.translate({-0.003, -0.003, -0.003});
        model1.rotate({0.01, 0.01, 0.01});
        model1.update();
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
