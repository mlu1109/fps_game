#include "GameObject.hpp"

GameObject::GameObject(const Model &model, const OBJ &obj)
    : m_model(model)
{
    const std::vector<Mesh> meshes = obj.getMeshes();
    std::vector<glm::vec3> positions;
    for (const auto &mesh : meshes)
        for (const auto &v : mesh.vertices)
            positions.push_back(v.position);
    m_aabb = AABB(positions);
    m_boundingSphere = BoundingSphere(positions);

    for (const auto &mesh : meshes)
    {
        positions.clear();
        for (const auto &v : mesh.vertices)
        {
            positions.push_back(v.position);
        }
        m_hitboxes.emplace_back(positions);
    }

    update();
}

GameObject::GameObject(const Model &model)
    : m_model(model)
{
    update();
}

void GameObject::update()
{
    m_model.update(m_transform);
    m_aabb.update(m_transform);
    m_boundingSphere.update(m_transform);
    for (auto &hb : m_hitboxes)
        hb.update(m_transform);
}