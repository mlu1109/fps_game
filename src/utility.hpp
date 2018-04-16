#pragma once

#include <GL/glew.h>
#include <vector>
#include "loaders/obj.hpp"
#include "loaders/tga.hpp"
#include "renderer/vertex.hpp"
#include "renderer/texture.hpp"

/*
 * A bunch of helper functions that have not found a better home
 */

std::pair<std::vector<Vertex>, std::vector<GLuint>> transformOBJ(const OBJ &obj);