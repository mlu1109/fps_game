#include "Game.hpp"
#include "renderer/OBJ.hpp"
#include <glm/glm.hpp>
#include <algorithm>
#include <glm/gtx/norm.hpp>

const std::string MODEL_DIR = "assets/models/";

Model &Game::addStaticObject(const std::string &shader, const std::string &texture, const std::string &modelPath)
{
    OBJ obj(MODEL_DIR + modelPath);
    return m_staticObjects.emplace_back(obj.getVertices(), shader, texture, modelPath);
}

Model &Game::addStaticObject(const std::string &shader, const std::string &texture, const std::string &modelPath, const Transform &t)
{
    OBJ obj(MODEL_DIR + modelPath);
    Model &added = m_staticObjects.emplace_back(obj.getVertices(), shader, texture, modelPath);
    added.setTransform(t);
    added.update();
    return added;
}

void Game::pickByRay(const Ray &ray)
{
    std::sort(m_staticObjects.begin(), m_staticObjects.end(), [&](const Model& a, const Model& b){
        return glm::distance2(ray.origin, a.getTransform().T) < glm::distance2(ray.origin, b.getTransform().T);
    });

    for (auto &m : m_staticObjects)
    {
        if (m.getBoundingSphere().isIntersecting(ray))
        {
            m_picked = &m;
            return;
        }
    }
    m_picked = nullptr;
}