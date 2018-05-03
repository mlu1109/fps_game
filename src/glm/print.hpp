#pragma once

#include <glm/glm.hpp>
#include <iostream>

std::ostream &operator<<(std::ostream &os, const glm::vec3 &v);
std::ostream &operator<<(std::ostream &os, const glm::mat4 &v);