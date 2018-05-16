#pragma once

#include "GameObject.hpp"
#include "Camera.hpp"

class Player : public GameObject
{
    int m_healthPoints = 5;
    int m_maxHealth = 5;
    int m_ammo = 5;
    int m_maxAmmo = 5;
    int m_score = 0;

  public:
    Player(const Model &, const OBJ &);
    int getHealthPoints() const { return m_healthPoints; }
    int getMaxHealth() const { return m_maxHealth; }
    int getAmmo() const { return m_ammo; }
    int getMaxAmmo() const { return m_maxAmmo; }
    int getScore() const { return m_score; }
    void decreaseAmmo() { m_ammo -= 1; }
    void decraseHealthPoints() { m_healthPoints -= 1; }
    void increaseScore() { m_score += 1; }
    void increaseAmmo() { m_ammo += 1; }
    void increaseHealthPoints() { m_healthPoints += 1; }
};
