#pragma once

#include <glm/glm.hpp>
#include "loaders/OBJ.hpp"

struct BoundingSphere
{
    glm::vec3 center; // Offset from the origin of the enclosed object
    float radius;
};

BoundingSphere getBoundingSphere(const OBJ &);
