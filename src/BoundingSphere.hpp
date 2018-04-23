#pragma once

#include <glm/glm.hpp>
#include "renderer/Drawable.hpp"
#include "renderer/VertexArray.hpp"
#include "loaders/OBJ.hpp"
#include "utility.hpp"

struct BoundingSphere
{
    glm::vec3 center; // Offset from the origin of the enclosed object
    float radius;
};

BoundingSphere getBoundingSphere(const OBJ &);
