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

int main()
{
    GLFWwindow *window = initWindow();
    Camera camera;
    Renderer renderer{window, &camera};
    // Input
    Mouse mouse(&camera);
    initCallbacks(window, &mouse, &camera);
    // Resources
    const std::string modelPath = "tsbk07/various/cow.obj";
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

    // Skybox
    auto skyboxShader = renderer.loadShader("skybox");
    auto skyboxTex = renderer.loadTextureCubemap(CUBEMAP_DIR + "hw_desertnight/desert_night");
    auto skyboxVA = renderer.loadVertexArray("skybox.obj");

    
    TGA tgaHeightmap = loadTGA(HEIGHTMAP_DIR + "fft-terrain.tga");
    Heightmap heightmap(tgaHeightmap, "terrain", missingTex);
    renderer.loadVertexArray("fft-terrain", heightmap.getVertices(), heightmap.getIndices());
    heightmap.setVertexArray("fft-terrain");
    double t0 = glfwGetTime();
    int frameCount = 0;
    model0.update();
    model1.update();
    while (!glfwWindowShouldClose(window))
    {
        //const glm::vec3 &camPos = camera.getPosition();
        //float y = heightmap.getY(camPos);
        //camera.setY(y + 5);
        //std::cout << camPos << '\n';
        glfwPollEvents();
        handleKeyInput(window, camera);
        renderer.pre();
        renderer.renderSkybox(skyboxShader, skyboxTex, skyboxVA);
        renderer.render(heightmap);
       // renderer.render(model0);
       // renderer.render(model1);
       // renderer.enableBlend();
       // renderer.setShader("color_solid");
       // if (model0.getAABB().isIntersecting(model1.getBoundingSphere()))
       //     renderer.setUniformColor(255, 0, 0, 20);
       // else
       //     renderer.setUniformColor(0, 255, 0, 20);
       // renderer.render(model0.getAABB());
       // renderer.render(model1.getBoundingSphere());
       // renderer.disableBlend();
        renderer.post();
        //model0.translate({0.003, 0.003, 0.003});
        model0.rotate({0.001, 0.001, 0.001});
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
