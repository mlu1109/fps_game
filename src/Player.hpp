#pragma once

#include "GameObject.hpp"
#include "Camera.hpp"

class Player : public GameObject
{
    int m_healthPoints;
    int m_ammo;

  public:
    Player(const Model &, const OBJ &);
};
