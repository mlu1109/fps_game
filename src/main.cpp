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
#include "Game.hpp"
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

const std::string TEXTURE_DIR = "assets/textures/";
const std::string MODEL_DIR = "assets/models/";
const std::string HEIGHTMAP_DIR = "assets/heightmaps/";
const std::string CUBEMAP_DIR = "assets/cubemaps/";

int main()
{
    GLFWwindow *window = initWindow();
    Camera camera;
    Renderer renderer{window, &camera};
    Game game;
    // Input
    Mouse mouse(&camera, &game);
    initCallbacks(window, &mouse, &camera);

    // Resources
    // --------------------------------------------------
    // Shaders
    auto shaderColorNormal = renderer.loadShader("color_normal");
    auto shaderColorSolid = renderer.loadShader("color_solid");
    auto shaderPlainTexture = renderer.loadShader("plain_texture");
    auto shaderSkybox = renderer.loadShader("skybox");
    auto shaderTerrain = renderer.loadShader("terrain");
    auto shaderWater = renderer.loadShader("water");
    // VertexArrays
    auto vaCubeId = renderer.loadVertexArray("cube.obj");
    auto vaLowPolyBush = renderer.loadVertexArray("Follage_Pack/OBJ Files/Low_Poly_Bush_001.obj");
    auto vaLowPolyTree = renderer.loadVertexArray("Follage_Pack/OBJ Files/Low_Poly_Tree_003.obj");
    auto vaSkybox = renderer.loadVertexArray("skybox.obj");
    auto vaSphere = renderer.loadVertexArray("sphere.obj");
    auto vaWater = renderer.loadVertexArray("grid.obj");
    // Objects (needed for bounding box calculations)
    auto objWater = OBJ(MODEL_DIR + vaWater);
    auto objLowPolyBush = OBJ(MODEL_DIR + vaLowPolyBush);
    auto objLowPolyTree = OBJ(MODEL_DIR + vaLowPolyTree);
    // Textures
    auto texSkybox = renderer.loadTextureCubemap("hw_desertnight/desert_night");
    auto texTerrainId = renderer.loadTexture("terrain50.tga");
    auto texWater = texSkybox;

    // Terrain (crappy design)
    TGA tgaHeightmap = loadTGA(HEIGHTMAP_DIR + "fft-terrain.tga");
    Heightmap terrain(tgaHeightmap, shaderTerrain, texTerrainId);
    auto vaTerrainId = renderer.loadVertexArray("fft-terrain.tga", terrain.getVertices(), terrain.getIndices());
    terrain.setVertexArray(vaTerrainId);

    // Models
    // --------------------------------------------------
    // Water
    Model modelWater(objWater.getVertices(), shaderWater, texWater, vaWater);
    // Vegetation
    Model vegetationBush(objLowPolyBush.getVertices(), "color_normal", "", vaLowPolyBush);
    Model vegetationTree(objLowPolyTree.getVertices(), "color_normal", "", vaLowPolyTree);

    for (int i = 0; i < 1000; ++i)
    {
        float x = rand() % terrain.getWidth();
        float z = rand() % terrain.getHeight();
        float y = terrain.getY(x, z);
        if (y < 13)
            continue;
        if (i % 2 == 0)
        {
            vegetationTree.setTranslate({x, y, z});
            vegetationTree.setRotate({0, 0, rand()});
            vegetationTree.update();
            game.addStaticObject(vegetationTree);
        }
        else
        {
            vegetationBush.setTranslate({x, y, z});
            vegetationBush.update();
            game.addStaticObject(vegetationBush);
        }
    }

    double t0 = glfwGetTime();
    int frameCount = 0;

    modelWater.scale({terrain.getWidth(), 0, terrain.getHeight()});
    modelWater.translate(terrain.getCenter());
    modelWater.translate({0, 12, 0});
    modelWater.update();
    camera.setPosition({terrain.getWidth() / 4, 0, terrain.getHeight() / 4});
    while (!glfwWindowShouldClose(window))
    {
        camera.setY(terrain.getY(camera.getX(), camera.getZ()) + 5);
        glfwPollEvents();
        handleKeyInput(window, camera);

        // Render
        // --------------------------------------------------
        renderer.pre();

        // Skybox, terrain
        renderer.renderSkybox(shaderSkybox, texSkybox, vaSkybox);
        renderer.render(terrain);
        // Static objects
        for (const auto &o : game.getStaticObjects())
            renderer.render(o);
        
        // Water
        renderer.enableBlend();
        renderer.setShader(shaderWater);
        renderer.setUniformTime(glfwGetTime());
        renderer.renderCubemap(modelWater);
        renderer.disableBlend();
        // Bounding volumes
        renderer.enableBlend();
        for (const auto &o : game.getStaticObjects())
        {
            renderer.setShader("color_solid");
            if (&o == game.getPicked())
                renderer.setUniformColor(255, 255, 255, 125);
            else
                renderer.setUniformColor(0, 200, 20, 25);

            renderer.render(o.getBoundingSphere());
            renderer.render(o.getAABB());
        }
        renderer.disableBlend();

        renderer.post();

        // Print FPS
        // --------------------------------------------------
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
