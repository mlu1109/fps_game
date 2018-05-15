#pragma once

#include <glm/glm.hpp>
#include "Camera.hpp"

struct Ray
{
    Ray(int screenX, int screenY, int screenWidth, int screenHeight, const Camera&);
    glm::vec3 origin;
    glm::vec3 direction; // Unit
};
