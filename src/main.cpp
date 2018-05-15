#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <unordered_map>
#include "BoundingSphere.hpp"
#include "Callbacks.hpp"
#include "Keyboard.hpp"
#include "Heightmap.hpp"
#include "Camera.hpp"
#include "renderer/Renderer.hpp"
#include "renderer/Window.hpp"
#include "glm/print.hpp"
#include "GameObject.hpp"
#include "ResourceManager.hpp"
#include "World.hpp"
#include "Enemy.hpp"
#include <cmath>

const std::string PATH_OBJ_SPHERE = "assets/models/sphere.obj";
const std::string PATH_OBJ_CUBE = "assets/models/cube.obj";
// Follage pack
const std::array<std::string, 6> PATH_OBJ_LOW_POLY_TREES{
    "assets/models/Follage_Pack/OBJ Files/Low_Poly_Tree_001.obj",
    "assets/models/Follage_Pack/OBJ Files/Low_Poly_Tree_002.obj",
    "assets/models/Follage_Pack/OBJ Files/Low_Poly_Tree_003.obj",
    "assets/models/Follage_Pack/OBJ Files/Low_Poly_Tree_004.obj",
    "assets/models/Follage_Pack/OBJ Files/Low_Poly_Tree_005.obj",
    "assets/models/Follage_Pack/OBJ Files/Low_Poly_Tree_006.obj"};
const std::array<std::string, 6> PATH_OBJ_LOW_POLY_TREE_HITBOXES{
    "assets/models/Follage_Pack/OBJ Files/Low_Poly_Tree_001_hitbox.obj",
    "assets/models/Follage_Pack/OBJ Files/Low_Poly_Tree_002_hitbox.obj",
    "assets/models/Follage_Pack/OBJ Files/Low_Poly_Tree_003_hitbox.obj",
    "assets/models/Follage_Pack/OBJ Files/Low_Poly_Tree_004_hitbox.obj",
    "assets/models/Follage_Pack/OBJ Files/Low_Poly_Tree_005_hitbox.obj",
    "assets/models/Follage_Pack/OBJ Files/Low_Poly_Tree_006_hitbox.obj"};
// Skybox
const std::string PATH_OBJ_SKYBOX = "assets/models/skybox.obj";
const std::string PATH_TGA_SKYBOX = "assets/cubemaps/hw_desertnight/desert_night";
const std::string SHADER_SKYBOX = "skybox";
// Colors
const glm::vec3 GREEN = {0.0f, 1.0f, 0.0f};
const glm::vec3 BROWN = {0.33f, 0.21f, 0.04f};
// Terrain
const std::string PATH_TGA_TERRAIN_HEIGHTMAP = "assets/heightmaps/fft-terrain.tga";
// Water
const std::string PATH_OBJ_WATER = "assets/models/grid.obj";
const std::string SHADER_WATER = "water";

World initWorld(ResourceManager &resourceManager)
{
    Terrain terrain = resourceManager.loadTerrain(PATH_TGA_TERRAIN_HEIGHTMAP);
    Skybox skybox = resourceManager.loadSkybox(PATH_OBJ_SKYBOX, PATH_TGA_SKYBOX, SHADER_SKYBOX);
    skybox.setLightPosition({30, 85, 133});
    skybox.setLightColor({0.91, 1.0, 0.94});
    GameObject water = resourceManager.loadObject(PATH_OBJ_WATER);
    water.scale({terrain.getWidth(), 0, terrain.getHeight()});
    water.translate({terrain.getWidth() / 2, 15, terrain.getHeight() / 2});
    water.update();
    World world(terrain, skybox, water);
    std::vector<GameObject> trees;
    //for (const auto &path : PATH_OBJ_LOW_POLY_TREES)
    for (int i = 0; i < 3; ++i)
        trees.emplace_back(resourceManager.loadObject(PATH_OBJ_LOW_POLY_TREES[i], PATH_OBJ_LOW_POLY_TREE_HITBOXES[i]));

    for (int i = 0; i < 1000; ++i)
        world.addStaticObjectOnRandomLocation(trees[rand() % trees.size()]);

    return world;
}

