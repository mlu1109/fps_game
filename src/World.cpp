#include "World.hpp"
#include <cmath>

void World::addStaticObjectOnRandomLocation(GameObject o)
{
    float x = rand() % m_terrain.getWidth();
    float z = rand() % m_terrain.getHeight();
    float y = m_terrain.getY(x, z);
    while (y < m_waterLevel)
    {
        x = rand() % m_terrain.getWidth();
        z = rand() % m_terrain.getHeight();
        y = m_terrain.getY(x, z);
    }
    glm::vec3 translate(x, y, z);
    glm::vec3 rotate(0, rand(), 0);
    float randX = rand() % 25 * 0.01;
    float randY = rand() % 25 * 0.01;
    float randZ = rand() % 25 * 0.01;
    glm::vec3 scale(1 + randX, 1 + randY, 1 + randZ);
    //o.setRotate(rotate);
    o.setTranslate(translate);
    o.setScale(scale);
    o.update();
    m_staticObjects.emplace_back(std::move(o));
}

void World::addEnemyOnRandomLocation(Enemy e)
{
    float x = rand() % m_terrain.getWidth();
    float z = rand() % m_terrain.getHeight();
    float y = m_terrain.getY(x, z);
    glm::vec3 translate(x, y, z);
    e.setTranslate(translate);
    e.update();
    m_enemies.emplace_back(std::move(e));
}

void World::moveEnemies(const GameObject &o)
{
    for (auto &e : m_enemies)
    {
        e.moveTowardsObject(o);
        e.setY(m_terrain.getY(e.getTransform().T.x, e.getTransform().T.z));
    }
}

void World::handleShot(const Ray &ray)
{
    auto it = m_enemies.begin();
    for (; it != m_enemies.end(); ++it)
    {
        if (it->getBoundingSphere().isIntersecting(ray))
            break;
    }
    
    if (it != m_enemies.end())
        m_enemies.erase(it);
}