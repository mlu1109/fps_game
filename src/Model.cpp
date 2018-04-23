#include "Model.hpp"
#include <iostream>

BoundingSphere Model::getTransformedBoundingSphere() const
{
    BoundingSphere bs = m_boundingSphere;
    float maxScale = std::max(m_transform.S.x, std::max(m_transform.S.y, m_transform.S.z));
    bs.radius *= maxScale;
    bs.center += m_transform.T;
    return bs;
}