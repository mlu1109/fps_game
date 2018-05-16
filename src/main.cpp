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
    GameObject goBox = resourceManager.loadObject("assets/models/opengameart/PlasticContainer.obj", "assets/models/opengameart/PlasticContainer.obj");
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
    const Shader *shaderTexture = resourceManager.loadShader("plain_texture");
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
    const VertexArray *vaPlane = resourceManager.loadVertexArray(OBJ("assets/models/plane.obj").getMeshes()[0]);

    /*
     * Load Textures
     */

    const Texture *texCrosshari = resourceManager.loadTexture("assets/textures/crosshair.tga");

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

        if (t0 - lastSpawn > 5)
        {
            if (world.getEnemies().size() < 10)
            {
                world.addEnemyOnRandomLocation(enemy);
            }
            lastSpawn = t0;
            if (world.getAmmoBoxes().size() < 2)
            {
                world.addAmmoBoxOnRandomLocation(goBox);
            }
            if (world.getHealthBoxes().size() < 2)
            {
                world.addHealthBoxOnRandomLocation(goBox);
            }
        }

        world.moveEnemies(player);

        /*
         * Player
         */

        // Shoot
        if (mouse.hasLeftClicked())
        {
            if (player.getAmmo() <= 0)
            {
                std::cout << "Insufficient ammo!\n";
            }
            else
            {
                player.decreaseAmmo();
                Ray ray(camera.getWindowWidth() / 2, camera.getWindowHeight() / 2, camera.getWindowWidth(), camera.getWindowHeight(), camera);
                if (world.handleShot(ray))
                    player.increaseScore();
            }
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
         * Player enemy collisions
         */

        std::vector<Enemy> &enemies = world.getEnemies();
        for (auto it = enemies.begin(); it != enemies.end(); ++it)
        {
            if (it->getBoundingSphere().isIntersecting(player.getBoundingSphere()))
            {
                player.decraseHealthPoints();
                if (player.getHealthPoints() <= 0)
                {
                    std::cout << "You died! You managed to kill " << player.getScore() << " enemies!\n";
                    return 0;
                }
                enemies.erase(it);
                break;
            }
        }

        /*
         * Player boxes collisions
         */
        // Ammo
        std::vector<GameObject> &ammoBoxes = world.getAmmoBoxes();
        if (player.getAmmo() < player.getMaxAmmo())
        {
            for (auto it = ammoBoxes.begin(); it != ammoBoxes.end(); ++it)
            {
                if (it->getBoundingSphere().isIntersecting(player.getBoundingSphere()))
                {
                    player.increaseAmmo();
                    ammoBoxes.erase(it);
                    break;
                }
            }
        }
        // Health
        std::vector<GameObject> &healthBoxes = world.getHealthBoxes();
        if (player.getHealthPoints() < player.getMaxHealth())
        {
            for (auto it = healthBoxes.begin(); it != healthBoxes.end(); ++it)
            {
                if (it->getBoundingSphere().isIntersecting(player.getBoundingSphere()))
                {
                    player.increaseHealthPoints();
                    healthBoxes.erase(it);
                    break;
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

        renderer.setUniformActivePointLights(world.getEnemies().size() + world.getAmmoBoxes().size() + world.getHealthBoxes().size());
        std::array<glm::vec3, 20> pointLightPositions;
        std::array<glm::vec3, 20> pointLightColors;
        for (size_t i = 0; i < world.getEnemies().size(); ++i)
        {
            pointLightPositions[i] = world.getEnemies()[i].getTransform().T;
            pointLightColors[i] = {1, 0, 0};
        }
        for (size_t i = 0; i < world.getAmmoBoxes().size(); ++i)
        {
            int offset = world.getEnemies().size();
            pointLightPositions[offset + i] = world.getAmmoBoxes()[i].getTransform().T;
            pointLightColors[offset + i] = {0, 0, 1};
        }
        for (size_t i = 0; i < world.getHealthBoxes().size(); ++i)
        {
            int offset = world.getEnemies().size() + world.getAmmoBoxes().size();
            pointLightPositions[offset + i] = world.getHealthBoxes()[i].getTransform().T;
            pointLightColors[offset + i] = {0, 1, 0};
        }
        renderer.setUniformPointLightPositions(pointLightPositions);
        renderer.setUniformPointLightColors(pointLightColors);
        // Static Objects
        for (const auto &o : world.getStaticObjects())
        {
            //            if (glm::dot(o.getTransform().T, camera.getPosition() - camera.getDirection()) > 0)
            o.getModel().render(renderer, camera.getWorldView());
        }

        // Ammo boxes
        for (const auto &a : world.getAmmoBoxes())
        {
            a.getModel().render(renderer, camera.getWorldView());
        }

        // Health boxes
        for (const auto &h : world.getHealthBoxes())
        {
            h.getModel().render(renderer, camera.getWorldView());
        }

        // Enemies
        for (const auto &e : world.getEnemies())
            e.getModel().render(renderer, camera.getWorldView());

        player.getModel().render(renderer, camera.getWorldView());
        // Terrain
        renderer.setUniformModelWorld(glm::mat4{1.0f});
        renderer.setUniformModelViewNormal(glm::transpose(glm::inverse(camera.getWorldView())));
        renderer.setUniformMaterialAmbient({0.3f, 0.7f, 0.3f});
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
        /*
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
        renderer.disableBlend();
        */
        /*
         * HUD
         */
        renderer.disableDepthTest();
        renderer.enableBlend();
        // Crosshair
        renderer.setShader(shaderTexture);
        renderer.setUniformModelWorld(glm::translate(glm::mat4{1.0f}, glm::vec3{0, 0, -1.0f}) * glm::scale(glm::mat4{1.0f}, glm::vec3{0.03, 0.03, 0}));
        renderer.setUniformWorldView(glm::mat4{1.0f});
        renderer.setUniformViewScreen(glm::mat4{1.0f});
        renderer.setTexture(texCrosshari);
        renderer.render(vaPlane);
        // Health/Ammo
        renderer.setShader(shaderColorSolid);
        renderer.setUniformViewScreen(glm::mat4{1.0f});
        renderer.setUniformWorldView(glm::mat4{1.0f});
        float containerWidth = 0.2;
        float containerHeight = 0.04;
        // Health
        renderer.setUniformLightColor(glm::vec3{0.0f, 0.0f, 0.0f});
        float containerX = -0.7f;
        float containerY = -0.8f;
        renderer.setUniformModelWorld(glm::translate(glm::mat4{1.0f}, glm::vec3{containerX, containerY, -1.0f}) * glm::scale(glm::mat4{1.0f}, glm::vec3{containerWidth, containerHeight, 0}));
        renderer.render(vaPlane);

        float healthX = containerX;
        float healthY = containerY;
        float healthWidth = containerWidth;
        healthWidth *= (float)player.getHealthPoints() / (float)player.getMaxHealth();
        float healthHeight = containerHeight;
        renderer.setUniformModelWorld(glm::translate(glm::mat4{1.0f}, glm::vec3{healthX, healthY, -1.0f}) * glm::scale(glm::mat4{1.0f}, glm::vec3{healthWidth, healthHeight, 0}));
        renderer.setUniformLightColor(glm::vec3{0.0f, 1.0f, 0.0f});
        renderer.render(vaPlane);
        // Ammo
        renderer.setUniformLightColor(glm::vec3{0.0f, 0.0f, 0.0f});
        containerX = containerX + 2 * containerWidth;
        renderer.setUniformModelWorld(glm::translate(glm::mat4{1.0f}, glm::vec3{containerX, containerY, -1.0f}) * glm::scale(glm::mat4{1.0f}, glm::vec3{containerWidth, containerHeight, 0}));
        renderer.render(vaPlane);
        float ammoX = containerX;
        float ammoY = containerY;
        float ammoWidth = containerWidth;
        float ammoHeight = containerHeight;
        ammoWidth *= (float)player.getAmmo() / (float)player.getMaxAmmo();
        renderer.setUniformModelWorld(glm::translate(glm::mat4{1.0f}, glm::vec3{ammoX, ammoY, -1.0f}) * glm::scale(glm::mat4{1.0f}, glm::vec3{ammoWidth, ammoHeight, 0}));
        renderer.setUniformLightColor(glm::vec3{0.0f, 1.0f, 1.0f});
        renderer.render(vaPlane);
        renderer.enableDepthTest();

        window.swapBuffers();
        // Print FPS
        // --------------------------------------------------
        ++frameCount;
        double t1 = glfwGetTime();
        if (t1 - t0 >= 1)
        {
            std::cout << "FPS: " << frameCount << '\r';
            frameCount = 0;
            t0 = t1;
        }
    }
}
