#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <unordered_map>
#include "BoundingSphere.hpp"
#include "Callbacks.hpp"
#include "Keyboard.hpp"
#include "Heightmap.hpp"
#include "renderer/Camera.hpp"
#include "renderer/Renderer.hpp"
#include "renderer/Window.hpp"
#include "glm/print.hpp"
#include "GameObject.hpp"
#include "ResourceManager.hpp"
#include <cmath>

int main()
{
    Window window;
    Camera camera;
    camera.setPosition({0, 0, 0});
    Renderer renderer;
    ResourceManager resourceManager;
    // Input
    Mouse mouse(&camera);
    initCallbacks(&window, &mouse, &camera);

    GameObject goSphere = resourceManager.loadObject("assets/models/Follage_Pack/OBJ Files/Low_Poly_Tree_002.obj");
    Skybox skybox = resourceManager.loadSkybox("assets/models/skybox.obj", "assets/cubemaps/hw_desertnight/desert_night", "skybox");
    skybox.setLightPosition({30, 85, 133});
    skybox.setLightColor({0.91, 1.0, 0.94});
    //const Shader *shaderNormal = resourceManager.loadShader("color_normal");
    const Shader *shaderPhong = resourceManager.loadShader("phong");
    double t0 = glfwGetTime();
    int frameCount = 0;
    while (!window.shouldClose())
    {
        glfwPollEvents();
        handleKeyInput(window, camera);

        goSphere.rotate({0, 0.005, 0});
        goSphere.update();
        // Render
        // --------------------------------------------------
        renderer.clear();
        skybox.render(renderer, camera.getWorldView(), camera.getViewScreen());
        renderer.setShader(shaderPhong);
        renderer.setUniformWorldView(camera.getWorldView());
        renderer.setUniformViewScreen(camera.getViewScreen());
        renderer.setUniformLightPosition(skybox.getLightPosition());
        renderer.setUniformLightColor(skybox.getLightColor() * 0.4f);
        renderer.setUniformCameraPosition(camera.getPosition());
        goSphere.getModel().render(renderer, camera.getWorldView());
        window.swapBuffers();

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
