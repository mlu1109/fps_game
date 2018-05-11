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
#include <cmath>

int main()
{
    Window window;
    Camera camera;
    Renderer renderer;
    ResourceManager resourceManager;
    // Input
    Mouse mouse(&camera);
    initCallbacks(&window, &mouse, &camera);

    OBJ objSphere("assets/models/sphere.obj");
    const VertexArray *vaSphere = resourceManager.loadVertexArray(objSphere.getMeshes()[0]);
    const Shader *shaderNormal = resourceManager.loadShader("color_normal");

    double t0 = glfwGetTime();
    int frameCount = 0;
    while (!window.shouldClose())
    {
        glfwPollEvents();
        handleKeyInput(window, camera);

        // Render
        // --------------------------------------------------
        renderer.clear();
        renderer.setShader(shaderNormal);
        renderer.setUniformWorldView(camera.getWorldView());
        renderer.setUniformViewScreen(camera.getViewScreen());
        renderer.setUniformModelWorld(glm::translate(glm::mat4{1.0f}, glm::vec3{0, 0, -1.0f}));
        renderer.render(vaSphere);
        window.swapBuffers();

        // Print FPS
        // --------------------------------------------------
        ++frameCount;
        double t1 = glfwGetTime();
        if (t1 - t0 >= 1)
        {
         //   std::cout << "FPS: " << frameCount << '\n';
            frameCount = 0;
            t0 = t1;
        }
    }
}