int main()
{
    Window window;
    Renderer renderer;
    ResourceManager resourceManager;
    World world = initWorld(resourceManager);
    Player player = resourceManager.loadPlayer("assets/models/opengameart/human_low.obj", "assets/models/opengameart/human_low.obj");
    Enemy enemy = resourceManager.loadEnemy("assets/models/opengameart/enemy_low.obj", "assets/models/opengameart/enemy_low.obj");
    Camera camera;
    glm::vec3 cameraPlayerOffset = {0.0, 2.0f, 0.0f};
    Mouse mouse(&camera);
    initCallbacks(&window, &mouse, &camera);

    /*
     * Load shaders
     */

    const Shader *shaderColorSolid = resourceManager.loadShader("color_solid");
    const Shader *shaderPhong = resourceManager.loadShader("phong");
    const Shader *shaderWater = resourceManager.loadShader("water");
    std::vector<const Shader *> shaders = {shaderColorSolid, shaderPhong, shaderWater};

    // Constant uniforms
    for (const auto shader : shaders)
    {
        renderer.setShader(shader);
        renderer.setUniformLightPosition(world.getSkybox().getLightPosition());
        renderer.setUniformLightColor(world.getSkybox().getLightColor() * 0.25f);
    }

    /*
     * Load VertexArrays
     */

    const VertexArray *vaSphere = resourceManager.loadVertexArray(OBJ(PATH_OBJ_SPHERE).getMeshes()[0]);
    const VertexArray *vaCube = resourceManager.loadVertexArray(OBJ(PATH_OBJ_CUBE).getMeshes()[0]);

    /*
     * Game loop
     */

    double t0 = glfwGetTime();
    double lastSpawn = t0;
    int frameCount = 0;
    world.addEnemyOnRandomLocation(enemy);
    while (!window.shouldClose())
    {
        glfwPollEvents();
        handleKeyInput(window, camera);
        camera.setY(world.getTerrain().getY(camera.getX(), camera.getZ()) + cameraPlayerOffset.y);

        /*
         * Enemies
         */

        if (t0 - lastSpawn > 10)
        {
            if (world.getEnemies().size() < 10)
            {
                world.addEnemyOnRandomLocation(enemy);
            }
            lastSpawn = t0;
        }

        world.moveEnemies(player);

        /*
         * Player
         */

        // Shoot
        if (mouse.hasLeftClicked())
        {
            Ray ray(camera.getWindowWidth() / 2, camera.getWindowHeight() / 2, camera.getWindowWidth(), camera.getWindowHeight(), camera);
            world.handleShot(ray);
        }

        // Move
        Transform newPlayerTransform = camera.getTransform();
        Transform oldPlayerTransform = player.getTransform();
        newPlayerTransform.R.x = 0;
        newPlayerTransform.R.z = 0;
        newPlayerTransform.T -= cameraPlayerOffset - (mouse.getMouseWheel() - 1.0f) * camera.getDirection();
        newPlayerTransform.T.y = world.getTerrain().getY(newPlayerTransform.T.x, newPlayerTransform.T.z);
        player.setTransform(newPlayerTransform);
        player.update();

        /*
         * Player static objects collisions
         */

        const std::vector<GameObject> &staticObjects = world.getStaticObjects();
        for (const auto &o : staticObjects)
        {
            if (o.getBoundingSphere().isIntersecting(player.getBoundingSphere()))
            {
                for (const auto &a_hb : o.getHitboxes())
                    if (a_hb.isIntersecting(player.getBoundingSphere()))
                    {
                        glm::vec3 mtv = a_hb.getMTV(player.getBoundingSphere());
                        if (glm::dot(a_hb.getCenter(), player.getTransform().T) < 0)
                        {
                            player.translate(mtv);
                            camera.translate(mtv);
                        }
                        else
                        {
                            player.translate(-mtv);
                            camera.translate(-mtv);
                        }
                    }
            }
        }

        /*
         * Render
         */

        renderer.clear();
        world.getSkybox().render(renderer, camera.getWorldView(), camera.getViewScreen());
        renderer.setShader(shaderPhong);
        renderer.setUniformWorldView(camera.getWorldView());
        renderer.setUniformViewScreen(camera.getViewScreen());
        renderer.setUniformCameraPosition(camera.getPosition());
        
        // Point Lights
        renderer.setUniformActivePointLights((int) world.getEnemies().size());
        std::array<glm::vec3, 10> pointLightPositions;
        std::array<glm::vec3, 10> pointLightColors;
        for (int i = 0; i < world.getEnemies().size(); ++i)
        {
            pointLightPositions[i] = world.getEnemies()[i].getTransform().T;
            pointLightColors[i] = {1, 0, 0};
        }
        renderer.setUniformPointLightPositions(pointLightPositions);
        renderer.setUniformPointLightColors(pointLightColors);
        // Static Objects
        for (const auto &o : world.getStaticObjects())
            o.getModel().render(renderer, camera.getWorldView());

        // Enemies
        for (const auto &e : world.getEnemies())
            e.getModel().render(renderer, camera.getWorldView());

        player.getModel().render(renderer, camera.getWorldView());
        // Terrain
        renderer.setUniformModelWorld(glm::mat4{1.0f});
        renderer.setUniformModelViewNormal(glm::transpose(glm::inverse(camera.getWorldView())));
        renderer.setUniformMaterialAmbient({0.3f, 0.7f, 0.0f});
        renderer.setUniformMaterialDiffuse({0.5f, 0.5f, 0.5f});
        renderer.render(world.getTerrain().getVertexArray());
        // Water
        renderer.enableBlend();
        renderer.setShader(shaderWater);
        renderer.setUniformViewScreen(camera.getViewScreen());
        renderer.setUniformTime(glfwGetTime());
        renderer.setUniformWorldView(camera.getWorldView());
        renderer.setUniformMaterialAmbient({0, 0, 0.3});
        renderer.setUniformMaterialDiffuse({0.0, 0.0, 0.0});
        renderer.setUniformCameraPosition(camera.getPosition());
        world.getWater().getModel().render(renderer, camera.getWorldView());
        renderer.disableBlend();
        // BoundingVolumes
        renderer.enableBlend();
        renderer.setShader(shaderColorSolid);
        renderer.setUniformViewScreen(camera.getViewScreen());
        renderer.setUniformWorldView(camera.getWorldView());
        renderer.setUniformViewScreen(camera.getViewScreen());
        for (const auto &o : world.getStaticObjects())
        {
            //renderer.setUniformModelWorld(o.getAABB().getModelWorld());
            //renderer.render(vaCube);
            //renderer.setUniformModelWorld(o.getBoundingSphere().getModelWorld());
            //renderer.render(vaSphere);
            for (const auto &hb : o.getHitboxes())
            {
                renderer.setUniformModelWorld(hb.getModelWorld());
                renderer.render(vaCube);
            }
        }

        renderer.setUniformModelWorld(player.getBoundingSphere().getModelWorld());
        renderer.render(vaSphere);
        //for (const auto &hb : player.getHitboxes())
        //{
        //    renderer.setUniformModelWorld(hb.getModelWorld());
        //    renderer.render(vaCube);
        //}
        renderer.disableBlend();
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
