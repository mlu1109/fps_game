#include "Enemy.hpp"

Enemy::Enemy(const Model &m, const OBJ &o)
    : GameObject(m, o) {}

void Enemy::moveTowardsObject(const GameObject &o)
{
    const glm::vec3 &objectPosition = o.getTransform().T;
    const glm::vec3 &position = m_transform.T;
    if (objectPosition == position)
        return;
    
    glm::vec3 direction = glm::normalize(objectPosition - position);
    translate(direction * m_speed);
    update();
}