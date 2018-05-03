#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Transform
{
    glm::vec3 T{0, 0, 0}; // Translate
    glm::vec3 R{0, 0, 0}; // Rotate
    glm::vec3 S{1, 1, 1}; // Scale

    glm::mat4 getMatrix() const { return glm::translate(glm::mat4(1.0f), T) *
                                         glm::rotate(glm::mat4(1.0f), R.x, glm::vec3{1.0f, 0.0f, 0.0f}) *
                                         glm::rotate(glm::mat4(1.0f), R.y, glm::vec3{0.0f, 1.0f, 0.0f}) *
                                         glm::rotate(glm::mat4(1.0f), R.z, glm::vec3{0.0f, 0.0f, 1.0f}) *
                                         glm::scale(glm::mat4(1.0f), S); }
};
