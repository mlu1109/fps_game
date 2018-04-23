#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

struct Transform
{
    glm::vec3 T{0, 0, 0}; // Translate
    glm::vec3 R{0, 0, 0}; // Rotate
    glm::vec3 S{1, 1, 1}; // Scale

    glm::mat4 getMatrix() const { return glm::translate(T) * glm::orientate4(R) * glm::scale(S); }
};