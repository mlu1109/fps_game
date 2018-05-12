#include "GameObject.hpp"

GameObject::GameObject(const Model &model)
    : m_model(model)
{
    update();
}

void GameObject::update()
{
    m_model.update(m_transform);
    //    m_aabb.update(m_transform);
    //    m_boundingSphere.update(m_transform);
}