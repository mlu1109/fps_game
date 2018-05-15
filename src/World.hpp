#pragma once

#include "GameObject.hpp"
#include "Skybox.hpp"
#include "Terrain.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Ray.hpp"

class World
{
    std::vector<GameObject> m_staticObjects;
    std::vector<Enemy> m_enemies;
    std::vector<GameObject> m_beams;
    Terrain m_terrain;
    Skybox m_skybox;
    GameObject m_water;
    float m_waterLevel;

  public:
    World(const Terrain &t, const Skybox &s, const GameObject &water)
        : m_terrain(t), m_skybox(s), m_water(water) { m_waterLevel = water.getTransform().T.y; }

    const std::vector<GameObject> &getStaticObjects() const { return m_staticObjects; }
    const std::vector<Enemy> &getEnemies() const { return m_enemies; }
    const Terrain &getTerrain() const { return m_terrain; }
    const Skybox &getSkybox() const { return m_skybox; }
    const GameObject &getWater() const { return m_water; }
    float getWaterLevel() const { return m_waterLevel; }
    void addStaticObject(const GameObject &o) { m_staticObjects.push_back(o); }
    void addStaticObjectOnRandomLocation(GameObject);
    void addEnemyOnRandomLocation(Enemy);
    void moveEnemies(const GameObject &o);
    void handleShot(const Ray &);
};