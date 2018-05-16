#pragma once

#include "GameObject.hpp"

class Enemy : public GameObject
{
    int m_healthPoints;
    float m_speed = 0.25f;

  public:
    Enemy(const Model &, const OBJ &);
    void moveTowardsObject(const GameObject &o);
};
