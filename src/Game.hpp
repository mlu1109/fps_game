#pragma once

#include <vector>
#include "Heightmap.hpp"
#include "Model.hpp"
#include "Ray.hpp"

class Game
{
    std::vector<Model> m_staticObjects;
    Model *m_picked;

  public:
    const std::vector<Model> &getStaticObjects() const { return m_staticObjects; }

    const Model *getPicked() const { return m_picked; }

    Model &addStaticObject(const Model &model) { return m_staticObjects.emplace_back(model); }
    Model &addStaticObject(const std::string &shader, const std::string &texture, const std::string &modelPath);
    Model &addStaticObject(const std::string &shader, const std::string &texture, const std::string &modelPath, const Transform &);

    void pickByRay(const Ray &ray);
};