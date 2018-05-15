#include "Player.hpp"

Player::Player(const Model &m, const OBJ &o)
    : GameObject(m, o)
{
    update();
}